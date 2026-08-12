import os
import argparse
import torch
from transformers import AutoTokenizer
from tqdm import tqdm

from model_bert import BertCWS
from dataloader_bert import get_bert_dataloaders

def load_cws_model(model_path, device):
    checkpoint = torch.load(model_path, map_location=device, weights_only=False)
    if isinstance(checkpoint, dict) and checkpoint.get("format") == "bert_cws_state_dict":
        model = BertCWS(
            pretrained_model_name=checkpoint.get("pretrained_model", "hfl/chinese-roberta-wwm-ext"),
            num_tags=checkpoint.get("num_tags", 4),
        )
        model.load_state_dict(checkpoint["state_dict"])
        model.to(device)
        return model
    return checkpoint

def load_segments_file(file_path):
    segments_list = []
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            words = line.strip().split()
            segments_list.append(words)
    return segments_list

def load_dictionary(dict_path):
    dictionary_words = set()
    if os.path.exists(dict_path):
        print(f"Loading dictionary from {dict_path}...")
        with open(dict_path, 'r', encoding='utf-8') as f:
            for line in f:
                parts = line.strip().split()
                if parts:
                    dictionary_words.add(parts[0])
        print(f"Loaded {len(dictionary_words)} words in dictionary.")
    return dictionary_words

def dictionary_correct(segments, dictionary_words):
    if not dictionary_words:
        return segments
        
    i = 0
    corrected_segments = []
    while i < len(segments):
        # 1. Merge adjacent words if the combined form is in the dictionary (highly reliable for PKU-standard)
        if i + 1 < len(segments):
            combined = segments[i] + segments[i+1]
            if combined in dictionary_words:
                corrected_segments.append(combined)
                i += 2
                continue
                
        # 2. Merge three adjacent words if the combination exists
        if i + 2 < len(segments):
            combined = segments[i] + segments[i+1] + segments[i+2]
            if combined in dictionary_words:
                corrected_segments.append(combined)
                i += 3
                continue
                
        # 3. Split long words that are NOT in the dictionary if their parts are high-confidence words
        word = segments[i]
        if len(word) >= 4 and word not in dictionary_words:
            split_done = False
            for split_idx in range(2, len(word) - 1):
                part1 = word[:split_idx]
                part2 = word[split_idx:]
                if part1 in dictionary_words and part2 in dictionary_words:
                    corrected_segments.extend([part1, part2])
                    split_done = True
                    break
            if split_done:
                i += 1
                continue
                
        corrected_segments.append(segments[i])
        i += 1
        
    return corrected_segments

def compute_f1(pred_list, gold_list):
    tp = fp = fn = 0
    
    for idx, (pred, gold) in enumerate(zip(pred_list, gold_list)):
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

def evaluate_model_on_val(model_path, train_file, dict_path, use_dict, device):
    if not os.path.exists(model_path):
        print(f"Error: Model file not found at {model_path}")
        return
    if not os.path.exists(train_file):
        print(f"Error: Training file not found at {train_file}")
        return
        
    print(f"Loading model from {model_path} onto {device}...")
    model = load_cws_model(model_path, device)
    model.eval()
    
    pretrained_model = 'hfl/chinese-roberta-wwm-ext'
    tokenizer = AutoTokenizer.from_pretrained(pretrained_model)
    
    # Load dictionary if enabled
    dictionary_words = load_dictionary(dict_path) if use_dict else set()
    
    print("Loading validation dataset split...")
    _, val_loader = get_bert_dataloaders(
        train_file, model_name=pretrained_model, batch_size=4, max_len=128
    )
    
    id2tag = ['B', 'M', 'E', 'S']
    
    pred_words_list = []
    gold_words_list = []
    
    dataset = val_loader.dataset
    sentences = dataset.sentences
    labels = dataset.labels
    
    def tags_to_words(chars, tags):
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
        
    print("Running evaluation on local validation split...")
    corrected_count = 0
    for idx in tqdm(range(len(dataset)), desc="Evaluating"):
        sentence = sentences[idx]
        gold_tags = labels[idx]
        
        encoding = tokenizer(
            list(sentence),
            is_split_into_words=True,
            max_length=128,
            padding='max_length',
            truncation=True,
            return_tensors='pt'
        )
        
        input_ids = encoding['input_ids'].to(device)
        attention_mask = encoding['attention_mask'].to(device)
        
        with torch.no_grad():
            pred = model.infer(input_ids, attention_mask)[0]
            
        model_segments = tags_to_words(list(sentence), pred)
        
        if use_dict and dictionary_words:
            final_segments = dictionary_correct(model_segments, dictionary_words)
            if final_segments != model_segments:
                corrected_count += 1
        else:
            final_segments = model_segments
            
        pred_words_list.append(final_segments)
        gold_words_list.append(tags_to_words(list(sentence), gold_tags))
        
    p, r, f1 = compute_f1(pred_words_list, gold_words_list)
    
    print(f"\n{'='*45}")
    print(f"Validation Dataset: {os.path.basename(train_file)} (10% Split)")
    print(f"Line Count        : {len(dataset)}")
    print(f"Dictionary Correct: {'ENABLED' if use_dict else 'DISABLED'}")
    if use_dict:
        print(f"Corrected Lines   : {corrected_count} / {len(dataset)} ({corrected_count/len(dataset)*100:.2f}%)")
    print(f"Precision         : {p:.4f}")
    print(f"Recall            : {r:.4f}")
    print(f"Word F1-score     : {f1:.4f}  (True local validation metric)")
    print(f"{'='*45}")

