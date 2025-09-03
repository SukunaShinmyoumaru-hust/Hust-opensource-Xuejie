# coding=utf-8
import os
import sys
sys.path.append('..')
from shared import hmm
# import hmm

class HmmSeg(hmm.BaseHmmTagger):
    """ 采用隐式马尔可夫模型的中文分词器
    """
    def cut(self, sentence):
        """ 对给定句子进行分词.

            Args:
              sentence: 待分词的句子.

            Returns:
              分好的词语序列.
        """
        if self.hmm == None:
            raise Exception("You have to train or load model before tagging.")

        sentence = sentence.strip()
        
        tags = []
        idxed_seq = [self.obsv2idx[obsv] if obsv in self.obsv2idx.keys() else 0 for obsv in sentence]
        idxed_tag = self.hmm.find_hidden_state(idxed_seq)
        tags = [self.idx2hide[idx] for idx in idxed_tag]

        assert len(sentence) == len(tags), "标记与句子长度不一致"

        # 由tag还原回词
        words = []
        lo, hi = 0, 0
        for i in range(len(tags)):
            if tags[i] == 'B':
                lo = i
            elif tags[i] == 'E':
                hi = i+1
                words.append(sentence[lo:hi])
            elif tags[i] == 'S':
                words.append(sentence[i:i+1])

        if tags[-1] == 'B':
            words.append(sentence[-1])  # 处理 SB,EB
        elif tags[-1] == 'M':
            words.append(sentence[lo:-1])
        
        assert len(sentence) == len("".join(words)), "还原失败,长度不一致\n{0}\n{1}\n{2}".format(sentence,"".join(words), "".join(tags))
        
        return words 

def process_corpus(path, encoding):
    """ 将原始分词数据集处理成BMES标记的数据集. 

        返回格式[[('你','B'),..],[..],..]
    """
    corpus = []
    with open(path, 'r', encoding=encoding) as f:
        for line in f:
            l = []
            for word in line.strip().split():
                if len(word) == 1:
                    l.append((word[0], 'S'))
                    continue
                for i in range(len(word)):
                    if i == 0:
                        l.append((word[i], 'B'))
                    elif i == len(word)-1:
                        l.append((word[i], 'E'))
                    else:
                        l.append((word[i], 'M'))
            corpus.append(l)
    return corpus

if __name__ == '__main__':
    script_dir = os.path.dirname(__file__)
    corpus_path = os.path.join(script_dir, 'datasets/shanxi/test.txt')
    corpus = process_corpus(corpus_path, encoding='utf-8')

    seg = HmmSeg()
    seg.train(corpus)
    model_path = os.path.join(script_dir, "sx_hmm_para")
    seg.save(model_path)
    # seg.load(model_path)
    s = "专访老瓦：可能参加北京奥运中国领先不是悲剧２００７年０６月１８日１４：５２大江网."
    words = seg.cut(s)
    print("/".join(words))
    