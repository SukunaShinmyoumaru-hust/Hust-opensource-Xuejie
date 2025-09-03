import torch
import pickle
from torch.utils.data import Dataset, DataLoader
from torch.nn.utils.rnn import pad_sequence


class Sentence(Dataset):
    def __init__(self, x, y, batch_size=10):
        self.x = x
        self.y = y
        self.batch_size = batch_size

    def __len__(self):
        return len(self.x)

    def __getitem__(self, idx):
        assert len(self.x[idx]) == len(self.y[idx])
        return self.x[idx], self.y[idx]

    @staticmethod
    def collate_fn(train_data):
        train_data.sort(key=lambda data: len(data[0]), reverse=True)
        data_length = [len(data[0]) for data in train_data]
        data_x = [torch.LongTensor(data[0]) for data in train_data]
        data_y = [torch.LongTensor(data[1]) for data in train_data]
        data_mask = [torch.ones(i, dtype=torch.uint8) for i in data_length]
        data_x = pad_sequence(data_x, batch_first=True, padding_value=0)
        data_y = pad_sequence(data_y, batch_first=True, padding_value=0)
        data_mask = pad_sequence(data_mask, batch_first=True, padding_value=0)
        return data_x, data_y, data_mask, data_length


if __name__ == '__main__':
    # test
    with open('../data/ner_data_save.pkl', 'rb') as inp:
        word2id = pickle.load(inp)
        id2word = pickle.load(inp)
        tag2id = pickle.load(inp)
        id2tag = pickle.load(inp)
        x_train = pickle.load(inp)
        y_train = pickle.load(inp)
        x_test = pickle.load(inp)
        y_test = pickle.load(inp)

    train_dataloader = DataLoader(Sentence(x_train, y_train), batch_size=10, shuffle=True,
                                  collate_fn=Sentence.collate_fn)

    for in_put, label, mask, length in train_dataloader:
        print(in_put, label)
        break
