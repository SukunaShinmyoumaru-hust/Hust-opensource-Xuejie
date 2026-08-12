import os
import torch
from torch.utils.data import Dataset, DataLoader
from transformers import AutoTokenizer

class NERDataset(Dataset):
    """NER数据集，用于BERT+CRF命名实体识别模型"""
    def __init__(self, sentences, labels, tokenizer, max_len=256):
        """
        Args:
            sentences: 句子列表，每个句子是字符串（由字符组成）
            labels: 标签列表，每个元素是该句子中每个字符的标签ID列表
            tokenizer: BERT分词器
            max_len: 最大序列长度
        """
        self.sentences = sentences
        self.labels = labels
        self.tokenizer = tokenizer
        self.max_len = max_len

    def __len__(self):
        return len(self.sentences)

    def __getitem__(self, idx):
        sentence = self.sentences[idx]
        char_labels = self.labels[idx]

        # Tokenize with is_split_into_words=True
        # 将每个汉字作为独立token输入BERT分词器
        encoding = self.tokenizer(
            list(sentence),
            is_split_into_words=True,
            max_length=self.max_len,
            padding='max_length',
            truncation=True,
            return_tensors=None  # returns regular list/dict
        )

        word_ids = encoding.word_ids()
        
        # Align labels: 对齐标签，处理CLS/SEP/PAD等特殊token
        # CLS和SEP位置标记为-100（CRF中会通过mask排除）
        label_ids = []
        for word_idx in word_ids:
            if word_idx is None:
                label_ids.append(-100)  # ignored by CRF/loss
            else:
                label_ids.append(char_labels[word_idx])

        # Convert to tensors
        input_ids = torch.tensor(encoding['input_ids'], dtype=torch.long)
        attention_mask = torch.tensor(encoding['attention_mask'], dtype=torch.long)
        label_ids = torch.tensor(label_ids, dtype=torch.long)

        return input_ids, attention_mask, label_ids


def load_ner_data(file_path):
    """
    加载NER格式数据文件
    数据格式: 每行 "字 标签"，空行分隔句子
    标签集: O, B-PER, I-PER, E-PER, S-PER, B-LOC, I-LOC, E-LOC, S-LOC,
           B-ORG, I-ORG, E-ORG, S-ORG, B-TIME, I-TIME, E-TIME, S-TIME,
           B-MISC, I-MISC, E-MISC, S-MISC (共21个标签)
    
    Args:
        file_path: NER数据文件路径
        
    Returns:
        sentences: 句子列表 (每个句子是字符串)
        labels: 标签列表 (每个元素是该句子中每个字符的标签字符串列表)
    """
    sentences = []
    labels = []
    
    print(f"Loading NER data from {file_path}...")
    with open(file_path, 'r', encoding='utf-8') as f:
        current_chars = []
        current_tags = []
        for line in f:
            line = line.strip()
            if not line:
                # 空行表示句子结束
                if current_chars:
                    sentences.append(''.join(current_chars))
                    labels.append(current_tags)
                    current_chars = []
                    current_tags = []
                continue
            
            parts = line.split(' ')
            if len(parts) >= 2:
                char = parts[0]
                tag = parts[1]
                current_chars.append(char)
                current_tags.append(tag)
        
        # 处理最后一个句子（文件末尾可能没有空行）
        if current_chars:
            sentences.append(''.join(current_chars))
            labels.append(current_tags)
    
    print(f"Loaded {len(sentences)} valid sentences.")
    return sentences, labels


def build_tag_mappings(label_lists):
    """
    从标签数据中构建 tag2id 和 id2tag 映射
    
    Args:
        label_lists: 标签列表的列表（每个句子一个标签列表）
        
    Returns:
        tag2id: 标签到ID的映射字典
        id2tag: ID到标签的映射列表
    """
    unique_tags = set()
    for tags in label_lists:
        unique_tags.update(tags)
    
    # 按固定顺序排列标签，确保每次运行结果一致
    # 先放O标签，再按实体类型和BIES前缀排序
    sorted_tags = sorted(unique_tags)
    # 把O标签放在第一个位置
    if 'O' in sorted_tags:
        sorted_tags.remove('O')
        sorted_tags = ['O'] + sorted_tags
    
    tag2id = {tag: idx for idx, tag in enumerate(sorted_tags)}
    id2tag = sorted_tags
    
    print(f"Built tag mappings: {len(id2tag)} tags")
    print(f"Tags: {id2tag}")
    return tag2id, id2tag


def convert_tags_to_ids(label_lists, tag2id):
    """
    将字符串标签列表转换为标签ID列表
    
    Args:
        label_lists: 标签字符串列表的列表
        tag2id: 标签到ID的映射字典
        
    Returns:
        label_id_lists: 标签ID列表的列表
    """
    label_id_lists = []
    for tags in label_lists:
        label_id_lists.append([tag2id[t] for t in tags])
    return label_id_lists


def get_bert_ner_dataloaders(train_file, valid_file, model_name='hfl/chinese-roberta-wwm-ext', 
                              batch_size=4, max_len=256):
    """
    加载NER训练集和验证集，返回DataLoader
    
    Args:
        train_file: 训练数据文件路径
        valid_file: 验证数据文件路径
        model_name: 预训练模型名称
        batch_size: 批次大小
        max_len: 最大序列长度
        
    Returns:
        train_loader: 训练数据DataLoader
        val_loader: 验证数据DataLoader
        tag2id: 标签到ID映射
        id2tag: ID到标签映射
    """
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    
    # 加载训练集和验证集
    train_sents, train_labels = load_ner_data(train_file)
    val_sents, val_labels = load_ner_data(valid_file)
    
    # 从训练集构建标签映射（训练集应包含所有标签）
    tag2id, id2tag = build_tag_mappings(train_labels)
    
    # 将字符串标签转换为ID
    train_label_ids = convert_tags_to_ids(train_labels, tag2id)
    val_label_ids = convert_tags_to_ids(val_labels, tag2id)
    
    print(f"Train sentences: {len(train_sents)}, Val sentences: {len(val_sents)}")
    
    train_dataset = NERDataset(train_sents, train_label_ids, tokenizer, max_len)
    val_dataset = NERDataset(val_sents, val_label_ids, tokenizer, max_len)
    
    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True, pin_memory=True)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False, pin_memory=True)
    
    return train_loader, val_loader, tag2id, id2tag


if __name__ == '__main__':
    # Test script: 测试数据加载是否正常
    my_dir = os.path.dirname(os.path.abspath(__file__))
    train_file = os.path.join(my_dir, "data", "ner_train.txt")
    valid_file = os.path.join(my_dir, "data", "ner_valid.txt")
    
    if os.path.exists(train_file) and os.path.exists(valid_file):
        train_loader, val_loader, tag2id, id2tag = get_bert_ner_dataloaders(
            train_file, valid_file, batch_size=4, max_len=64
        )
        print(f"\ntag2id: {tag2id}")
        print(f"id2tag: {id2tag}")
        print(f"Number of tags: {len(id2tag)}")
        
        for input_ids, mask, label_ids in train_loader:
            print("\ninput_ids shape:", input_ids.shape)
            print("mask shape:", mask.shape)
            print("label_ids shape:", label_ids.shape)
            print("First item input_ids:", input_ids[0])
            print("First item label_ids:", label_ids[0])
            break
    else:
        print(f"Data files not found. Expected:\n  {train_file}\n  {valid_file}")
