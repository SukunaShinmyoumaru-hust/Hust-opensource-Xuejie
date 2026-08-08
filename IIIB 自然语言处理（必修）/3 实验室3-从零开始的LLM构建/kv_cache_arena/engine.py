"""
KV Cache Arena 评估引擎 — 请勿修改此文件。

评估方式：在 WikiText-2 测试集的长文本段落上计算 Perplexity（困惑度）。
  - Perplexity 越低 = 模型对文本的预测越准 = 压缩损失越小
  - 逐 token 推进：喂入 token_i，对 token_{i+1} 打分，累加 NLL 后取指数

学生需要在 strategy.py 中实现 compress() 函数，在 cache 超过
MAX_BUDGET 时对其裁剪，目标是在低预算下最小化 Perplexity。
"""
import os
import math
import time
import torch
from typing import Callable, Optional
from tqdm import tqdm

_HERE     = os.path.dirname(os.path.abspath(__file__))
_MODEL_ID = 'gpt2'

_model     = None
_tokenizer = None
_device    = None

_CHUNK_LEN  = 400   # 每个评估 chunk 的固定 token 数
_N_DOCS     = 20    # 默认评估的 chunk 数（~2 min on CPU）


# ─── 模型懒加载 ──────────────────────────────────────────────────────────────

def _load_model():
    global _model, _tokenizer, _device
    if _model is not None:
        return
    os.environ.setdefault('HF_ENDPOINT', 'https://hf-mirror.com')
    from transformers import GPT2LMHeadModel, GPT2Tokenizer
    _device = 'cuda:0' if torch.cuda.is_available() else 'cpu'
    print(f'[engine] Loading {_MODEL_ID} on {_device} ...', flush=True)
    _tokenizer = GPT2Tokenizer.from_pretrained(_MODEL_ID)
    _tokenizer.pad_token = _tokenizer.eos_token
    _model = GPT2LMHeadModel.from_pretrained(_MODEL_ID)
    _model.to(_device).eval()
    for p in _model.parameters():
        p.requires_grad = False
    print(f'[engine] {_MODEL_ID} ready  '
          f'({sum(p.numel() for p in _model.parameters()):,} params, '
          f'device={_device})\n')


# ─── 数据集加载 ──────────────────────────────────────────────────────────────

def _load_docs(n_docs: int) -> list:
    """
    将 WikiText-2 测试集全文拼接后切成 _CHUNK_LEN 个 token 的 chunks。

    标准做法：避免因段落长短不一导致评估不公平；每个 chunk 长度固定，
    budget 相对 chunk 长度的比例完全一致，结果更稳定可对比。

    WikiText-2 test set 约 278k tokens → _CHUNK_LEN=400 可得约 690 个 chunk。
    """
    _load_model()
    print('[engine] Loading WikiText-2 test set ...', flush=True)
    try:
        import datasets as hf_datasets
        ds = hf_datasets.load_dataset('wikitext', 'wikitext-2-raw-v1', split='test')
        full_text = '\n'.join(row['text'] for row in ds if row['text'].strip())
    except Exception as e:
        print(f'[engine] Warning: failed to load WikiText-2 via datasets ({e}).')
        try:
            from huggingface_hub import hf_hub_download
            import pyarrow.parquet as pq
            parquet_path = hf_hub_download(
                repo_id='wikitext',
                repo_type='dataset',
                filename='wikitext-2-raw-v1/test-00000-of-00001.parquet',
            )
            table = pq.read_table(parquet_path)
            texts = table.column('text').to_pylist()
            full_text = '\n'.join(t for t in texts if t and t.strip())
            print('[engine] Loaded WikiText-2 test set from Hub parquet fallback.')
        except Exception as e2:
            print(f'[engine] Warning: failed to load WikiText-2 parquet ({e2}), using built-in corpus.')
            full_text = '\n'.join(_BUILTIN_CORPUS)

    all_ids = _tokenizer.encode(full_text)
    docs = [
        all_ids[i: i + _CHUNK_LEN]
        for i in range(0, len(all_ids) - _CHUNK_LEN, _CHUNK_LEN)
    ][:n_docs]

    if not docs:
        raise RuntimeError('Not enough tokens in dataset.')

    total_toks = sum(len(d) for d in docs)
    print(f'[engine] {len(docs)} chunks × {_CHUNK_LEN} tokens = {total_toks} tokens total\n')
    return docs


