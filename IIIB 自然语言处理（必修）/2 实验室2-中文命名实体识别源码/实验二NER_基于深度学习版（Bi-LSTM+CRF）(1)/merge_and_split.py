"""
合并 RMRB 和 MSRA NER 数据集
处理三个不一致：
1. 分隔符: MSRA 用下划线 B_PER → 转为连字符 B-PER
2. 标注方案: MSRA 是 BIO → 转为 BIOES
3. 实体类型: MSRA 只有 PER/LOC/ORG，RMRB 多了 TIME/MISC → 兼容，无需处理
"""
import os
import random

def load_sentences(file_path):
    """加载 NER 语料，返回 [[(字, 标签), ...], ...]"""
    sentences = []
    current = []
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line:
                if current:
                    sentences.append(current)
                    current = []
            else:
                parts = line.split()
                if len(parts) >= 2:
                    current.append((parts[0], parts[1]))
    if current:
        sentences.append(current)
    return sentences


def fix_msra_separator(tag):
    """MSRA 用下划线 B_PER → 统一为连字符 B-PER"""
    return tag.replace('_', '-')


def bio_to_bioes(sentences):
    """BIO → BIOES 转换"""
    converted = []
    for sentence in sentences:
        new_sent = []
        n = len(sentence)
        for i, (char, tag) in enumerate(sentence):
            if tag == 'O':
                new_sent.append((char, 'O'))
                continue

            prefix, etype = tag.split('-', 1)
            # 看下一个 token
            if i + 1 < n:
                next_tag = sentence[i + 1][1]
                next_prefix = next_tag.split('-')[0] if '-' in next_tag else next_tag
                next_etype = next_tag.split('-')[1] if '-' in next_tag else None
            else:
                next_prefix, next_etype = 'O', None

            if prefix == 'B':
                if next_prefix == 'I' and next_etype == etype:
                    new_sent.append((char, f'B-{etype}'))  # 后面还有 → B
                else:
                    new_sent.append((char, f'S-{etype}'))  # 后面没了 → S（单字实体）
            elif prefix == 'I':
                if next_prefix == 'I' and next_etype == etype:
                    new_sent.append((char, f'I-{etype}'))  # 后面还有 → I
                else:
                    new_sent.append((char, f'E-{etype}'))  # 后面没了 → E（实体结尾）
            else:
                new_sent.append((char, tag))  # 已经是 BIOES 的直接保留

        converted.append(new_sent)
    return converted


def save_sentences(sentences, file_path):
    """保存为 NER 格式"""
    with open(file_path, 'w', encoding='utf-8') as f:
        for sent in sentences:
            for char, tag in sent:
                f.write(f'{char} {tag}\n')
            f.write('\n')


def split_data(sentences, seed=43):
    """打乱并按 9:1 划分 (训练:验证, 不切测试集)"""
    random.seed(seed)
    random.shuffle(sentences)
    n = len(sentences)
    split_idx = int(n * 0.9)
    train = sentences[:split_idx]
    valid = sentences[split_idx:]
    return train, valid


if __name__ == '__main__':
    base_dir = os.path.dirname(os.path.abspath(__file__))

    # ======== 1. 加载 RMRB（已经是 BIOES 格式）========
    rmrb_path = os.path.join(base_dir, 'data', 'data', 'RMRB_NER_CORPUS.txt')
    print(f"加载 RMRB: {rmrb_path}")
    rmrb = load_sentences(rmrb_path)
    print(f"  RMRB 句子数: {len(rmrb)}")

    # ======== 2. 加载 MSRA（BIO 格式，下划线分隔） ========
    msra_dir = os.path.join(base_dir, 'data', 'external_datasets', 'msra')
    msra_all = []
    for fname in ['train.bio.txt', 'dev.bio.txt', 'test.bio.txt']:
        fpath = os.path.join(msra_dir, fname)
        if os.path.exists(fpath):
            sents = load_sentences(fpath)
            print(f"加载 MSRA {fname}: {len(sents)} 句")
            msra_all.extend(sents)
    print(f"  MSRA 总句子数: {len(msra_all)}")

    # ======== 3. 转换 MSRA 标签 ========
    # 3a. 下划线 → 连字符
    msra_fixed = [[(c, fix_msra_separator(t)) for c, t in sent] for sent in msra_all]
    # 3b. BIO → BIOES
    msra_bioes = bio_to_bioes(msra_fixed)

    # 验证转换后的标签
    all_tags = set()
    for sent in msra_bioes:
        for _, tag in sent:
            all_tags.add(tag)
    print(f"  MSRA 转换后标签: {sorted(all_tags)}")

    # ======== 4. 合并 ========
    merged = rmrb + msra_bioes
    print(f"\n合并后总句子数: {len(merged)} (RMRB {len(rmrb)} + MSRA {len(msra_bioes)})")

    # ======== 5. 划分 9:1 (训练:验证, 不切测试集) ========
    train, valid = split_data(merged)
    print(f"划分结果: 训练 {len(train)}, 验证 {len(valid)}")

    # ======== 6. 保存到基线项目 data/ ========
    baseline_data = os.path.join(base_dir, 'data')
    save_sentences(train, os.path.join(baseline_data, 'ner_train.txt'))
    save_sentences(valid, os.path.join(baseline_data, 'ner_valid.txt'))
    print(f"\n已保存到: {baseline_data}/ner_*.txt")

    # ======== 7. 复制到 my_bert_ner/data/ ========
    bert_data = os.path.join(base_dir, '..', 'my_bert_ner', 'data')
    os.makedirs(bert_data, exist_ok=True)
    save_sentences(train, os.path.join(bert_data, 'ner_train.txt'))
    save_sentences(valid, os.path.join(bert_data, 'ner_valid.txt'))
    print(f"已保存到: {bert_data}/ner_*.txt")

    print("\n✅ 完成！两个目录下都已生成训练/验证数据。")