def main():
    parser = argparse.ArgumentParser(description="Evaluate Chinese Word Segmentation F1-score")
    parser.add_argument("--mode", type=str, default="val", choices=["val", "file"], 
                        help="val: evaluate saved model on validation split; file: compare two space-separated files")
    parser.add_argument("--model_path", type=str, default="save/best_model.pkl", help="Path to saved model (used in 'val' mode)")
    parser.add_argument("--pred_path", type=str, default="cws_result_bert.txt", help="Path to predicted segments (used in 'file' mode)")
    parser.add_argument("--gold_path", type=str, default="", help="Path to gold standard segments (used in 'file' mode)")
    parser.add_argument("--dict_path", type=str, default="data/dict_pku_only.txt", help="Path to dictionary for evaluation post-processing")
    parser.add_argument("--use_dict", action="store_true", default=False, help="Whether to apply dictionary post-processing during val evaluation")
    
    args = parser.parse_args()
    
    my_dir = os.path.dirname(os.path.abspath(__file__))
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    
    if args.mode == "val":
        model_path = os.path.join(my_dir, args.model_path)
        train_file = os.path.join(my_dir, "data", "train_expanded.txt")
        dict_path = os.path.join(my_dir, args.dict_path)
        evaluate_model_on_val(model_path, train_file, dict_path, args.use_dict, device)
    else:
        pred_path = os.path.join(my_dir, args.pred_path)
        gold_path = args.gold_path
        if not gold_path:
            gold_path = os.path.abspath(os.path.join(my_dir, "..", "2 基于深度学习版 （Bi-LSTM+CRF）", "data", "test.txt"))
            
        if not os.path.exists(pred_path):
            print(f"Error: Predicted segments file not found at {pred_path}")
            return
        if not os.path.exists(gold_path):
            print(f"Error: Gold standard segments file not found at {gold_path}")
            return
            
        print(f"Loading predictions from {pred_path}...")
        pred_list = load_segments_file(pred_path)
        
        print(f"Loading gold standard from {gold_path}...")
        gold_list = load_segments_file(gold_path)
        
        # Check if gold standard has spaces (basic integrity check)
        has_spaces = any(len(line) > 1 for line in gold_list)
        if not has_spaces:
            print("\nWARNING: The gold standard file seems to contain UNSEGMENTED text (no spaces).")
            print("Evaluating against an unsegmented file will result in near-zero F1-score.")
            print("Please use --mode val to evaluate the model on the local validation split instead.\n")
            
        if len(pred_list) != len(gold_list):
            print(f"Warning: File lengths differ! Predictions: {len(pred_list)} lines, Gold: {len(gold_list)} lines.")
            min_len = min(len(pred_list), len(gold_list))
            pred_list = pred_list[:min_len]
            gold_list = gold_list[:min_len]
            
        p, r, f1 = compute_f1(pred_list, gold_list)
        
        print(f"\n{'='*45}")
        print(f"Evaluation dataset: {os.path.basename(gold_path)}")
        print(f"Line count        : {len(pred_list)}")
        print(f"Precision         : {p:.4f}")
        print(f"Recall            : {r:.4f}")
        print(f"Word F1-score     : {f1:.4f}  (Local file comparison)")
        print(f"{'='*45}")

if __name__ == "__main__":
    main()
