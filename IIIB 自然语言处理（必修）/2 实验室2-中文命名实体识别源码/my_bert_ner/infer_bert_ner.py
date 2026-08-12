import os
import torch
import pickle
import argparse
from transformers import AutoTokenizer
from tqdm import tqdm
from model_bert_ner import BertNER

def load_ner_model(model_path, device):
    checkpoint = torch.load(model_path, map_location=device, weights_only=False)
    if isinstance(checkpoint, dict) and checkpoint.get("format") == "bert_ner_state_dict":
        model = BertNER(
            pretrained_model_name=checkpoint.get("pretrained_model", "hfl/chinese-roberta-wwm-ext"),
            num_tags=checkpoint["num_tags"],
            lstm_hidden=checkpoint.get("lstm_hidden", 512),
            lstm_layers=checkpoint.get("lstm_layers", 2),
            dropout=checkpoint.get("dropout", 0.3),
        )
        model.load_state_dict(checkpoint["state_dict"])
        model.to(device)
        return model
    return checkpoint


def extract_entities(chars, tag_ids, id2tag):
    """
    从BIES标签序列中提取实体列表
    
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
        entities: 实体列表 [(实体文本, 实体类型, 起始位置, 结束位置), ...]
    """
    entities = []
    entity_start = -1
    entity_type = None
    
    for i, tag_id in enumerate(tag_ids):
        if i >= len(chars):
            break
            
        tag = id2tag[tag_id]
        
        if tag.startswith('S-'):
            # 单字实体：直接加入
            etype = tag[2:]
            entities.append((chars[i], etype, i, i))
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
                entities.append((entity_text, entity_type, entity_start, i))
            entity_start = -1
            entity_type = None
            
        else:
            # O标签或其他：重置状态
            entity_start = -1
            entity_type = None
    
    return entities


def load_test_sentences(file_path):
    """
    从NER测试文件中加载句子
    数据格式: 每行 "字 标签"，空行分隔句子
    
    Args:
        file_path: 测试数据文件路径
        
    Returns:
        sentences: 句子字符串列表
        gold_tag_strs: 金标标签字符串列表的列表（如果有标签的话）
    """
    sentences = []
    gold_tag_strs = []
    
    current_chars = []
    current_tags = []
    
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                if current_chars:
                    sentences.append(''.join(current_chars))
                    gold_tag_strs.append(current_tags)
                    current_chars = []
                    current_tags = []
                continue
            
            parts = line.split(' ')
            if len(parts) >= 2:
                current_chars.append(parts[0])
                current_tags.append(parts[1])
            elif len(parts) == 1:
                # 只有字符没有标签（纯推理模式）
                current_chars.append(parts[0])
                current_tags.append('O')
        
        # 处理最后一个句子
        if current_chars:
            sentences.append(''.join(current_chars))
            gold_tag_strs.append(current_tags)
    
    return sentences, gold_tag_strs


