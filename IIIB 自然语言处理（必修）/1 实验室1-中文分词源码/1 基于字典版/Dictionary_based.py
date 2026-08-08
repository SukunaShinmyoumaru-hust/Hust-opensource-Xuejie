import os


class Tokenizer(object):
    def __init__(self, words, max_len):
        self.words = words
        self.max_len = max_len

    def fmm_split(self, text):
        '''
        正向最大匹配分词算法
        :param text: 待分词字符串
        :return: 分词结果，以list形式存放，每个元素为分出的词
        '''
        i, n = 0, len(text)
        result = []
        while i < n:
            matched = False
            upper = min(self.max_len, n - i)
            for length in range(upper, 0, -1):
                w = text[i:i + length]
                if w in self.words:
                    result.append(w)
                    i += length
                    matched = True
                    break
            if not matched:
                result.append(text[i])
                i += 1
        return result

    def rmm_split(self, text):
        '''
        逆向最大匹配分词算法
        :param text: 待分词字符串
        :return: 分词结果，以list形式存放，每个元素为分出的词
        '''
        i = len(text)
        result = []
        while i > 0:
            matched = False
            upper = min(self.max_len, i)
            for length in range(upper, 0, -1):
                w = text[i - length:i]
                if w in self.words:
                    result.append(w)
                    i -= length
                    matched = True
                    break
            if not matched:
                result.append(text[i - 1:i])
                i -= 1
        return list(reversed(result))

    def bimm_split(self, text):
        '''
        双向最大匹配分词算法
        :param text: 待分词字符串
        :return: 分词结果，以list形式存放，每个元素为分出的词
        '''
        fmm, rmm = self.fmm_split(text), self.rmm_split(text)
        if len(fmm) != len(rmm):
            return fmm if len(fmm) < len(rmm) else rmm

        def single_char_count(segments):
            return sum(1 for seg in segments if len(seg) == 1)

        sf, sr = single_char_count(fmm), single_char_count(rmm)
        if sf != sr:
            return fmm if sf < sr else rmm
        return rmm


def load_dict(path):
    tmp = set()
    with open(path, 'r', encoding='utf-8') as f:
        for line in f:
            word = line.strip().split(' ')[0]
            tmp.add(word)
    return tmp


if __name__ == '__main__':
    _dir = os.path.dirname(os.path.abspath(__file__))
    words = load_dict(os.path.join(_dir, 'dict.txt'))
    max_len = max(map(len, [word for word in words]))

    # test
    tokenizer = Tokenizer(words, max_len)
    texts = [
        '研究生命的起源',
        '无线电法国别研究',
        '人要是行，干一行行一行，一行行行行行，行行行干哪行都行。'
    ]
    for text in texts:
        # 前向最大匹配
        print('前向最大匹配:', '/'.join(tokenizer.fmm_split(text)))
        # 后向最大匹配
        print('后向最大匹配:', '/'.join(tokenizer.rmm_split(text)))
        # 双向最大匹配
        print('双向最大匹配:', '/'.join(tokenizer.bimm_split(text)))
        print('')