# ─── 内置语料（网络不可用时的后备） ──────────────────────────────────────────

_BUILTIN_CORPUS = [
    "The Python programming language was created by Guido van Rossum and first released in "
    "1991. Python emphasizes code readability and allows programmers to express concepts in "
    "fewer lines of code than languages such as C++ or Java. The language provides constructs "
    "that enable clear programming on both small and large scales. Python supports multiple "
    "programming paradigms, including structured, object-oriented, and functional programming. "
    "It has a comprehensive standard library and a large ecosystem of third-party packages. "
    "Python is widely used in web development, scientific computing, artificial intelligence, "
    "data analysis, and automation scripting. The language is dynamically typed and uses "
    "automatic memory management through garbage collection. Its clean syntax and extensive "
    "documentation make it an excellent choice for both beginners and experienced developers.",

    "The solar system consists of the Sun and the astronomical objects gravitationally bound "
    "to it. The Sun contains 99.86 percent of the system's known mass. The four inner planets, "
    "Mercury, Venus, Earth, and Mars, are terrestrial planets made primarily of rock and metal. "
    "The four outer planets, Jupiter, Saturn, Uranus, and Neptune, are gas giants or ice giants. "
    "The solar system formed 4.6 billion years ago from the gravitational collapse of a giant "
    "molecular cloud. The Sun is a main-sequence star at the center of the solar system. Beyond "
    "Neptune lies the Kuiper Belt, a region containing dwarf planets such as Pluto. Comets from "
    "the Oort Cloud occasionally visit the inner solar system, producing brilliant displays. "
    "The study of the solar system has been transformed by robotic spacecraft missions over the "
    "past six decades, revealing the diverse geology and atmospheres of planets and moons.",

    "Machine learning is a method of data analysis that automates analytical model building. "
    "It is based on the idea that systems can learn from data, identify patterns and make "
    "decisions with minimal human intervention. Machine learning algorithms are trained using "
    "large amounts of data, processing examples and adjusting parameters to improve accuracy. "
    "Deep learning is a subfield of machine learning that uses neural networks with many layers. "
    "These deep neural networks have achieved remarkable results in image recognition, natural "
    "language processing, and reinforcement learning. The transformer architecture, introduced "
    "in 2017, revolutionized natural language processing and led to the development of powerful "
    "language models. Today, large language models trained on vast corpora are capable of "
    "generating coherent text, answering complex questions, writing code, and solving reasoning "
    "problems across many domains of human knowledge and activity.",

    "The history of computing began in ancient times with mechanical calculation tools. "
    "The abacus was one of the earliest computing devices, used in Mesopotamia thousands of "
    "years ago. The modern digital computer emerged in the 1940s. Alan Turing proposed the "
    "concept of a universal computing machine in 1936, which became the theoretical foundation "
    "for modern computers. The first electronic computers, including ENIAC and UNIVAC, were "
    "enormous machines filling entire rooms. The invention of the transistor in 1947 and the "
    "integrated circuit in the 1950s enabled the miniaturization of computers. Intel released "
    "the first commercial microprocessor in 1971, paving the way for the personal computer "
    "revolution of the 1970s and 1980s. The internet connected these personal computers into "
    "a global network, fundamentally transforming communication, commerce, and daily life.",
] * 20   # repeat to provide enough documents if WikiText-2 is unavailable


# ─── KV cache 辅助 ───────────────────────────────────────────────────────────

def _cache_len(pkv) -> int:
    if hasattr(pkv, 'layers'):
        return pkv.layers[0].keys.shape[-2]
    return pkv[0][0].shape[2]


def _no_compress(pkv, max_budget, step):
    return pkv


# ─── 单文档 Perplexity 计算 ───────────────────────────────────────────────────

