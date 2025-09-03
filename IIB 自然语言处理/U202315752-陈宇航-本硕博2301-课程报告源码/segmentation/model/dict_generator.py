# coding=utf-8

import json
import argparse
import time
def json_read(file_path, encoding):
    """ 读取json文件解析为dict返回.
    """
    with open(file_path, 'r', encoding=encoding) as f:
        return json.load(f)

def dict_save(mdict, save_path="dict.json"):
    """ 将词典保存至json文件中

        Args:
          mdict: 待保存词典
          save_path: 词典将保存在该路径下
    """
    with open(save_path, 'w', encoding="utf-16") as f:
        json.dump(mdict, f)
        print("Dict is saved in -- {0}.".format(save_path))
    
def generate_dict(corpus_path, encoding='utf-16', verbose=True):
    """ 根据数据集生成词典.

        Args:
          corpus_path: 数据集路径,数据集必须采用空白分割词语.
          verbose: 是否打印进度.

        Returns:
          返回一个词典, 格式为{词: 词出现次数}.
    """
    corpus_dict = {}
    with open(corpus_path, "r", encoding=encoding) as f:
        print("Start processing.")
        count = 0
        for line in f.readlines():
            for word in line.strip().split():
                if(word in corpus_dict):
                    corpus_dict[word] += 1
                else:
                    corpus_dict[word] = 1
                    count += 1

                if(count % 10000 == 0):
                    print("Found {0} words.".format(count))
        corpus_dict['_t_'] = count
        print("Finished. Total number of words: {0}".format(count))
        
    return corpus_dict

def load_sogou_dict(file_path, encoding='gb18030'):
    """ 读取搜狗词典 """
    d = {}
    d['_t_'] = 0
    with open(file_path, 'r', encoding=encoding) as f:
        for line in f.readlines():
            word, freq = line.strip().split('\t')[:2]
            d['_t_'] += int(freq) + 1
            d[word] = int(freq) + 1
    return d

if __name__ == '__main__':
    print('首先使用dict_generator.py，将需要使用的数据集转换为json格式的字典。这里采用的是PKU数据集，你可以自己搜寻数据集放在datasets目录下。')
    parser = argparse.ArgumentParser()
    parser.add_argument('--corpus_path', default='datasets/pku/pku_training_words.utf8')
    parser.add_argument('--dict_path', default='dicts/pku_dict.json')
    parser.add_argument('--encoding', default='utf-8')
    args = parser.parse_args()
    time_now = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    print("当前时间: ", time_now)
    corpus_dict = generate_dict(args.corpus_path, encoding=args.encoding)
    dict_save(corpus_dict, save_path=args.dict_path)
    time_after = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    print("当前时间: ", time_after)
    print("词典生成完毕，耗时：", time.mktime(time.strptime(time_after, "%Y-%m-%d %H:%M:%S")) - time.mktime(time.strptime(time_now, "%Y-%m-%d %H:%M:%S")))
    print("下面展示分词结果前50项")
    count = 0
    for pair in corpus_dict.items():
        print(pair)
        count += 1 
        if count == 50:
            break
            