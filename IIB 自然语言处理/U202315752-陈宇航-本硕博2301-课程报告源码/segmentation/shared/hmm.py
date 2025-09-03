# coding=utf-8

import numpy as np
import os
# import utilities
import pickle
from . import smooths

class Hmm:
    """ 通用隐式马尔可夫模型.
    """
    def __init__(self):
        pass

    def setup(self, sp_mat, tp_mat, ep_mat, num_obsv, num_hide):
        """ 初始化马尔可夫模型,提供必要的参数

            Args:
              sp_mat: 初始概率矩阵
              tp_mat: 转移概率矩阵
              ep_mat: 发射概率矩阵
              num_obsv: 观察值种类数量
              num_hide: 隐藏值种类数量
        """
        self.sp_mat = sp_mat
        self.tp_mat = tp_mat
        self.ep_mat = ep_mat
        self.num_obsv = num_obsv
        self.num_hide = num_hide

    def find_hidden_state(self, obsv_seq):
        """ 给定观察序列,使用维特比算法寻找最可能的隐藏序列.
        """
        return self.__veterbi(obsv_seq)    
    
    def __veterbi(self, obsv_seq):
        # 初始化
        len_seq = len(obsv_seq)
        f = np.zeros([len_seq, self.num_hide])
        f_arg = np.zeros([len_seq, self.num_hide], dtype=int)
        # print(f.shape)
        for i in range(0, self.num_hide):
            f[0, i] = self.sp_mat[i] * self.ep_mat[obsv_seq[0], i]
            f_arg[0, i] = 0

        # 动态规划求解
        for i in range(1, len_seq):
            for j in range(self.num_hide):
                fs = [f[i-1, k] * self.tp_mat[j, k] * self.ep_mat[obsv_seq[i], j] 
                               for k in range(self.num_hide)]
                f[i, j] = max(fs)
                f_arg[i, j] = np.argmax(fs)
        
        # 反向求解最好的隐藏序列
        hidden_seq = [0] * len_seq
        z = np.argmax(f[len_seq-1, self.num_hide-1])
        hidden_seq[len_seq-1] = z
        for i in reversed(range(1, len_seq)):
            z = f_arg[i, z]
            hidden_seq[i-1] = z
        
        return hidden_seq

class HmmMatBuilder():
    """ 该类包含构建隐式马尔可夫模型所需矩阵的函数.
    """
    def __init__(self, corpus=None, num_obsv=None, num_hide=None):
        """ 初始化
            
            Args:
              corpus: 数据集,注意格式要求,不满足要求的话,
                      可以通过``index_corpus``函数进行编码.
              num_obsv: 观察状态数
              num_hide: 隐藏状态数

            数据集格式要求如下:
              一个列表,列表内容为一系列列表,每个列表包含观察序列及其对应的隐藏序列. 
              示例:[[(观察值,隐藏值),...],[...],...]
              观察值和隐藏值需要提前索引,这里将采用观测值和隐藏值作为矩阵的下标,
              因此其值必须为整型.
        """
        self.corpus = corpus
        self.num_obsv = num_obsv
        self.num_hide = num_hide
        if num_obsv != None and num_hide != None:
            self.sp_mat = np.zeros(self.num_hide)
            self.tp_mat = np.zeros([self.num_hide, self.num_hide])
            self.ep_mat = np.zeros([self.num_obsv, self.num_hide])
        else:
            self.sp_mat = None
            self.tp_mat = None
            self.ep_mat = None

    def build(self, smooth='add1'):
        """ 构建初始概率矩阵,转移概率矩阵以及发射概率矩阵.
        """
        if smooth not in ['add1', 'gt']:
            raise ValueError("Invalid value for smooth, only accept 'add1' and 'gt'.")
        for seq in self.corpus:
            for i in range(len(seq)):
                obsv_cur, hide_cur = seq[i]
                
                if(i == 0):
                    self.sp_mat[hide_cur] += 1
                else:
                    obsv_pre, hide_pre = seq[i-1]
                    self.tp_mat[hide_cur, hide_pre] += 1
                
                self.ep_mat[obsv_cur, hide_cur] += 1

        # 加1平滑
        if smooth == 'add1':
            self.sp_mat += 1
            self.tp_mat += 1
            self.ep_mat += 1

            self.sp_mat /= self.sp_mat.sum()
            self.tp_mat /= self.tp_mat.sum(axis=1)[:,None]
            self.ep_mat /= self.ep_mat.sum(axis=1)[:,None]
        else:
            self.sp_mat = smooths.simple_good_turing1d(self.sp_mat)
            self.tp_mat = smooths.simple_good_turing2d(self.tp_mat)
            self.ep_mat = smooths.simple_good_turing2d(self.ep_mat)

        # self.sp_mat *= 1e3
        # self.tp_mat *= 1e3
        # self.ep_mat *= 1e3

    def save(self, path):
        """ 保存隐式马尔可夫模型 """
        np.save(os.path.join(path, "start.npy"), self.sp_mat)
        np.save(os.path.join(path, "trans.npy"), self.tp_mat)
        np.save(os.path.join(path, "emit.npy"), self.ep_mat)
    
    def load(self, path):
        """ 加载隐式马尔可夫模型 """
        self.sp_mat = np.load(os.path.join(path, "start.npy"))
        self.tp_mat = np.load(os.path.join(path, "trans.npy"))
        self.ep_mat = np.load(os.path.join(path, "emit.npy"))
        self.num_obsv = self.ep_mat.shape[0]
        self.num_hide = self.ep_mat.shape[1]

