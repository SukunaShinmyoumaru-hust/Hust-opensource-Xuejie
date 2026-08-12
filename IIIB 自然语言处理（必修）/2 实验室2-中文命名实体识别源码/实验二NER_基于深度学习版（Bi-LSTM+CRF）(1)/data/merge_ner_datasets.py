"""
NER 数据集合并脚本
用于将外部 NER 数据集（如 MSRA NER、Weibo NER 等）与人民日报语料合并，
统一标签体系并生成训练数据。

使用方法：
1. 下载外部数据集，放入 data/ 目录
2. 修改下方 EXTERNAL_DATASETS 配置
3. 运行: python merge_ner_datasets.py
4. 重新运行 0.split.py 和 1.data_u_ner.py

支持的外部数据集格式：
- BIO/BIOES 格式：每行一个 "字 标签"，空行分隔句子
- MSRA 格式：同上

标签映射规则（统一到人民日报的 5 类体系）：
- PER, B-PER, I-PER, ... → PER（人名）
- LOC, B-LOC, I-LOC, ... → LOC（地名）
- ORG, B-ORG, I-ORG, ... → ORG（机构名）
- TIME, B-TIME, I-TIME, ... → TIME（时间）
- GPE → LOC（地缘政治实体 → 地名）
- TITLE, EDU, PRO, RACE, CONT → MISC（其他）
- 其余未知类型 → MISC
"""

import os
import random

# ========== 配置区 ==========

# 人民日报原始语料路径
RMRB_CORPUS = 'data/RMRB_NER_CORPUS.txt'

# 外部数据集配置（根据实际下载的数据集修改）
# 每个条目包含：文件路径、源标签格式（'bio' 或 'bioes'）
EXTERNAL_DATASETS = [
    # 示例：MSRA NER 数据集（下载后取消注释）
    # {'path': 'data/msra_train.txt', 'format': 'bio'},

    # 示例：Weibo NER 数据集
    # {'path': 'data/weibo_train.txt', 'format': 'bio'},
]

# 合并后的输出文件
OUTPUT_FILE = 'data/MERGED_NER_CORPUS.txt'

# ========== 标签映射表 ==========

# 将各种外部数据集的实体类别统一映射到人民日报的 5 类体系
ENTITY_TYPE_MAP = {
    # 人名
    'PER': 'PER',
    'NR': 'PER',
    'PERSON': 'PER',

    # 地名
    'LOC': 'LOC',
    'NS': 'LOC',
    'LOCATION': 'LOC',
    'GPE': 'LOC',      # 地缘政治实体 → 归入地名

    # 机构名
    'ORG': 'ORG',
    'NT': 'ORG',
    'ORGANIZATION': 'ORG',

    # 时间
    'TIME': 'TIME',
    'DATE': 'TIME',

    # 其他（不在以上类别的统一归入 MISC）
    'MISC': 'MISC',
    'TITLE': 'MISC',
    'EDU': 'MISC',
    'PRO': 'MISC',
    'RACE': 'MISC',
    'CONT': 'MISC',
}


def map_tag(tag):
    """
    将外部数据集的标签映射到人民日报的标签体系
    例如：B-GPE → B-LOC, I-NR → I-PER, O → O
    """
    if tag == 'O':
        return 'O'

    if '-' in tag:
        prefix, etype = tag.split('-', 1)
        mapped_type = ENTITY_TYPE_MAP.get(etype, 'MISC')
        return f'{prefix}-{mapped_type}'
    else:
        # 某些数据集可能使用不带前缀的标签
        return tag


def bio_to_bioes(sentences):
    """
    将 BIO 格式转换为 BIOES 格式
    BIO: B-XXX, I-XXX, O
    BIOES: B-XXX, I-XXX, E-XXX, S-XXX, O
    """
    converted = []
    for sentence in sentences:
        new_sentence = []
        for i, (char, tag) in enumerate(sentence):
            if tag == 'O':
                new_sentence.append((char, 'O'))
                continue

            prefix, etype = tag.split('-', 1)

            # 判断下一个标签
            if i + 1 < len(sentence):
                next_tag = sentence[i + 1][1]
                next_prefix = next_tag.split('-')[0] if '-' in next_tag else next_tag
                next_etype = next_tag.split('-')[1] if '-' in next_tag else None
            else:
                next_prefix = 'O'
                next_etype = None

            if prefix == 'B':
                # B 后面紧跟 I 同类型 → 保持 B
                # B 后面不是 I 同类型 → 改为 S（单字实体）
                if next_prefix == 'I' and next_etype == etype:
                    new_sentence.append((char, f'B-{etype}'))
                else:
                    new_sentence.append((char, f'S-{etype}'))

            elif prefix == 'I':
                # I 后面仍是 I 同类型 → 保持 I
                # I 后面不是 I 同类型 → 改为 E（实体结尾）
                if next_prefix == 'I' and next_etype == etype:
                    new_sentence.append((char, f'I-{etype}'))
                else:
                    new_sentence.append((char, f'E-{etype}'))
            else:
                # 已经是 BIOES 格式中的 E 或 S，直接保留
                new_sentence.append((char, tag))

        converted.append(new_sentence)
    return converted


