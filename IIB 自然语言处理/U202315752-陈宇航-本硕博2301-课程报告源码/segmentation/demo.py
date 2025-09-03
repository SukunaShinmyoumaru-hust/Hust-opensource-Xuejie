
from model.mm_seg import MMSeg
from model.unigram_seg import UniGramSeg
from model.hmm_seg import HmmSeg

import os

from segmentation.model import dict_generator


def get_mmseg():
    seg = MMSeg()
    mdict = dict_generator.json_read("model/dicts/pku_dict.json", encoding='utf-16')
    seg.set_dict(mdict)
    return seg

def get_uniseg():
    seg = UniGramSeg()
    mdict = dict_generator.json_read("model/dicts/pku_dict.json", encoding='utf-16')
    seg.set_dict(mdict)
    return seg

def get_hmmseg():
    script_dir = os.path.dirname(__file__)
    seg = HmmSeg()
    model_path = os.path.join(script_dir, "model/hmm_para")
    seg.load(model_path)
    return seg

if __name__ == "__main__":
    mmseg = get_mmseg()
    uniseg = get_uniseg()
    hmmseg = get_hmmseg()

    s = ""
    print("这是一个分词Demo,输入exit退出.")
    while s != 'exit':
        s = input("请输入待分词句子: ")
        print("最大匹配法：")
        words = mmseg.cut(s)
        print("/".join(words))

        print("Unigram：")
        words = uniseg.cut(s)
        print("/".join(words))
        
        print("HMM：")
        words = hmmseg.cut(s)
        print("/".join(words))