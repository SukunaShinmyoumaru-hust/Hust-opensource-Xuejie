import os
import time
import argparse

import torch
from sklearn.model_selection import train_test_split
from tqdm import tqdm
from transformers import AutoTokenizer

from dataloader_bert import load_cws_data
from infer_bert import dictionary_correct, load_cws_model, load_dictionary, tags_to_words


MODEL_NAME = "hfl/chinese-roberta-wwm-ext"
CHUNK_SIZE = 510


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


def predict_cws(sentence, tokenizer, model, device):
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
        pred_tags = model.infer(input_ids, attention_mask)[0]
        words.extend(tags_to_words(list(chunk), pred_tags))
    return words


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--dict_path", default=os.path.join("data", "dict_pku_only.txt"))
    parser.add_argument("--label", default="dict")
    args = parser.parse_args()

    start_time = time.time()
    my_dir = os.path.dirname(os.path.abspath(__file__))
    train_file = os.path.join(my_dir, "data", "train_expanded.txt")
    model_path = os.path.join(my_dir, "save", "best_model.pkl")
    dict_path = args.dict_path
    if not os.path.isabs(dict_path):
        dict_path = os.path.join(my_dir, dict_path)

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")

    tokenizer = AutoTokenizer.from_pretrained(MODEL_NAME)
    model = load_cws_model(model_path, device)
    model.eval()
    dictionary_words = load_dictionary(dict_path)

    sentences, labels = load_cws_data(train_file)
    _, val_sentences, _, val_labels = train_test_split(
        sentences, labels, test_size=0.1, random_state=43
    )

    gold_words_list = []
    cws_words_list = []
    dict_words_list = []
    changed_count = 0
    examples = []

    with torch.no_grad():
        for sentence, gold_tags in tqdm(
            list(zip(val_sentences, val_labels)), desc=f"Evaluating {args.label}"
        ):
            gold_words = tags_to_words(list(sentence), gold_tags)
            cws_words = predict_cws(sentence, tokenizer, model, device)
            corrected_words = dictionary_correct(cws_words, dictionary_words)

            gold_words_list.append(gold_words)
            cws_words_list.append(cws_words)
            dict_words_list.append(corrected_words)

            if corrected_words != cws_words:
                changed_count += 1
                if len(examples) < 8:
                    examples.append((sentence, cws_words, corrected_words, gold_words))

    cws_metrics = compute_f1(cws_words_list, gold_words_list)
    dict_metrics = compute_f1(dict_words_list, gold_words_list)

    print("\nCWS only: P={:.6f} R={:.6f} F1={:.6f} TP={} FP={} FN={}".format(*cws_metrics))
    print("CWS+{}: P={:.6f} R={:.6f} F1={:.6f} TP={} FP={} FN={}".format(args.label, *dict_metrics))
    print("Delta F1: {:+.6f}".format(dict_metrics[2] - cws_metrics[2]))
    print(
        "Changed sentences: {} / {} ({:.2f}%)".format(
            changed_count, len(val_sentences), changed_count / len(val_sentences) * 100
        )
    )
    print(f"Elapsed seconds: {time.time() - start_time:.1f}")

    if examples:
        print("\nExamples:")
        for idx, (sentence, cws_words, corrected_words, gold_words) in enumerate(examples, 1):
            print(f"\n#{idx} {sentence}")
            print("CWS :", " / ".join(cws_words))
            print("DICT:", " / ".join(corrected_words))
            print("GOLD:", " / ".join(gold_words))


if __name__ == "__main__":
    main()
