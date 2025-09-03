import torch
import pickle

if __name__ == '__main__':
    model = torch.load('savekpk/model_epoch9.pkl', map_location = torch.device('cpu' ),weights_only=False)
    output = open('cws_result.txt', 'w', encoding='utf-8')

    with open('data/datasave.pkl', 'rb') as inp:
        word2id = pickle.load(inp)
        id2word = pickle.load(inp)
        tag2id = pickle.load(inp)
        id2tag = pickle.load(inp)
        x_train = pickle.load(inp)
        y_train = pickle.load(inp)
        x_test = pickle.load(inp)
        y_test = pickle.load(inp)

    with open('data/test.txt', 'r', encoding='utf-8') as f:
        for test in f:
            flag = False
            test = test.strip()

            x = torch.LongTensor(1, len(test))
            mask = torch.ones_like(x, dtype=torch.uint8)
            length = [len(test)]
            for i in range(len(test)):
                if test[i] in word2id:
                    x[0, i] = word2id[test[i]]
                else:
                    x[0, i] = len(word2id)

            predict = model.infer(x, mask, length)[0]
            for i in range(len(test)):
                print(test[i], end='', file=output)
                if id2tag[predict[i]] in ['E', 'S']:
                    print(' ', end='', file=output)
            print(file=output)
