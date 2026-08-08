import os
import sys
import torch
import pickle
import argparse
from transformers import AutoTokenizer
from tqdm import tqdm

# Add CWS and NER directories to python path for loading custom models
my_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(my_dir)
sys.path.append(os.path.abspath(os.path.join(my_dir, "..", "..", "1 实验室1-中文分词源码", "my_bert")))

from model_bert_ner import BertNER
from model_bert import BertCWS

# Tag mapping for CWS
CWS_TAGS = ['B', 'M', 'E', 'S']

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

def load_dictionary(dict_path):
    """Load PKU-standard dictionary for CWS post-processing"""
    dictionary_words = set()
    if os.path.exists(dict_path):
        print(f"Loading CWS dictionary from {dict_path}...")
        with open(dict_path, 'r', encoding='utf-8') as f:
            for line in f:
                parts = line.strip().split()
                if parts:
                    dictionary_words.add(parts[0])
        print(f"Loaded {len(dictionary_words)} words in CWS dictionary.")
    else:
        print(f"Warning: CWS dictionary not found at {dict_path}")
    return dictionary_words

def dictionary_correct(segments, dictionary_words):
    """Correct segmentation using dictionary rules"""
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

def cws_tags_to_words(chars, tags):
    """Convert CWS BMES tag IDs to segmented words"""
    words = []
    buf = ''
    for ch, tag_id in zip(chars, tags):
        buf += ch
        tag = CWS_TAGS[tag_id]
        if tag in ['E', 'S']:
            words.append(buf)
            buf = ''
    if buf:
        words.append(buf)
    return words

def extract_entities(chars, tag_ids, id2tag):
    """
    Extract entity list from BIES tags
    Returns: list of (entity_text, entity_type, start_idx, end_idx)
    """
    entities = []
    entity_start = -1
    entity_type = None
    
    for i, tag_id in enumerate(tag_ids):
        if i >= len(chars):
            break
            
        tag = id2tag[tag_id]
        
        if tag.startswith('S-'):
            etype = tag[2:]
            entities.append((chars[i], etype, i, i))
            entity_start = -1
            entity_type = None
            
        elif tag.startswith('B-'):
            entity_start = i
            entity_type = tag[2:]
            
        elif tag.startswith('I-'):
            if entity_start == -1 or tag[2:] != entity_type:
                entity_start = -1
                entity_type = None
                
        elif tag.startswith('E-'):
            if entity_start != -1 and tag[2:] == entity_type:
                entity_text = ''.join(chars[entity_start:i+1])
                entities.append((entity_text, entity_type, entity_start, i))
            entity_start = -1
            entity_type = None
            
        else:
            entity_start = -1
            entity_type = None
            
    return entities


def perform_boundary_fusion(chars, cws_words, entities, allowed_entity_types=None):
    """
    Merge CWS words only inside reliable named-entity spans.
    PKU-style fusion keeps the CWS model as the primary segmenter and uses NER
    only as a conservative proper-name repair signal.
    
    Args:
        chars: List of characters in the sentence.
        cws_words: List of words segmented by the CWS model.
        entities: List of (entity_text, entity_type, start_idx, end_idx) from the NER model.
        allowed_entity_types: Entity types allowed to repair CWS boundaries.
        
    Returns:
        fused_words: List of final segmented words.
    """
    n = len(chars)
    if n == 0:
        return []

    if allowed_entity_types is None:
        allowed_entity_types = {"PER", "LOC", "ORG"}
        
    # Represent word boundaries as a boolean array where is_word_end[i] is True if index i is the end of a word.
    is_word_end = [False] * n
    
    # 2. Populate initial CWS boundaries
    cur_idx = 0
    for word in cws_words:
        w_len = len(word)
        if w_len > 0:
            cur_idx += w_len
            is_word_end[cur_idx - 1] = True
            
    # 3. Apply NER boundaries as merge-only constraints
    # Characters strictly inside an entity cannot be the end of a word.
    # We do NOT force is_word_end[end] = True or is_word_end[start - 1] = True,
    # because CWS is already highly accurate at word separation. This avoids
    # slicing CWS-grouped words (e.g., keeping "中国人" instead of splitting into "中国 / 人").
    for ent_text, ent_type, start, end in entities:
        if start < 0 or end >= n or start > end:
            continue
        if ent_type not in allowed_entity_types:
            continue
        if end == start:
            continue
            
        # Ensure characters inside the entity are merged
        for k in range(start, end):
            is_word_end[k] = False
            
    # Always make sure the last character ends the sentence
    is_word_end[n - 1] = True
    
    # 4. Reconstruct final segmented words from boundaries
    fused_words = []
    buf = []
    for i, ch in enumerate(chars):
        buf.append(ch)
        if is_word_end[i]:
            fused_words.append("".join(buf))
            buf = []
    if buf:
        fused_words.append("".join(buf))
        
    return fused_words


