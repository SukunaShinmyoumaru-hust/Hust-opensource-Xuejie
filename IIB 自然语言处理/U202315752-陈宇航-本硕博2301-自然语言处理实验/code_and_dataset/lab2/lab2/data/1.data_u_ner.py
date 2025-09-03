import pickle


TRAIN_DATA = "NER_train.txt"

SAVE_PATH = "ner_data_save.pkl"

unique = set()
with open('ner_train.txt', 'r',encoding='utf-8')as f:
    for line in f:
        try:
            items = line.strip().split(' ')
            if len(items) >= 2:
                unique.update([items[1]])
        except Exception as e:
            print("Failed to process line:", line)
            print("Error message:", str(e))
            continue
id2tag = list(unique)
print(id2tag)
tag2id = {}
for i, label in enumerate(id2tag):
    tag2id[label] = i

word2id = {}
id2word = []


def getlist(input_str):
    """
    单个分词转换为tag序列
    :param input_str: 单个分词
    :return: tag序列
    """
    output_str = []
    if len(input_str) == 1:
        output_str.append(tag2id['S'])
    elif len(input_str) == 2:
        output_str = [tag2id['B'], tag2id['E']]
    else:
        m_num = len(input_str) - 2
        m_list = [tag2id['M']] * m_num
        output_str.append(tag2id['B'])
        output_str.extend(m_list)
        output_str.append(tag2id['E'])
    return output_str


def handle_data():
    """
    处理数据，并保存至save_path
    :return:
    """
    output = open(SAVE_PATH, 'wb')
    x_train = []
    y_train = []


    word_num = 0
    with open(TRAIN_DATA, 'r', encoding="utf-8") as ifp:
        file_set(x_train, y_train, ifp, word_num)



    print(x_train[0])
    print([id2word[temp] for temp in x_train[0]])
    print(y_train[0])
    print([id2tag[temp] for temp in y_train[0]])


    pickle.dump(word2id, output)
    pickle.dump(id2word, output)
    pickle.dump(tag2id, output)
    pickle.dump(id2tag, output)
    pickle.dump(x_train, output)
    pickle.dump(y_train, output)



    output.close()


def file_set(x, y, ifp, word_num):
    line_x = []
    line_y = []
    for file_line in ifp:
        file_line = file_line.strip()
        if not file_line:
            x.append(line_x)
            y.append(line_y)
            line_x = []
            line_y = []
            continue
        file_line = file_line.split(' ')
        if file_line[0] in id2word:
            line_x.append(word2id[file_line[0]])
        else:
            id2word.append(file_line[0])
            word2id[file_line[0]] = word_num
            line_x.append(word_num)
            word_num = word_num + 1
        line_y.append(tag2id[file_line[1]])


if __name__ == "__main__":
    handle_data()
