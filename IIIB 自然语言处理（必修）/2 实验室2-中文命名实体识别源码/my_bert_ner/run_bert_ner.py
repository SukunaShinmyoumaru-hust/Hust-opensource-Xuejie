import os
import logging
import pickle
import torch
import torch.nn as nn
from torch.optim import AdamW
from transformers import get_linear_schedule_with_warmup, AutoTokenizer
from tqdm import tqdm
import numpy as np

from model_bert_ner import BertNER
from dataloader_bert_ner import get_bert_ner_dataloaders, load_ner_data


# ==================== 实体级F1评估逻辑 ====================

def extract_entities(chars, tag_ids, id2tag):
    """
    从BIES标签序列中提取实体列表
    实体格式: (实体文本, 实体类型, 起始位置, 结束位置)
    
    BIES标注规则:
        B-XXX: 实体开始
        I-XXX: 实体中间
        E-XXX: 实体结束
        S-XXX: 单字实体
        O:     非实体
    
    Args:
        chars: 字符列表
        tag_ids: 标签ID列表
        id2tag: ID到标签的映射列表
        
    Returns:
        entities: 实体集合 {(实体文本, 实体类型, 起始位置, 结束位置), ...}
    """
    entities = set()
    entity_start = -1
    entity_type = None
    
    for i, tag_id in enumerate(tag_ids):
        if i >= len(chars):
            break
            
        tag = id2tag[tag_id]
        
        if tag.startswith('S-'):
            # 单字实体：直接加入
            etype = tag[2:]
            entities.add((chars[i], etype, i, i))
            entity_start = -1
            entity_type = None
            
        elif tag.startswith('B-'):
            # 实体开始：记录起始位置和类型
            entity_start = i
            entity_type = tag[2:]
            
        elif tag.startswith('I-'):
            # 实体中间：类型必须与B一致，否则重置
            if entity_start == -1 or tag[2:] != entity_type:
                entity_start = -1
                entity_type = None
                
        elif tag.startswith('E-'):
            # 实体结束：类型必须与B一致
            if entity_start != -1 and tag[2:] == entity_type:
                entity_text = ''.join(chars[entity_start:i+1])
                entities.add((entity_text, entity_type, entity_start, i))
            entity_start = -1
            entity_type = None
            
        else:
            # O标签或其他：重置状态
            entity_start = -1
            entity_type = None
    
    return entities


def compute_entity_f1(pred_entities_list, gold_entities_list):
    """
    计算实体级Precision、Recall和F1
    一个实体完全匹配（文本+类型+位置）才算正确
    
    Args:
        pred_entities_list: 每个句子的预测实体集合列表
        gold_entities_list: 每个句子的金标实体集合列表
        
    Returns:
        precision, recall, f1
    """
    tp = fp = fn = 0
    for pred_entities, gold_entities in zip(pred_entities_list, gold_entities_list):
        tp += len(pred_entities & gold_entities)
        fp += len(pred_entities - gold_entities)
        fn += len(gold_entities - pred_entities)
    
    precision = tp / (tp + fp) if (tp + fp) > 0 else 0
    recall    = tp / (tp + fn) if (tp + fn) > 0 else 0
    f1        = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
    return precision, recall, f1


def evaluate(model, val_loader, tokenizer, id2tag, device, max_len=128):
    """
    在验证集上计算实体级F1
    逐句预测，与gold标签对比
    """
    model.eval()
    
    # 从DataLoader获取原始句子和标签
    dataset = val_loader.dataset
    sentences = dataset.sentences
    labels = dataset.labels
    
    pred_entities_list = []
    gold_entities_list = []
    
    print("Evaluating on validation dataset...")
    with torch.no_grad():
        for idx in tqdm(range(len(dataset))):
            sentence = sentences[idx]
            gold_tags = labels[idx]
            
            # Tokenize single sentence
            encoding = tokenizer(
                list(sentence),
                is_split_into_words=True,
                max_length=max_len,
                padding='max_length',
                truncation=True,
                return_tensors='pt'
            )
            
            input_ids = encoding['input_ids'].to(device)
            attention_mask = encoding['attention_mask'].to(device)
            
            # Predict: model.infer返回字符级预测（1-to-1对应句子中每个字符）
            pred = model.infer(input_ids, attention_mask)[0]  # returns list of predicted tag IDs
            pred_char_tags = pred
            
            # 提取预测实体和金标实体
            chars = list(sentence)
            pred_entities = extract_entities(chars, pred_char_tags, id2tag)
            gold_entities = extract_entities(chars, gold_tags, id2tag)
            
            pred_entities_list.append(pred_entities)
            gold_entities_list.append(gold_entities)
    
    p, r, f1 = compute_entity_f1(pred_entities_list, gold_entities_list)
    return p, r, f1