def _compute_ppl(token_ids: list, compress_fn: Callable, max_budget: Optional[int]) -> float:
    """
    逐 token 推进计算 Perplexity，每步在 cache 超过 max_budget 后调用 compress_fn。

    输入 token_i → 预测 token_{i+1}，累加 NLL，最后取指数。
    """
    ids = torch.tensor(token_ids, dtype=torch.long, device=_device).unsqueeze(0)
    n   = ids.shape[1]

    pkv         = None
    actual_pos  = 0
    nll_sum     = 0.0
    count       = 0

    for i in range(n - 1):
        token = ids[:, i:i+1]
        pos   = torch.tensor([[actual_pos]], device=_device, dtype=torch.long)

        with torch.no_grad():
            out = _model(
                input_ids           = token,
                past_key_values     = pkv,
                position_ids        = pos,
                use_cache           = True,
                output_attentions   = False,
                output_hidden_states= False,
            )

        actual_pos += 1
        pkv = out.past_key_values

        # 在 cache 超过预算时调用学生的压缩函数
        if max_budget is not None and _cache_len(pkv) > max_budget:
            pkv = compress_fn(pkv, max_budget, step=i)

        # 对 token_{i+1} 打分
        log_prob = torch.log_softmax(out.logits[0, -1, :], dim=-1)
        nll_sum += -log_prob[ids[0, i + 1]].item()
        count   += 1

    return math.exp(nll_sum / count) if count > 0 else float('nan')


# ─── 评估套件 ─────────────────────────────────────────────────────────────────

def _run_suite(docs, compress_fn, max_budget, label) -> float:
    ppls = []
    t0   = time.time()
    for token_ids in tqdm(docs, desc=label, ncols=72, leave=True):
        ppls.append(_compute_ppl(token_ids, compress_fn, max_budget))
    elapsed = time.time() - t0

    avg = sum(ppls) / len(ppls)
    std = (sum((p - avg) ** 2 for p in ppls) / len(ppls)) ** 0.5

    print(f'\n── {label} ──')
    print(f'  Perplexity : {avg:.2f}  ± {std:.2f}')
    print(f'  Time       : {elapsed:.1f}s')
    return avg


# ─── 公开 API ─────────────────────────────────────────────────────────────────

def evaluate(
    compress_fn:   Callable,
    max_budget:    int,
    strategy_name: str           = 'MyStrategy',
    n_docs:        Optional[int] = None,
    show_baseline: bool          = True,
):
    """
    运行完整评估。

    参数:
        compress_fn    : 学生实现的 compress(past_key_values, max_budget, step) -> pkv
        max_budget     : cache 最大保留 token 数（None = 不限）
        strategy_name  : 显示名称
        n_docs         : 评估文档数（None = 使用默认 60 篇）
        show_baseline  : 是否同时跑无压缩基线作为对比
    """
    _load_model()
    docs = _load_docs(n_docs or _N_DOCS)

    W = 62
    print('=' * W)
    print('  KV Cache Arena — Student Evaluation')
    print('=' * W)
    print(f'  Strategy : {strategy_name}')
    print(f'  Budget   : {max_budget} tokens  (chunk length = {_CHUNK_LEN} tokens, ratio={max_budget/_CHUNK_LEN:.0%})')
    print(f'  Model    : {_MODEL_ID}')
    print(f'  Docs     : {len(docs)}')
    print('=' * W + '\n')

    student_ppl = _run_suite(
        docs, compress_fn, max_budget,
        label=f'[Student] {strategy_name} (budget={max_budget})',
    )

    baseline_ppl = None
    if show_baseline:
        baseline_ppl = _run_suite(
            docs, _no_compress, None,
            label='[Baseline] No compression',
        )

    print('\n' + '=' * W)
    if baseline_ppl is not None:
        delta = student_ppl - baseline_ppl
        pct   = delta / baseline_ppl * 100
        sign  = '+' if delta >= 0 else ''
        ok    = '✅'
        print(f'  Student   PPL : {student_ppl:.2f}')
        print(f'  Baseline  PPL : {baseline_ppl:.2f}')
        print(f'  Degradation   : {sign}{pct:.1f}%  {ok}')
    else:
        print(f'  Perplexity : {student_ppl:.2f}')
    print('=' * W)

    return student_ppl