def index_corpus(corpus):
    """ 将数据集进行编码

        Args:
          corpus: 格式必须为[[(obsv, hide), (obsv,hide),...], ...]

        Returns:
          idxed_corpus: 编码后的corpus,格式不变
          (obsv2idx, idx2obsv): 两个dict用于观察值与其编码之间的转换
          (hide2idx, idx2hide): 两个dict用于隐藏值与其编码之间的转换
    """
    obsv2idx, idx2obsv = {'unk': 0}, {0: 'unk'}
    hide2idx, idx2hide = {}, {}
    obsv_idx, hide_idx = 1, 0

    # build dictionaries and indexing
    idxed_corpus = []
    for seq in corpus:
        idxed_seq = []
        for obsv, hide in seq:
            if obsv not in obsv2idx.keys():
                obsv2idx[obsv] = obsv_idx
                idx2obsv[obsv_idx] = obsv
                obsv_idx += 1
            if hide not in hide2idx.keys():
                hide2idx[hide] = hide_idx
                idx2hide[hide_idx] = hide
                hide_idx += 1
            # indexing
            idxed_seq.append((obsv2idx[obsv], hide2idx[hide]))
        idxed_corpus.append(idxed_seq)
    
    return idxed_corpus, (obsv2idx, idx2obsv), (hide2idx, idx2hide)

def save_dicts(dicts, path):
    """ 保存四个dict至json文件 """
    obsv2idx, idx2obsv, hide2idx, idx2hide = dicts
    with open(os.path.join(path, "obsv2idx.p"), "wb") as f:
        pickle.dump(obsv2idx, f)
    with open(os.path.join(path, "idx2obsv.p"), "wb") as f:
        pickle.dump(idx2obsv, f)
    with open(os.path.join(path, "hide2idx.p"), "wb") as f:
        pickle.dump(hide2idx, f)
    with open(os.path.join(path, "idx2hide.p"), "wb") as f:
        pickle.dump(idx2hide, f)

def load_dicts(path):
    """ 读取四个dict """
    obsv2idx = None
    idx2obsv = None
    hide2idx = None
    idx2hide = None
    with open(os.path.join(path, "obsv2idx.p"), "rb") as f:
        obsv2idx = pickle.load(f)
    with open(os.path.join(path, "idx2obsv.p"), "rb") as f:
        idx2obsv = pickle.load(f)
    with open(os.path.join(path, "hide2idx.p"), "rb") as f:
        hide2idx = pickle.load(f)
    with open(os.path.join(path, "idx2hide.p"), "rb") as f:
        idx2hide = pickle.load(f)

    return obsv2idx, idx2obsv, hide2idx, idx2hide

class BaseHmmTagger:
    """ 基于隐式马尔可夫模型的序列标注基类 """
    def __init__(self):
        self.builder = None
        self.hmm = None

    def train(self, corpus, smooth='add1'):
        """ 根据数据集构建隐式马尔可夫模型.

            可能耗费较长时间.
        """
        idxed_corpus, (self.obsv2idx, self.idx2obsv), (self.hide2idx, self.idx2hide) = index_corpus(corpus)
        self.builder = HmmMatBuilder(idxed_corpus, 
                                         len(self.obsv2idx.keys()),
                                         len(self.hide2idx.keys()))
        self.builder.build(smooth)

        self.hmm = Hmm()
        self.hmm.setup(self.builder.sp_mat, self.builder.tp_mat, self.builder.ep_mat, 
                       self.builder.num_obsv, self.builder.num_hide)

    def load(self, path):
        """ 加载模型 """
        self.obsv2idx, self.idx2obsv, self.hide2idx, self.idx2hide = load_dicts(path)
        self.builder = HmmMatBuilder()
        self.builder.load(path)

        self.hmm = Hmm()
        self.hmm.setup(self.builder.sp_mat, self.builder.tp_mat, self.builder.ep_mat, 
                       self.builder.num_obsv, self.builder.num_hide)

    def save(self, path):
        """ 保存模型 """
        self.builder.save(path)
        dicts = (self.obsv2idx, self.idx2obsv, self.hide2idx, self.idx2hide)
        save_dicts(dicts, path)

if __name__ == '__main__':
    corpus_path = 'datasets/199801.txt'
    corpus = utilities.load_renmin(corpus_path)
    idxed_corpus, (obsv2idx, idx2obsv), (hide2idx, idx2hide) = index_corpus(corpus)
    dicts = (obsv2idx, idx2obsv, hide2idx, idx2hide)
    save_dicts(dicts, "hmm_para")

    builder = HmmMatBuilder(idxed_corpus, len(obsv2idx.keys()),len(hide2idx.keys()))
    builder.build()
    builder.save("hmm_para")

    obsv2idx, idx2obsv, hide2idx, idx2hide = load_dicts("hmm_para")
    builder = HmmMatBuilder()
    builder.load("hmm_para")

    hmm = Hmm()
    hmm.setup(builder.sp_mat, builder.tp_mat, builder.ep_mat, builder.num_obsv, builder.num_hide)

    seq = ['19980101-01-001-002','中共中央','总书记', '、', '国家', '主席', '江', '泽民']
    idxed_seq = [obsv2idx[word] for word in seq]

    idxed_pos = hmm.find_hidden_state(idxed_seq)
    pos = [idx2hide[idx] for idx in idxed_pos]
    
    print(idxed_seq)
    print(" ".join(seq))
    print(idxed_pos)
    print(" ".join(pos))

#    19980101-01-001-002/m  中共中央/nt  总书记/n  、/w  国家/n  主席/n  江/nr  泽民/nr  