import os
import logging
import torch
import torch.nn as nn
from torch.optim import AdamW
from transformers import get_linear_schedule_with_warmup, AutoTokenizer
from tqdm import tqdm
import numpy as np

from model_bert import BertCWS
from dataloader_bert import get_bert_dataloaders, load_cws_data, parse_pku_line

# Word-level F1-score calculation logic (identical to eval.py)
def tags_to_words(chars, tags, id2tag):
    words = []
    buf = ''
    for i, (ch, tag) in enumerate(zip(chars, tags)):
        buf += ch
        if id2tag[tag] in ['E', 'S']:
            words.append(buf)
            buf = ''
    if buf:
        words.append(buf)
    return words

def compute_f1(pred_list, gold_list):
    tp = fp = fn = 0
    for pred, gold in zip(pred_list, gold_list):
        pred_set = set()
        gold_set = set()
        pos = 0
        for w in pred:
            pred_set.add((pos, pos + len(w)))
            pos += len(w)
        pos = 0
        for w in gold:
            gold_set.add((pos, pos + len(w)))
            pos += len(w)

        tp += len(pred_set & gold_set)
        fp += len(pred_set - gold_set)
        fn += len(gold_set - pred_set)

    precision = tp / (tp + fp) if (tp + fp) > 0 else 0
    recall    = tp / (tp + fn) if (tp + fn) > 0 else 0
    f1        = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
    return precision, recall, f1

def evaluate(model, val_loader, tokenizer, id2tag, device, max_len=150):
    model.eval()
    
    # We will compute word-level F1 on validation dataset
    pred_words_list = []
    gold_words_list = []
    
    # To reconstruct sentences correctly, we need the original validation sentences and labels
    # We will get them from the dataset inside val_loader
    dataset = val_loader.dataset
    sentences = dataset.sentences
    labels = dataset.labels
    
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
            
            # Predict
            pred = model.infer(input_ids, attention_mask)[0]  # returns list of predicted tag IDs
            
            # The model's infer method now directly returns character-level predictions (1-to-1 with sentence characters)
            pred_char_tags = pred
            
            # Convert tags to words
            pred_words_list.append(tags_to_words(list(sentence), pred_char_tags, id2tag))
            gold_words_list.append(tags_to_words(list(sentence), gold_tags, id2tag))
            
    p, r, f1 = compute_f1(pred_words_list, gold_words_list)
    return p, r, f1


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
        "format": "bert_cws_state_dict",
        "pretrained_model": "hfl/chinese-roberta-wwm-ext",
        "num_tags": 4,
        "state_dict": model.state_dict(),
    }, path)

def train():
    # Hyperparameters
    pretrained_model = 'hfl/chinese-roberta-wwm-ext'
    batch_size = 32  
    gradient_accumulation_steps = 1  
    learning_rate = 2e-5
    epochs = 8
    max_len = 192  
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    
    my_dir = os.path.dirname(os.path.abspath(__file__))
    train_file = os.path.join(my_dir, "data", "train_expanded.txt")
    save_dir = os.path.join(my_dir, "save")
    os.makedirs(save_dir, exist_ok=True)
    
    # 初始化日志
    set_logger(save_dir)
    
    logging.info(f"Using device: {device}")
    logging.info(f"Hyperparameters: lr={learning_rate}, epochs={epochs}, batch_size={batch_size}, "
                 f"grad_accum={gradient_accumulation_steps}, max_len={max_len}")
    
    # 1. Load Tokenizer & DataLoaders
    tokenizer = AutoTokenizer.from_pretrained(pretrained_model)
    train_loader, val_loader = get_bert_dataloaders(
        train_file, model_name=pretrained_model, batch_size=batch_size, max_len=max_len
    )
    
    # Tag mappings
    id2tag = ['B', 'M', 'E', 'S']
    num_tags = len(id2tag)
    
    # 2. Initialize Model
    logging.info("Initializing RoBERTa + CRF model...")
    model = BertCWS(pretrained_model_name=pretrained_model, num_tags=num_tags)
    # Enable gradient checkpointing to save over 50% VRAM (trading compute for memory)
    model.bert.gradient_checkpointing_enable()
    model.to(device)
    for name, param in model.named_parameters():
        logging.info(f"  {name}: {param.shape}, requires_grad={param.requires_grad}")
    
    # 3. Optimizer & Scheduler
    # We use AdamW and apply slight weight decay
    # 分层学习率: BERT 底层用小 lr 微调, Linear+CRF 用大 lr 从头学习
    bert_params_decay = [p for n, p in model.named_parameters() if 'bert' in n and not any(nd in n for nd in ['bias', 'LayerNorm.weight'])]
    bert_params_no_decay = [p for n, p in model.named_parameters() if 'bert' in n and any(nd in n for nd in ['bias', 'LayerNorm.weight'])]
    task_params = [p for n, p in model.named_parameters() if 'bert' not in n]
    optimizer_grouped_parameters = [
        {'params': bert_params_decay, 'lr': learning_rate, 'weight_decay': 0.01},
        {'params': bert_params_no_decay, 'lr': learning_rate, 'weight_decay': 0.0},
        {'params': task_params, 'lr': 1e-3, 'weight_decay': 0.0},
    ]
    optimizer = AdamW(optimizer_grouped_parameters)
    
    total_steps = len(train_loader) * epochs // gradient_accumulation_steps
    scheduler = get_linear_schedule_with_warmup(
        optimizer, num_warmup_steps=int(0.1 * total_steps), num_training_steps=total_steps
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
        
        # 6. Evaluation after each epoch
        val_p, val_r, val_f1 = evaluate(model, val_loader, tokenizer, id2tag, device, max_len=max_len)
        logging.info(f"Epoch {epoch + 1} validation - Precision: {val_p:.4f}, Recall: {val_r:.4f}, F1: {val_f1:.4f}")
        
        # Save best model
        if val_f1 > best_f1:
            best_f1 = val_f1
            best_model_path = os.path.join(save_dir, "best_model.pkl")
            logging.info(f"New best F1: {best_f1:.4f}! Saving model to {best_model_path}")
            save_model_for_inference(model, best_model_path)
        else:
            logging.info(f"F1 {val_f1:.4f} did not improve from {best_f1:.4f}")
            
    logging.info(f"Training complete! Best validation F1: {best_f1:.4f}")

if __name__ == "__main__":
    train()