def main():
    parser = argparse.ArgumentParser(description="Segment text using CWS + NER Boundary Fusion (策略1)")
    parser.add_argument("--cws_model_path", type=str, 
                        default="../../1 实验室1-中文分词源码/my_bert/save/best_model.pkl",
                        help="Path to CWS model")
    parser.add_argument("--ner_model_path", type=str, 
                        default="save/best_ner_model.pkl",
                        help="Path to NER model")
    parser.add_argument("--ner_tag_map", type=str, 
                        default="save/ner_tag_mappings.pkl",
                        help="Path to NER tag mappings")
    parser.add_argument("--input_path", type=str, 
                        default="../../../4 test_data(需要提交分词结果)20250414.txt",
                        help="Path to raw test file to segment")
    parser.add_argument("--output_path", type=str, 
                        default="cws_ner_fusion_result.txt",
                        help="Path to save fusion segmentation results")
    parser.add_argument("--dict_path", type=str, 
                        default="../../1 实验室1-中文分词源码/my_bert/data/dict_pku_only.txt",
                        help="Path to CWS dictionary")
    parser.add_argument("--use_dict", action="store_true", default=False,
                        help="Whether to use dictionary post-processing")
    parser.add_argument("--entity_types", nargs="+", default=["PER", "LOC", "ORG"],
                        help="NER entity types used to merge CWS boundaries")
    
    args = parser.parse_args()
    
    # Resolve absolute paths
    cws_model_path = os.path.abspath(os.path.join(my_dir, args.cws_model_path))
    ner_model_path = os.path.abspath(os.path.join(my_dir, args.ner_model_path))
    ner_tag_map = os.path.abspath(os.path.join(my_dir, args.ner_tag_map))
    input_path = os.path.abspath(os.path.join(my_dir, args.input_path))
    output_path = os.path.abspath(os.path.join(my_dir, args.output_path))
    dict_path = os.path.abspath(os.path.join(my_dir, args.dict_path))
    
    # 1. Validation
    if not os.path.exists(cws_model_path):
        print(f"Error: CWS model not found at {cws_model_path}")
        return
    if not os.path.exists(ner_model_path):
        print(f"Error: NER model not found at {ner_model_path}")
        return
    if not os.path.exists(ner_tag_map):
        print(f"Error: NER tag map not found at {ner_tag_map}")
        return
    if not os.path.exists(input_path):
        print(f"Error: Test file not found at {input_path}")
        return
        
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")
    
    # 2. Load CWS Dictionary if enabled
    dictionary_words = load_dictionary(dict_path) if args.use_dict else set()
    
    # 3. Load NER Tag Mapping
    with open(ner_tag_map, 'rb') as f:
        tag_mappings = pickle.load(f)
        ner_id2tag = tag_mappings['id2tag']
        
    # 4. Load Models
    print("Loading CWS model...")
    cws_model = load_cws_model(cws_model_path, device)
    cws_model.eval()
    
    print("Loading NER model...")
    ner_model = load_ner_model(ner_model_path, device)
    ner_model.eval()
    
    # 5. Load Tokenizer
    pretrained_model = 'hfl/chinese-roberta-wwm-ext'
    tokenizer = AutoTokenizer.from_pretrained(pretrained_model)
    
    # 6. Read input lines
    with open(input_path, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f]
        
    print(f"Loaded {len(lines)} sentences from {input_path}")
    print("Processing segmentation with CWS + NER Fusion...")
    
    fusion_outputs = []
    modified_count = 0
    total_entities_merged = 0
    
    for i, line in enumerate(tqdm(lines)):
        if not line:
            fusion_outputs.append("")
            continue
            
        # Character list
        chars = list(line)
        
        # Tokenize
        encoding = tokenizer(
            chars,
            is_split_into_words=True,
            max_length=512,
            padding=False,
            truncation=True,
            return_tensors='pt'
        )
        input_ids = encoding['input_ids'].to(device)
        attention_mask = encoding['attention_mask'].to(device)
        
        # Predict CWS
        with torch.no_grad():
            cws_pred = cws_model.infer(input_ids, attention_mask)[0]
        cws_words = cws_tags_to_words(chars, cws_pred)
        
        # Apply dictionary correction before NER boundary fusion
        if args.use_dict and dictionary_words:
            cws_words = dictionary_correct(cws_words, dictionary_words)
        
        # Predict NER
        with torch.no_grad():
            ner_pred = ner_model.infer(input_ids, attention_mask)[0]
        entities = extract_entities(chars, ner_pred, ner_id2tag)
        
        # Perform Fusion
        fused_words = perform_boundary_fusion(chars, cws_words, entities, set(args.entity_types))
        
        # Tracking metrics
        if len(fused_words) != len(cws_words):
            modified_count += 1
            total_entities_merged += (len(cws_words) - len(fused_words))
            
            # Print a few examples of fusion
            if modified_count <= 5:
                print(f"\nExample {modified_count} of Fusion:")
                print(f"  Raw Text: {line}")
                print(f"  Entities: {[(e[0], e[1]) for e in entities]}")
                print(f"  Original: {' / '.join(cws_words)}")
                print(f"  Fused   : {' / '.join(fused_words)}")
                
        fusion_outputs.append(" ".join(fused_words))
        
    # 6. Save results
    with open(output_path, 'w', encoding='utf-8') as f:
        for seg_line in fusion_outputs:
            f.write(seg_line + "\n")
            
    print(f"\n✅ Fusion Complete!")
    print(f"Segmented results saved to: {output_path}")
    print(f"Total sentences modified: {modified_count} / {len(lines)} ({modified_count/len(lines)*100:.2f}%)")
    print(f"Total boundary adjustments: merged {total_entities_merged} word fragments inside entities.")

if __name__ == "__main__":
    main()
