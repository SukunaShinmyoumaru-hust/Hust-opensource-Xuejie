# coding=utf-8
import argparse

def precision(pred, std):
    """ 求分词结果的正确率

        Args:
          pred: 分词结果,一个元组,其内容为各个句子的分词结果(仍为元组).
          std: 标准分词结果,结构与pred相同.
        
        Returns:
          返回正确率,取值[0,1].
    """
    correct = 0
    total_num = 0

    for p_s, std_s in zip(pred, std):
        for word in p_s:
            if(word in std_s):
                correct += 1
        total_num += len(p_s)
    
    return float(correct) / total_num

def recall(pred, std):
    """ 求分词结果的召回率

        Args:
          pred: 分词结果,一个元组,其内容为各个句子的分词结果(仍为元组).
          std: 标准分词结果,结构与pred相同.
        
        Returns:
          返回召回率,取值[0,1].
    """
    correct = 0
    total_num = 0

    for p_s, std_s in zip(pred, std):
        for word in p_s:
            if(word in std_s):
                correct += 1
        total_num += len(std_s)
    
    return float(correct) / total_num

def f1_score(pred, std):
    """ 求分词结果的f1值

        Args:
          pred: 分词结果,一个元组,其内容为各个句子的分词结果(仍为元组).
          std: 标准分词结果,结构与pred相同.
        
        Returns:
          返回f1值,取值[0,1].
    """
    prec = precision(pred, std)
    rec = recall(pred, std)

    return 2.0 * prec * rec / (prec + rec)
 
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--pred_path', default='test_data/pku/unigram_seg.txt')
    parser.add_argument('--gold_path', default='test_data/pku/gold.txt')
    parser.add_argument('--pred_encoding', default='gb18030')
    parser.add_argument('--gold_encoding', default='utf-8')
    args = parser.parse_args()
    
    pred = []
    with open(args.pred_path, "r", encoding=args.pred_encoding) as f:
        lines = f.readlines()
        for sent in lines:
            pred.append(sent.strip().split())

    std = []
    with open(args.gold_path, "r", encoding=args.gold_encoding) as f:
        for sent in f.readlines():
            std.append(sent.strip().split())

    print("Precision: {0}.".format(precision(pred, std)))
    print("Recall: {0}.".format(recall(pred, std)))
    print("F1 Score: {0}.".format(f1_score(pred, std)))