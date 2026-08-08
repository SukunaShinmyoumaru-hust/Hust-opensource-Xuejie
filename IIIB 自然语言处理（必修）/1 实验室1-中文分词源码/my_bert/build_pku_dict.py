"""
从 PKU 训练数据中提取稳定词典（不引入 jieba）。

原先的做法是把训练集中所有长度 >= 2 的 gold 词都登录进词典，
但后处理会把“这 / 是”“提 / 出”等片段无条件合并成训练集中出现过
的词，导致大量 PKU 标准下应拆分的边界被破坏。

这里改为统计每个候选词在 PKU gold 中的边界稳定性：
  whole_count: 作为完整词出现的次数
  split_count: 作为多个相邻 gold 词拼接出现的次数
只保留高频且 split 冲突很少的词，作为保守纠错词典。
"""
import os


def should_keep(word, whole_count, split_count):
    n = len(word)
    total = whole_count + split_count
    stability = whole_count / total if total else 0.0

    # 两字词最容易产生“这是/提出/见到”类误合并，要求最严格。
    if n == 2:
        return whole_count >= 50 and split_count == 0

    # 三字词也较常见，允许极少数冲突，但必须高度稳定。
    if n == 3:
        return whole_count >= 10 and stability >= 0.995 and split_count <= 1

    # 长词更可能是专名、机构名或术语，但仍要求在 PKU 边界上稳定。
    return whole_count >= 3 and stability >= 0.98


def main():
    my_dir = os.path.dirname(os.path.abspath(__file__))
    train_path = os.path.join(my_dir, "data", "train_expanded.txt")
    output_path = os.path.join(my_dir, "data", "dict_pku_only.txt")

    if not os.path.exists(train_path):
        print(f"Error: {train_path} not found")
        return

    sentences = []
    whole_counts = {}
    with open(train_path, 'r', encoding='utf-8') as f:
        for line in f:
            words = line.strip().split()
            if not words:
                continue
            sentences.append(words)
            for w in words:
                if len(w) >= 2:
                    whole_counts[w] = whole_counts.get(w, 0) + 1

    candidates = set(whole_counts)
    max_len = max(map(len, candidates)) if candidates else 0
    split_counts = {w: 0 for w in candidates}

    for words in sentences:
        for i in range(len(words)):
            merged = words[i]
            for j in range(i + 1, len(words)):
                merged += words[j]
                if len(merged) > max_len:
                    break
                if merged in candidates:
                    split_counts[merged] += 1

    stable_words = {
        word: (whole_counts[word], split_counts[word])
        for word in candidates
        if should_keep(word, whole_counts[word], split_counts[word])
    }

    with open(output_path, 'w', encoding='utf-8') as f:
        for word in sorted(stable_words.keys()):
            whole, split = stable_words[word]
            f.write(f"{word} {whole} split={split}\n")

    print(f"PKU candidate words: {len(candidates)}")
    print(f"Stable PKU dictionary: {len(stable_words)} words -> {output_path}")

if __name__ == '__main__':
    main()
