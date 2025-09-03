#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
三种分词器 (MM、UniGram、HMM) 可选版，中文输出
"""

import timeit, os, argparse
import model.dict_generator as dg
from model.mm_seg     import MMSeg
from model.unigram_seg import UniGramSeg
from model.hmm_seg     import HmmSeg

# ---------- 构造分词器 ----------
def build_mm_seg(dataset: str):
    seg = MMSeg()
    if dataset != 'pku':
        raise ValueError('[dataset] 参数不合法，目前仅支持 pku')
    seg.set_dict(dg.json_read('model/dicts/pku_dict.json', encoding='utf-16'))
    return seg

def build_unigram_seg(dataset: str):
    seg = UniGramSeg()
    if dataset != 'pku':
        raise ValueError('[dataset] 参数不合法，目前仅支持 pku')
    seg.set_dict(dg.json_read('model/dicts/pku_dict.json', encoding='utf-16'))
    return seg

def build_hmm_seg(dataset: str):
    seg = HmmSeg()
    if dataset != 'pku':
        raise ValueError('[dataset] 参数不合法，目前仅支持 pku')
    seg.load(os.path.join(os.path.dirname(__file__), 'model/hmm_para'))
    return seg

# 工厂映射表
SEGGER_FACTORY = {
    'mm'     : build_mm_seg,
    'unigram': build_unigram_seg,
    'hmm'    : build_hmm_seg,
}

# ---------- 主程序 ----------
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--segger',  default='mm',  choices=SEGGER_FACTORY.keys(),
                        help='分词算法 (mm | unigram | hmm)')
    parser.add_argument('--dataset', default='pku', choices=['pku'],
                        help='数据集，目前仅 pku')
    args = parser.parse_args()

    # 1) 创建分词器
    seg = SEGGER_FACTORY[args.segger](args.dataset)
    print(f'已选择分词算法：{args.segger}，数据集：{args.dataset}')

    # 2) 执行分词 + 计时
    test_path = os.path.join('test_data', args.dataset, 'test.txt')
    encoding  = 'utf-8' if args.dataset == 'pku' else 'utf-16'

    with open(test_path, 'r', encoding=encoding) as f:
        lines = f.readlines()

    pred, ch_count = [], 0
    total = len(lines)
    start_t = timeit.default_timer()

    for idx, sent in enumerate(lines, 1):
        try:
            pred.append(seg.cut(sent))
        except Exception:
            pred.append(['Error'] * len(sent))
        ch_count += len(sent)

        # 进度提示（每 500 行）
        if idx % 500 == 0:
            print(f'已处理 {idx}/{total} 行 ({idx/total:.2%})')

    end_t = timeit.default_timer()
    time_cost = end_t - start_t

    # 3) 统计信息
    print(f'字符总数：{ch_count}')
    print(f'耗时：{time_cost:.2f} 秒')
    print(f'分词速度：{int(ch_count / time_cost)} 字符/秒')

    # 4) 保存结果
    out_path = os.path.join('test_data', args.dataset, f'{args.segger}_seg.txt')
    with open(out_path, 'w', encoding='gb18030') as f:
        for words in pred:
            f.write(' '.join(words).strip() + '\n')

    print(f'分词结果已保存至：{out_path}')

if __name__ == '__main__':
    main()
