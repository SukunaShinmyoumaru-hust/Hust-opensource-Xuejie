"""
本地评估脚本：在内部验证集（datasave.pkl 的 x_test/y_test）上计算词级别 F1-score
用法：python eval.py
"""
import pickle
import sys
import types
import torch
import torch.nn as nn

try:
    import torchcrf  # noqa: F401
except ModuleNotFoundError:
    torchcrf_shim = types.ModuleType('torchcrf')

    class CRF(nn.Module):
        def __init__(self, num_tags=None, batch_first=True):
            super().__init__()
            self.batch_first = batch_first

        def decode(self, emissions, mask=None):
            if not getattr(self, 'batch_first', True):
                emissions = emissions.transpose(0, 1)
                if mask is not None:
                    mask = mask.transpose(0, 1)
            if mask is None:
                mask = emissions.new_ones(emissions.shape[:2], dtype=torch.bool)
            mask = mask.bool()

            transitions = self.transitions
            start = self.start_transitions
            end = self.end_transitions
            paths = []
            for b in range(emissions.size(0)):
                length = int(mask[b].sum().item())
                emit = emissions[b, :length]
                score = start + emit[0]
                history = []
                for t in range(1, length):
                    next_score = score.unsqueeze(1) + transitions + emit[t].unsqueeze(0)
                    best_score, best_tag = next_score.max(dim=0)
                    history.append(best_tag)
                    score = best_score
                score = score + end
                best_last = int(score.argmax().item())
                path = [best_last]
                for hist in reversed(history):
                    best_last = int(hist[best_last].item())
                    path.append(best_last)
                paths.append(list(reversed(path)))
            return paths

    torchcrf_shim.CRF = CRF
    sys.modules['torchcrf'] = torchcrf_shim


def tags_to_words(chars, tags, id2tag):
    """把字符序列 + BMES标签序列 转换为词列表（用字符位置区间表示）"""
    words = []
    buf = ''
    for i, (ch, tag) in enumerate(zip(chars, tags)):
        buf += ch
        if id2tag[tag] in ['E', 'S']:
            words.append(buf)
            buf = ''
    if buf:
        words.append(buf)
    return words


def compute_f1(pred_list, gold_list):
    """
    词级别 F1-score
    pred_list, gold_list: 每句话的词列表 list of list
    """
    tp = fp = fn = 0
    for pred, gold in zip(pred_list, gold_list):
        pred_set = set()
        gold_set = set()
        # 用 (start, end) 位置对来判断是否匹配
        pos = 0
        for w in pred:
            pred_set.add((pos, pos + len(w)))
            pos += len(w)
        pos = 0
        for w in gold:
            gold_set.add((pos, pos + len(w)))
            pos += len(w)

        tp += len(pred_set & gold_set)
        fp += len(pred_set - gold_set)
        fn += len(gold_set - pred_set)

    precision = tp / (tp + fp) if (tp + fp) > 0 else 0
    recall    = tp / (tp + fn) if (tp + fn) > 0 else 0
    f1        = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
    return precision, recall, f1


if __name__ == '__main__':
    # 1. 加载数据
    with open('data/datasave.pkl', 'rb') as inp:
        word2id = pickle.load(inp)
        id2word = pickle.load(inp)
        tag2id  = pickle.load(inp)
        id2tag  = pickle.load(inp)
        x_train = pickle.load(inp)
        y_train = pickle.load(inp)
        x_test  = pickle.load(inp)
        y_test  = pickle.load(inp)

    # 2. 找最新的模型（或指定epoch）
    import glob, os
    models = sorted(glob.glob('save/model_epoch*.pkl'))
    if not models:
        print("没有找到模型，请先运行 python run.py 训练")
        exit(1)
    model_path = models[-1]
    print(f"加载模型：{model_path}")

    model = torch.load(model_path, map_location=torch.device('cpu'), weights_only=False)
    model.eval()

    # 3. 对验证集推断
    pred_words_list = []
    gold_words_list = []

    with torch.no_grad():
        for x_seq, y_seq in zip(x_test, y_test):
            chars = [id2word[i] for i in x_seq]
            n = len(x_seq)

            x    = torch.LongTensor([x_seq])
            mask = torch.ones(1, n, dtype=torch.bool)
            pred = model.infer(x, mask, [n])[0]

            pred_words_list.append(tags_to_words(chars, pred,  id2tag))
            gold_words_list.append(tags_to_words(chars, y_seq, id2tag))

    # 4. 计算 F1
    p, r, f1 = compute_f1(pred_words_list, gold_words_list)
    print(f"\n{'='*40}")
    print(f"验证集样本数: {len(x_test)}")
    print(f"Precision : {p:.4f}")
    print(f"Recall    : {r:.4f}")
    print(f"F1-score  : {f1:.4f}  ← 本地参考分")
    print(f"{'='*40}")
    print("注意：服务器用 data/test.txt 评分，此分为内部验证集估计值")
