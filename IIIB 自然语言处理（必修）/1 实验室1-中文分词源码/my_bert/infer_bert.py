import os
import torch
import argparse
from transformers import AutoTokenizer
from tqdm import tqdm
from model_bert import BertCWS

# Mapping tags
id2tag = ['B', 'M', 'E', 'S']

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
                
        corrected_segments.append(segments[i])
        i += 1
        
    return corrected_segments

def main():
    parser = argparse.ArgumentParser(description="Inference script for RoBERTa + CRF CWS")
    parser.add_argument("--model_path", type=str, default="save/best_model.pkl", help="Path to the saved model")
    parser.add_argument("--input_path", type=str, default="", help="Path to raw test file")
    parser.add_argument("--output_path", type=str, default="cws_result_bert.txt", help="Path to write output segmentations")
    parser.add_argument("--dict_path", type=str, default="data/dict_pku_only.txt", help="Path to dictionary for post-processing")
    parser.add_argument("--use_dict", action="store_true", help="Whether to apply dictionary post-processing")
    
    args = parser.parse_args()
    
    my_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Default model path
    model_path = os.path.join(my_dir, args.model_path)
    if not os.path.exists(model_path):
        print(f"Error: Model not found at {model_path}. Please train the model first.")
        return
        
    # Default input path
    input_path = args.input_path
    if not input_path:
        # Try local test.txt
        input_path = os.path.abspath(os.path.join(my_dir, "..", "2 基于深度学习版 （Bi-LSTM+CRF）", "data", "test.txt"))
        
    if not os.path.exists(input_path):
        print(f"Error: Input file not found at {input_path}")
        return
        
    output_path = os.path.join(my_dir, args.output_path)
    dict_path = os.path.join(my_dir, args.dict_path)
    
    # Load model and tokenization settings
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    print(f"Loading model from {model_path} onto {device}...")
    model = load_cws_model(model_path, device)
    model.eval()
    
    pretrained_model = 'hfl/chinese-roberta-wwm-ext'
    tokenizer = AutoTokenizer.from_pretrained(pretrained_model)
    
    # Load dictionary if enabled
    dictionary_words = load_dictionary(dict_path) if args.use_dict else set()
    
    print(f"Processing lines in {input_path} and writing to {output_path}...")
    
    corrected_count = 0
    total_lines = 0
    
    with open(input_path, 'r', encoding='utf-8', errors='ignore') as in_f, \
         open(output_path, 'w', encoding='utf-8') as out_f:
             
        lines = in_f.readlines()
        total_lines = len(lines)
        
        for line in tqdm(lines, desc="Inference"):
            line = line.strip()
            if not line:
                out_f.write("\n")
                continue
                
            # Tokenize single line
            encoding = tokenizer(
                list(line),
                is_split_into_words=True,
                max_length=512,  # Increase max length for full-line test inputs
                padding=False,   # No padding needed for single-line inference
                truncation=True,
                return_tensors='pt'
            )
            
            input_ids = encoding['input_ids'].to(device)
            attention_mask = encoding['attention_mask'].to(device)
            
            with torch.no_grad():
                pred = model.infer(input_ids, attention_mask)[0]
                
            # The model's infer method now directly returns character-level predictions (1-to-1 with sentence characters)
            pred_char_tags = pred
                    
            # 1. Get model's segmentations
            model_segments = tags_to_words(list(line), pred_char_tags)
            
            # 2. Correct via dictionary
            if args.use_dict and dictionary_words:
                final_segments = dictionary_correct(model_segments, dictionary_words)
                if final_segments != model_segments:
                    corrected_count += 1
            else:
                final_segments = model_segments
                
            # 3. Write output
            out_f.write(" ".join(final_segments) + "\n")
            
    print(f"\nInference complete! Segmented {total_lines} lines.")
    if args.use_dict and dictionary_words:
        print(f"Dictionary post-processing corrected {corrected_count} lines out of {total_lines} ({corrected_count/total_lines*100:.2f}%).")

if __name__ == "__main__":
    main()