def main():
    parser = argparse.ArgumentParser(description="Inference script for RoBERTa + CRF NER (命名实体识别推理脚本)")
    parser.add_argument("--model_path", type=str, default="save/best_ner_model.pkl", 
                        help="Path to the saved NER model")
    parser.add_argument("--tag_map_path", type=str, default="save/ner_tag_mappings.pkl",
                        help="Path to saved tag mappings")
    parser.add_argument("--input_path", type=str, 
                        default="data/ner_test.txt", 
                        help="Path to NER test file")
    parser.add_argument("--output_path", type=str, default="ner_result_bert.txt", 
                        help="Path to write output NER results")
    
    args = parser.parse_args()
    
    my_dir = os.path.dirname(os.path.abspath(__file__))
    
    # 加载模型
    model_path = os.path.join(my_dir, args.model_path)
    if not os.path.exists(model_path):
        print(f"Error: Model not found at {model_path}. Please train the model first (run run_bert_ner.py).")
        return
    
    # 加载标签映射
    tag_map_path = os.path.join(my_dir, args.tag_map_path)
    if not os.path.exists(tag_map_path):
        print(f"Error: Tag mappings not found at {tag_map_path}. Please train the model first.")
        return
    
    with open(tag_map_path, 'rb') as f:
        tag_mappings = pickle.load(f)
        tag2id = tag_mappings['tag2id']
        id2tag = tag_mappings['id2tag']
    
    print(f"Loaded {len(id2tag)} tags: {id2tag}")
    
    # 加载测试数据
    input_path = os.path.join(my_dir, args.input_path)
    if not os.path.exists(input_path):
        print(f"Error: Input file not found at {input_path}")
        return
    
    output_path = os.path.join(my_dir, args.output_path)
    
    # Load model and tokenizer
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    print(f"Loading model from {model_path} onto {device}...")
    model = load_ner_model(model_path, device)
    model.eval()
    
    pretrained_model = 'hfl/chinese-roberta-wwm-ext'
    tokenizer = AutoTokenizer.from_pretrained(pretrained_model)
    
    # 加载测试句子
    sentences, gold_tag_strs = load_test_sentences(input_path)
    print(f"Loaded {len(sentences)} test sentences from {input_path}")
    print(f"Writing results to {output_path}...")
    
    # 统计实体级F1（如果有金标标签）
    total_pred_entities = []
    total_gold_entities = []
    has_gold = any(any(t != 'O' for t in tags) for tags in gold_tag_strs)
    
    with open(output_path, 'w', encoding='utf-8') as out_f:
        for i, sentence in enumerate(tqdm(sentences, desc="Inference")):
            if not sentence:
                continue
            
            # Tokenize single sentence
            encoding = tokenizer(
                list(sentence),
                is_split_into_words=True,
                max_length=512,  # 推理时使用更大的max_length
                padding=False,   # 单句推理不需要padding
                truncation=True,
                return_tensors='pt'
            )
            
            input_ids = encoding['input_ids'].to(device)
            attention_mask = encoding['attention_mask'].to(device)
            
            with torch.no_grad():
                pred = model.infer(input_ids, attention_mask)[0]
            
            # model.infer直接返回字符级预测（1-to-1对应句子中每个字符）
            pred_char_tags = pred
            chars = list(sentence)
            
            # 提取预测实体
            pred_entities = extract_entities(chars, pred_char_tags, id2tag)
            
            # 输出原始标注格式（每行: 字 预测标签）
            for j, ch in enumerate(chars):
                if j < len(pred_char_tags):
                    tag_str = id2tag[pred_char_tags[j]]
                else:
                    tag_str = 'O'
                out_f.write(f"{ch} {tag_str}\n")
            out_f.write("\n")
            
            # 输出提取的实体信息到控制台（前10个句子）
            if i < 10 and pred_entities:
                print(f"\n句子{i+1}: {sentence}")
                for ent_text, ent_type, start, end in pred_entities:
                    print(f"  [{ent_type}] {ent_text} (位置: {start}-{end})")
            
            # 收集实体用于计算F1
            total_pred_entities.append(set((e[0], e[1], e[2], e[3]) for e in pred_entities))
            
            if has_gold and i < len(gold_tag_strs):
                gold_tag_ids = [tag2id.get(t, tag2id['O']) for t in gold_tag_strs[i]]
                gold_entities = extract_entities(chars, gold_tag_ids, id2tag)
                total_gold_entities.append(set((e[0], e[1], e[2], e[3]) for e in gold_entities))
    
    print(f"\nInference complete! Processed {len(sentences)} sentences.")
    print(f"Results saved to {output_path}")
    
    # 如果有金标标签，计算并输出实体级F1
    if has_gold and total_gold_entities:
        tp = fp = fn = 0
        for pred_ents, gold_ents in zip(total_pred_entities, total_gold_entities):
            tp += len(pred_ents & gold_ents)
            fp += len(pred_ents - gold_ents)
            fn += len(gold_ents - pred_ents)
        
        precision = tp / (tp + fp) if (tp + fp) > 0 else 0
        recall    = tp / (tp + fn) if (tp + fn) > 0 else 0
        f1        = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
        
        print(f"\n===== Test Set Entity-Level Evaluation =====")
        print(f"Precision: {precision:.4f}")
        print(f"Recall:    {recall:.4f}")
        print(f"F1-score:  {f1:.4f}")
        print(f"TP: {tp}, FP: {fp}, FN: {fn}")


if __name__ == "__main__":
    main()
