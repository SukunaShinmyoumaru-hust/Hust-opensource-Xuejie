import codecs
from sklearn.model_selection import train_test_split
import pickle

INPUT_DATA = "train.txt"
TRAIN_DATA = "ner_train.txt"
VALID_DATA = "ner_valid.txt"
SAVE_PATH = "./ner_datasave.pkl"

# create id2tag
unique = set()
with open('ner_train.txt', 'r')as f:
    for line in f:
        try:
            unique.update([line.strip('\n').split(' ')[1]])
        except:
            pass
id2tag = list(unique)
print(id2tag)
tag2id = {}
for i, label in enumerate(id2tag):
    tag2id[label] = i

# id2tag = ['B', 'M', 'E', 'S']  # B：分词头部 M：分词词中 E：分词词尾 S：独立成词
# tag2id = {'B': 0, 'M': 1, 'E': 2, 'S': 3}
word2id = {}
id2word = []


def getList(input_str):
    '''
    单个分词转换为tag序列
    :param input_str: 单个分词
    :return: tag序列
    '''
    outpout_str = []
    if len(input_str) == 1:
        outpout_str.append(tag2id['S'])
    elif len(input_str) == 2:
        outpout_str = [tag2id['B'], tag2id['E']]
    else:
        M_num = len(input_str) - 2
        M_list = [tag2id['M']] * M_num
        outpout_str.append(tag2id['B'])
        outpout_str.extend(M_list)
        outpout_str.append(tag2id['E'])
    return outpout_str


def handle_data():
    '''
    处理数据，并保存至savepath
    :return:
    '''
    outp = open(SAVE_PATH, 'wb')

    x_train = []
    y_train = []
    x_valid = []
    y_valid = []

    wordnum = 0
    with open(TRAIN_DATA, 'r', encoding="utf-8") as ifp:
        line_x = []
        line_y = []
        for line in ifp:
            line = line.strip()
            if not line:
                x_train.append(line_x)
                y_train.append(line_y)
                line_x = []
                line_y = []
                continue
            line = line.split(' ')
            if line[0] in id2word:
                line_x.append(word2id[line[0]])
            else:
                id2word.append(line[0])
                word2id[line[0]] = wordnum
                line_x.append(wordnum)
                wordnum = wordnum + 1
            line_y.append(tag2id[line[1]])
            
    with open(VALID_DATA, 'r', encoding="utf-8") as ifp:
        line_x = []
        line_y = []
        for line in ifp:
            line = line.strip()
            if not line:
                x_valid.append(line_x)
                y_valid.append(line_y)
                line_x = []
                line_y = []
                continue
            line = line.split(' ')
            if line[0] in id2word:
                line_x.append(word2id[line[0]])
            else:
                id2word.append(line[0])
                word2id[line[0]] = wordnum
                line_x.append(wordnum)
                wordnum = wordnum + 1
            line_y.append(tag2id[line[1]])
    

    print(x_train[0])
    print([id2word[i] for i in x_train[0]])
    print(y_train[0])
    print([id2tag[i] for i in y_train[0]])
    # x_train, x_test, y_train, y_test = train_test_split(x_data, y_data, test_size=0.1, random_state=43)
    
    pickle.dump(word2id, outp)
    pickle.dump(id2word, outp)
    pickle.dump(tag2id, outp)
    pickle.dump(id2tag, outp)
    pickle.dump(x_train, outp)
    pickle.dump(y_train, outp)
    pickle.dump(x_valid, outp)
    pickle.dump(y_valid, outp)

    outp.close()



if __name__ == "__main__":
    handle_data()