def load_corpus(file_path):
    """
    加载 NER 语料文件（每行 "字 标签"，空行分隔句子）
    返回：[[(字1, 标签1), (字2, 标签2), ...], ...]
    """
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
                    char, tag = parts[0], parts[1]
                    current.append((char, tag))
    if current:
        sentences.append(current)
    return sentences


def save_corpus(sentences, file_path):
    """
    保存 NER 语料文件
    """
    with open(file_path, 'w', encoding='utf-8') as f:
        for sentence in sentences:
            for char, tag in sentence:
                f.write(f'{char} {tag}\n')
            f.write('\n')


def main():
    print("=" * 50)
    print("NER 数据集合并工具")
    print("=" * 50)

    # 1. 加载人民日报原始语料
    print(f"\n[1] 加载人民日报语料: {RMRB_CORPUS}")
    rmrb_sentences = load_corpus(RMRB_CORPUS)
    print(f"    句子数: {len(rmrb_sentences)}")

    all_sentences = list(rmrb_sentences)

    # 2. 加载并合并外部数据集
    if not EXTERNAL_DATASETS:
        print(f"\n[2] 未配置外部数据集，仅使用人民日报语料")
        print(f"    提示：下载外部数据集后，修改脚本中的 EXTERNAL_DATASETS 配置")
        print(f"    推荐数据集：")
        print(f"      - MSRA NER: GitHub 搜索 'MSRA NER dataset'")
        print(f"      - Weibo NER: GitHub 搜索 'Weibo NER dataset'")
        print(f"      - Resume NER: GitHub 搜索 'Resume NER dataset'")
    else:
        for i, ds_config in enumerate(EXTERNAL_DATASETS):
            ds_path = ds_config['path']
            ds_format = ds_config.get('format', 'bioes')
            print(f"\n[2.{i+1}] 加载外部数据集: {ds_path}")

            if not os.path.exists(ds_path):
                print(f"    ⚠️ 文件不存在，跳过: {ds_path}")
                continue

            ext_sentences = load_corpus(ds_path)
            print(f"    原始句子数: {len(ext_sentences)}")

            # 标签映射
            mapped_sentences = []
            for sent in ext_sentences:
                mapped_sent = [(char, map_tag(tag)) for char, tag in sent]
                mapped_sentences.append(mapped_sent)

            # BIO → BIOES 转换（如果需要）
            if ds_format == 'bio':
                print(f"    执行 BIO → BIOES 转换...")
                mapped_sentences = bio_to_bioes(mapped_sentences)

            all_sentences.extend(mapped_sentences)
            print(f"    合并后总句子数: {len(all_sentences)}")

    # 3. 保存合并后的语料
    print(f"\n[3] 保存合并后的语料: {OUTPUT_FILE}")
    save_corpus(all_sentences, OUTPUT_FILE)
    print(f"    总句子数: {len(all_sentences)}")

    # 4. 统计实体分布
    print(f"\n[4] 实体类别分布统计:")
    entity_counts = {}
    for sent in all_sentences:
        for char, tag in sent:
            if tag != 'O':
                etype = tag.split('-')[1] if '-' in tag else tag
                entity_counts[etype] = entity_counts.get(etype, 0) + 1
    for etype, count in sorted(entity_counts.items(), key=lambda x: -x[1]):
        print(f"    {etype}: {count} 个标签")

    print(f"\n✅ 合并完成！")
    print(f"   下一步操作：")
    print(f"   1. 将 0.split.py 中的语料路径改为 '{OUTPUT_FILE}'")
    print(f"   2. 重新运行 python 0.split.py")
    print(f"   3. 重新运行 python 1.data_u_ner.py")


if __name__ == '__main__':
    main()
