import torch
import pickle
from tqdm import tqdm

if __name__ == '__main__':
    device = torch.device("cpu")
    model = torch.load('savekpk/model_epoch9.pkl',map_location=device,weights_only=False)
    output = open('ner_result.txt', 'w', encoding='utf-8')

    with open('data/ner_data_save.pkl', 'rb') as fnp:
        word2id = pickle.load(fnp)
        id2word = pickle.load(fnp)
        tag2id = pickle.load(fnp)
        id2tag = pickle.load(fnp)
        x_train = pickle.load(fnp)
        y_train = pickle.load(fnp)
    
    
    with open("data/NER_test.txt",'r',encoding='utf-8') as f:
        lines = f.readlines()
    
    for line in tqdm(lines):
        text = line.strip()
    
        x = [word2id.get(char,len(word2id)) for char in text]
        x = torch.LongTensor(x).unsqueeze(0).to(device)
        length = [len(text)]
        mask = torch.ones_like(x, dtype=torch.uint8)
        predict = model.infer(x, mask, length)[0]

        for i in range(len(text)):
            print(text[i], id2tag[predict[i]], file=output)
        output.write("\n")