# ==================== 训练逻辑 ====================

def set_logger(save_dir):
    """设置日志：同时输出到控制台和 save/log.txt"""
    os.makedirs(save_dir, exist_ok=True)
    log_file = os.path.join(save_dir, 'log.txt')
    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        level=logging.INFO,
        datefmt='%Y-%m-%d %H:%M:%S',
        filename=log_file,
        filemode='w',
    )
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    console.setFormatter(logging.Formatter('%(asctime)s %(levelname)-8s %(message)s'))
    logging.getLogger('').addHandler(console)

def save_model_for_inference(model, path):
    """Save a portable checkpoint instead of pickling the full Transformers model."""
    torch.save({
        "format": "bert_ner_state_dict",
        "pretrained_model": "hfl/chinese-roberta-wwm-ext",
        "num_tags": model.hidden2tag.out_features,
        "lstm_hidden": model.lstm.hidden_size,
        "lstm_layers": model.lstm.num_layers,
        "dropout": model.dropout.p,
        "state_dict": model.state_dict(),
    }, path)

def train():
    # Hyperparameters
    pretrained_model = 'hfl/chinese-roberta-wwm-ext'
    batch_size = 32
    gradient_accumulation_steps = 1  
    bert_lr = 2e-5       # BERT 底层微调用小学习率
    task_lr = 5e-4       # BiLSTM + Linear 从头学习用中等学习率
    crf_lr = 1e-3        # CRF 转移矩阵用大学习率
    epochs = 10           # 数据量大(9:1切分), 多训几轮
    max_len = 192  # 增加输入长度上限以适应验证集中的长句子
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    
    my_dir = os.path.dirname(os.path.abspath(__file__))
    train_file = os.path.join(my_dir, "data", "ner_train.txt")
    valid_file = os.path.join(my_dir, "data", "ner_valid.txt")
    save_dir = os.path.join(my_dir, "save")
    os.makedirs(save_dir, exist_ok=True)
    
    # 初始化日志
    set_logger(save_dir)
    
    logging.info(f"Using device: {device}")
    logging.info(f"Hyperparameters: bert_lr={bert_lr}, task_lr={task_lr}, crf_lr={crf_lr}, "
                 f"epochs={epochs}, batch_size={batch_size}, grad_accum={gradient_accumulation_steps}")
    
    # 1. Load Tokenizer & DataLoaders
    tokenizer = AutoTokenizer.from_pretrained(pretrained_model)
    train_loader, val_loader, tag2id, id2tag = get_bert_ner_dataloaders(
        train_file, valid_file, model_name=pretrained_model, batch_size=batch_size, max_len=max_len
    )
    
    # Tag mappings (从数据中自动构建)
    num_tags = len(id2tag)
    logging.info(f"Number of NER tags: {num_tags}")
    logging.info(f"Tag mappings: {tag2id}")
    
    # 2. Initialize Model
    logging.info("Initializing RoBERTa + BiLSTM + CRF NER model...")
    model = BertNER(
        pretrained_model_name=pretrained_model,
        num_tags=num_tags,
        lstm_hidden=512,   # BiLSTM 每方向512维, 双向输出1024维
        lstm_layers=2,     # 2层BiLSTM
        dropout=0.3        # Dropout 0.3
    )
    # Enable gradient checkpointing to save VRAM
    model.bert.gradient_checkpointing_enable()
    model.to(device)
    for name, param in model.named_parameters():
        logging.info(f"  {name}: {param.shape}, requires_grad={param.requires_grad}")
    
    # 3. Optimizer & Scheduler (分层学习率)
    bert_params_decay = [p for n, p in model.named_parameters()
                         if 'bert' in n and not any(nd in n for nd in ['bias', 'LayerNorm.weight'])]
    bert_params_no_decay = [p for n, p in model.named_parameters()
                            if 'bert' in n and any(nd in n for nd in ['bias', 'LayerNorm.weight'])]
    lstm_linear_params = [p for n, p in model.named_parameters()
                          if 'bert' not in n and 'crf' not in n]
    crf_params = [p for n, p in model.named_parameters() if 'crf' in n]
    
    optimizer = AdamW([
        {'params': bert_params_decay,    'lr': bert_lr, 'weight_decay': 0.01},
        {'params': bert_params_no_decay, 'lr': bert_lr, 'weight_decay': 0.0},
        {'params': lstm_linear_params,   'lr': task_lr, 'weight_decay': 0.01},
        {'params': crf_params,           'lr': crf_lr,  'weight_decay': 0.0},
    ])
    
    total_steps = len(train_loader) * epochs // gradient_accumulation_steps
    scheduler = get_linear_schedule_with_warmup(
        optimizer, num_warmup_steps=int(0.15 * total_steps), num_training_steps=total_steps
    )
    
    # 4. FP16 Mixed Precision (modern PyTorch 2.x API)
    scaler = torch.amp.GradScaler('cuda')
    
    best_f1 = 0.0
    
    # 5. Training Loop
    for epoch in range(epochs):
        model.train()
        logging.info(f"--- Epoch {epoch + 1}/{epochs} ---")
        epoch_loss = 0.0
        
        # Progress bar
        pbar = tqdm(train_loader, desc="Training")
        
        optimizer.zero_grad()
        
        for step, (input_ids, attention_mask, label_ids) in enumerate(pbar):
            input_ids = input_ids.to(device)
            attention_mask = attention_mask.to(device)
            label_ids = label_ids.to(device)
            
            # Forward pass under autocast for fp16 (modern PyTorch 2.x API)
            with torch.amp.autocast('cuda'):
                loss = model(input_ids, attention_mask, label_ids)
                # Normalize loss to account for gradient accumulation
                loss = loss / gradient_accumulation_steps
                
            # Backward pass with scaler
            scaler.scale(loss).backward()
            
            epoch_loss += loss.item() * gradient_accumulation_steps
            
            # Update weights after accumulated steps
            if (step + 1) % gradient_accumulation_steps == 0 or (step + 1) == len(train_loader):
                scaler.unscale_(optimizer)
                # Clip gradients to prevent exploding gradients
                torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                
                scaler.step(optimizer)
                scaler.update()
                
                scheduler.step()
                optimizer.zero_grad()
                
            pbar.set_postfix({'loss': f"{loss.item() * gradient_accumulation_steps:.4f}"})
            
        avg_loss = epoch_loss / len(train_loader)
        logging.info(f"Epoch {epoch + 1} average training loss: {avg_loss:.4f}")
        
        # 6. Evaluation after each epoch (实体级F1)
        val_p, val_r, val_f1 = evaluate(model, val_loader, tokenizer, id2tag, device, max_len=max_len)
        logging.info(f"Epoch {epoch + 1} validation - Precision: {val_p:.4f}, Recall: {val_r:.4f}, F1: {val_f1:.4f}")
        
        # Save best model based on entity-level F1
        if val_f1 > best_f1:
            best_f1 = val_f1
            best_model_path = os.path.join(save_dir, "best_ner_model.pkl")
            logging.info(f"New best F1: {best_f1:.4f}! Saving model to {best_model_path}")
            save_model_for_inference(model, best_model_path)
            tag_map_path = os.path.join(save_dir, "ner_tag_mappings.pkl")
            with open(tag_map_path, 'wb') as f:
                pickle.dump({'tag2id': tag2id, 'id2tag': id2tag}, f)
            logging.info(f"Tag mappings saved to {tag_map_path}")
        else:
            logging.info(f"F1 {val_f1:.4f} did not improve from {best_f1:.4f}")
            
    logging.info(f"Training complete! Best validation entity-level F1: {best_f1:.4f}")

if __name__ == "__main__":
    train()
