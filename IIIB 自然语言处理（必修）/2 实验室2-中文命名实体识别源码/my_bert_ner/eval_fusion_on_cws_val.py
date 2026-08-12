import os
import pickle
import sys
import time

import torch
from sklearn.model_selection import train_test_split
from tqdm import tqdm
from transformers import AutoTokenizer

my_dir = os.path.dirname(os.path.abspath(__file__))
cws_dir = os.path.abspath(os.path.join(my_dir, "..", "..", "1 实验室1-中文分词源码", "my_bert"))
sys.path.append(my_dir)
sys.path.append(cws_dir)

from cws_ner_fusion import extract_entities, load_ner_model, perform_boundary_fusion
from dataloader_bert import load_cws_data
from infer_bert import load_cws_model, tags_to_words


MODEL_NAME = "hfl/chinese-roberta-wwm-ext"
CHUNK_SIZE = 510
ALLOWED_ENTITY_TYPES = {"PER", "LOC", "ORG"}


def word_spans(words):
    spans = set()
    pos = 0
    for word in words:
        spans.add((pos, pos + len(word)))
        pos += len(word)
    return spans


def compute_f1(predictions, golds):
    tp = fp = fn = 0
    for pred, gold in zip(predictions, golds):
        pred_spans = word_spans(pred)
        gold_spans = word_spans(gold)
        tp += len(pred_spans & gold_spans)
        fp += len(pred_spans - gold_spans)
        fn += len(gold_spans - pred_spans)

    precision = tp / (tp + fp) if tp + fp else 0.0
    recall = tp / (tp + fn) if tp + fn else 0.0
    f1 = 2 * precision * recall / (precision + recall) if precision + recall else 0.0
    return precision, recall, f1, tp, fp, fn


def predict_cws(sentence, tokenizer, cws_model, device):
    words = []
    for start in range(0, len(sentence), CHUNK_SIZE):
        chunk = sentence[start:start + CHUNK_SIZE]
        encoding = tokenizer(
            list(chunk),
            is_split_into_words=True,
            max_length=512,
            padding=False,
            truncation=True,
            return_tensors="pt",
        )
        input_ids = encoding["input_ids"].to(device)
        attention_mask = encoding["attention_mask"].to(device)
        pred_tags = cws_model.infer(input_ids, attention_mask)[0]
        words.extend(tags_to_words(list(chunk), pred_tags))
    return words


def predict_fusion(sentence, tokenizer, cws_model, ner_model, ner_id2tag, device):
    words = []
    for start in range(0, len(sentence), CHUNK_SIZE):
        chunk = sentence[start:start + CHUNK_SIZE]
        chars = list(chunk)
        encoding = tokenizer(
            chars,
            is_split_into_words=True,
            max_length=512,
            padding=False,
            truncation=True,
            return_tensors="pt",
        )
        input_ids = encoding["input_ids"].to(device)
        attention_mask = encoding["attention_mask"].to(device)

        cws_tags = cws_model.infer(input_ids, attention_mask)[0]
        cws_words = tags_to_words(chars, cws_tags)

        ner_tags = ner_model.infer(input_ids, attention_mask)[0]
        entities = extract_entities(chars, ner_tags, ner_id2tag)
        words.extend(perform_boundary_fusion(chars, cws_words, entities, ALLOWED_ENTITY_TYPES))
    return words


def main():
    start_time = time.time()
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")

    cws_model_path = os.path.join(cws_dir, "save", "best_model.pkl")
    ner_model_path = os.path.join(my_dir, "save", "best_ner_model.pkl")
    ner_tag_map_path = os.path.join(my_dir, "save", "ner_tag_mappings.pkl")
    train_file = os.path.join(cws_dir, "data", "train_expanded.txt")

    tokenizer = AutoTokenizer.from_pretrained(MODEL_NAME)

    print("Loading CWS model...")
    cws_model = load_cws_model(cws_model_path, device)
    cws_model.eval()

    print("Loading NER model...")
    ner_model = load_ner_model(ner_model_path, device)
    ner_model.eval()

    with open(ner_tag_map_path, "rb") as f:
        ner_id2tag = pickle.load(f)["id2tag"]

    print("Loading CWS validation split...")
    sentences, labels = load_cws_data(train_file)
    _, val_sentences, _, val_labels = train_test_split(
        sentences, labels, test_size=0.1, random_state=43
    )

    print(f"Validation sentences: {len(val_sentences)}")
    print(f"Max sentence length: {max(len(s) for s in val_sentences)}")
    print(f"Sentences over {CHUNK_SIZE} chars: {sum(len(s) > CHUNK_SIZE for s in val_sentences)}")
    print(f"Allowed NER entity types: {sorted(ALLOWED_ENTITY_TYPES)}")

    gold_words_list = []
    cws_words_list = []
    fusion_words_list = []
    examples = []
    changed_count = 0

    with torch.no_grad():
        for sentence, gold_tags in tqdm(
            list(zip(val_sentences, val_labels)), desc="Evaluating fusion"
        ):
            chars = list(sentence)
            gold_words = tags_to_words(chars, gold_tags)
            cws_words = predict_cws(sentence, tokenizer, cws_model, device)
            fusion_words = predict_fusion(
                sentence, tokenizer, cws_model, ner_model, ner_id2tag, device
            )

            gold_words_list.append(gold_words)
            cws_words_list.append(cws_words)
            fusion_words_list.append(fusion_words)

            if fusion_words != cws_words:
                changed_count += 1
                if len(examples) < 8:
                    examples.append((sentence, cws_words, fusion_words, gold_words))

    cws_metrics = compute_f1(cws_words_list, gold_words_list)
    fusion_metrics = compute_f1(fusion_words_list, gold_words_list)

    print("\nCWS only: P={:.6f} R={:.6f} F1={:.6f} TP={} FP={} FN={}".format(*cws_metrics))
    print("CWS+NER : P={:.6f} R={:.6f} F1={:.6f} TP={} FP={} FN={}".format(*fusion_metrics))
    print("Delta F1: {:+.6f}".format(fusion_metrics[2] - cws_metrics[2]))
    print(
        "Changed sentences: {} / {} ({:.2f}%)".format(
            changed_count, len(val_sentences), changed_count / len(val_sentences) * 100
        )
    )
    print(f"Elapsed seconds: {time.time() - start_time:.1f}")

    if examples:
        print("\nExamples:")
        for idx, (sentence, cws_words, fusion_words, gold_words) in enumerate(examples, 1):
            print(f"\n#{idx} {sentence}")
            print("CWS :", " / ".join(cws_words))
            print("FUS :", " / ".join(fusion_words))
            print("GOLD:", " / ".join(gold_words))


if __name__ == "__main__":
    main()
