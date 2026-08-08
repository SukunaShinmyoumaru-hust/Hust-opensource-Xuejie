import os
import torch
from torch.utils.data import Dataset, DataLoader
from transformers import AutoTokenizer
from sklearn.model_selection import train_test_split

class CWSDataset(Dataset):
    def __init__(self, sentences, labels, tokenizer, max_len=256):
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
        encoding = self.tokenizer(
            list(sentence),
            is_split_into_words=True,
            max_length=self.max_len,
            padding='max_length',
            truncation=True,
            return_tensors=None  # returns regular list/dict
        )

        word_ids = encoding.word_ids()
        
        # Align labels
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

def parse_pku_line(line):
    # Strip line
    line = line.strip()
    if not line:
        return None, None
    
    words = line.split()
    sentence_chars = []
    char_labels = []
    
    tag2id = {'B': 0, 'M': 1, 'E': 2, 'S': 3}
    
    for word in words:
        n = len(word)
        if n == 0:
            continue
        sentence_chars.extend(list(word))
        
        if n == 1:
            char_labels.append(tag2id['S'])
        elif n == 2:
            char_labels.extend([tag2id['B'], tag2id['E']])
        else:
            m_len = n - 2
            char_labels.append(tag2id['B'])
            char_labels.extend([tag2id['M']] * m_len)
            char_labels.append(tag2id['E'])
            
    return "".join(sentence_chars), char_labels

def load_cws_data(file_path):
    sentences = []
    labels = []
    
    print(f"Loading PKU data from {file_path}...")
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            sent, lbls = parse_pku_line(line)
            if sent and len(sent) == len(lbls):
                sentences.append(sent)
                labels.append(lbls)
                
    print(f"Loaded {len(sentences)} valid sentences.")
    return sentences, labels

def get_bert_dataloaders(file_path, model_name='hfl/chinese-roberta-wwm-ext', batch_size=8, max_len=256, test_size=0.1, random_state=43):
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    sentences, labels = load_cws_data(file_path)
    
    train_sents, val_sents, train_labels, val_labels = train_test_split(
        sentences, labels, test_size=test_size, random_state=random_state
    )
    
    print(f"Train sentences: {len(train_sents)}, Val sentences: {len(val_sents)}")
    
    train_dataset = CWSDataset(train_sents, train_labels, tokenizer, max_len)
    val_dataset = CWSDataset(val_sents, val_labels, tokenizer, max_len)
    
    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True, pin_memory=True)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False, pin_memory=True)
    
    return train_loader, val_loader

if __name__ == '__main__':
    # Test script
    my_dir = os.path.dirname(os.path.abspath(__file__))
    train_file = os.path.join(my_dir, "data", "train_expanded.txt")
    if os.path.exists(train_file):
        train_loader, val_loader = get_bert_dataloaders(train_file, batch_size=4, max_len=64)
        for input_ids, mask, label_ids in train_loader:
            print("input_ids shape:", input_ids.shape)
            print("mask shape:", mask.shape)
            print("label_ids shape:", label_ids.shape)
            print("First item input_ids:", input_ids[0])
            print("First item label_ids:", label_ids[0])
            break
