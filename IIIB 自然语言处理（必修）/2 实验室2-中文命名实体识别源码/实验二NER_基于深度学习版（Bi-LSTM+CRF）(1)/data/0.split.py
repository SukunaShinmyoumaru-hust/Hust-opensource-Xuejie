corpus_file = 'data/RMRB_NER_CORPUS.txt'
corpus = []
with open(corpus_file, 'r')as f:
    record = []
    for line in f:
        if line != '\n':
            record.append(line.strip('\n').split(' '))
        else:
            corpus.append(record)
            record = []

# print(len(corpus)) # 44011

import random
random.seed(43)
random.shuffle(corpus)

fulllen = len(corpus)
splitlen = len(corpus)//10

train = corpus[:splitlen*8]
valid = corpus[splitlen*8:splitlen*9]
test = corpus[splitlen*9:]

train_file = 'ner_train.txt'
valid_file = 'ner_valid.txt'
test_file = 'ner_test.txt'

for split_file, split_corpus in zip([train_file, valid_file, test_file],
                                    [train, valid, test]):
    with open(split_file, 'w')as f:
        for sentence in split_corpus:
            for word, label in sentence:
                f.write(word)
                f.write(' ')
                f.write(label)
                f.write('\n')
            f.write('\n')

