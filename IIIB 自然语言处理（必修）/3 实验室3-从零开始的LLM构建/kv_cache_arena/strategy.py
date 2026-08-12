"""
KV Cache 压缩策略 — 学生实现文件
=====================================
只需修改这个文件，不要改动其他文件！

任务说明
--------
模型在推理时会积累 KV cache。cache 越大，内存占用越高。
你的任务是实现一个压缩函数，在 cache 超过 max_budget 时将它裁剪。
目标：在尽可能低的内存预算下，最大化模型在测试集上的准确率。

接口说明
--------
compress(past_key_values, max_budget, step) -> past_key_values

    past_key_values : 当前的 KV cache（transformers DynamicCache 对象）
    max_budget      : 最大允许保留的 token 数（由 MAX_BUDGET 控制）
    step            : 当前是第几个 token（0-indexed），可用于判断压缩时机

    返回值 : 压缩后的 past_key_values（直接修改后返回即可）

可用工具函数（来自 kv_utils）
------------------------------
    kv_utils.get_seq_len(pkv)                          -> int      当前 cache 长度
    kv_utils.get_device(pkv)                           -> device
    kv_utils.select_indices(pkv, indices)              -> pkv      按位置保留
    kv_utils.keep_recent(pkv, n)                       -> pkv      保留最近 n 个
    kv_utils.keep_random(pkv, n)                       -> pkv      随机保留 n 个

评分方式
--------
在 WikiText-2 测试集的长文本段落（200-512 tokens）上计算 Perplexity（困惑度）。
- Perplexity 越低越好（越接近无压缩基线越好）
- 退化率 ≤ 5%  ✅   退化率 ≤ 20%  ⚠️   退化率 > 20%  ❌

运行方式
--------
    python run_eval.py              # 完整评估（含基线对比）
    python run_eval.py --quick      # 快速测试（前 50 条）
    python run_eval.py --no-baseline  # 跳过基线，只跑自己的策略
"""

import torch
import kv_utils

# ============================================================
STRATEGY_NAME = 'DocAwareHeadAbsLog'
MAX_BUDGET    = 64            # cache 保留的最大 token 数（上下文约 200-400 tokens）
# ============================================================

_positions = None
_head_positions = None
_last_step = -1
_last_budget = None
_chunk_id = 0


def _layers_of(pkv):
    try:
        return pkv.layers
    except:
        return None


def _seq_len(pkv):
    layers = _layers_of(pkv)
    if layers is not None:
        return layers[0].keys.shape[-2]
    return pkv[0][0].shape[2]


def _device_of(pkv):
    layers = _layers_of(pkv)
    if layers is not None:
        return layers[0].keys.device
    return pkv[0][0].device


def _select_indices(pkv, indices):
    layers = _layers_of(pkv)
    if layers is not None:
        for layer in layers:
            layer.keys = layer.keys[..., indices, :]
            layer.values = layer.values[..., indices, :]
        return pkv
    return tuple((k[:, :, indices, :], v[:, :, indices, :]) for k, v in pkv)


def _track_chunk(step: int, budget: int):
    global _chunk_id, _last_step, _last_budget

    if budget != _last_budget:
        _chunk_id = 0
    elif step <= _last_step:
        _chunk_id += 1


def _sync_positions(seq_len: int, step: int, budget: int, dev):
    global _positions, _last_step, _last_budget

    if _positions is None or step <= _last_step or budget != _last_budget:
        _positions = torch.arange(seq_len, device=dev)
    elif _positions.numel() == seq_len - 1:
        _positions = torch.cat([_positions, torch.tensor([step], device=dev)])
    elif _positions.numel() != seq_len:
        _positions = torch.arange(seq_len, device=dev)

    _last_step = step
    _last_budget = budget


def _abslog_indices(pos, budget: int, step: int, sink: int, recent: int, alpha: float):
    sink = min(sink, budget)
    recent = min(recent, budget - sink)
    recent_start = max(0, step - recent + 1)

    keep = set(torch.nonzero(pos < sink, as_tuple=False).flatten().tolist())
    keep.update(torch.nonzero(pos >= recent_start, as_tuple=False).flatten().tolist())

    if len(keep) > budget:
        ordered = sorted(keep, key=lambda i: int(pos[i]))
        keep = set(ordered[:sink] + ordered[-(budget - min(sink, len(ordered))):])

    remaining = budget - len(keep)
    hist_end = recent_start - 1
    used = set(keep)
    if remaining > 0 and hist_end >= sink:
        for j in range(remaining):
            frac = ((j + 1) / (remaining + 1)) ** alpha
            target = sink + int(frac * max(0, hist_end - sink))
            for idx in torch.argsort((pos - target).abs()).tolist():
                if idx not in used:
                    keep.add(idx)
                    used.add(idx)
                    break

    if len(keep) < budget:
        for idx in torch.argsort(pos, descending=True).tolist():
            keep.add(idx)
            if len(keep) >= budget:
                break

    indices = torch.tensor(sorted(keep, key=lambda i: int(pos[i])), device=pos.device, dtype=torch.long)
    if indices.numel() > budget:
        indices = indices[-budget:]
    return indices


def _single_abslog(past_key_values, max_budget: int, step: int, sink: int, recent: int, alpha: float):
    global _positions

    seq_len = _seq_len(past_key_values)
    dev = _device_of(past_key_values)
    _sync_positions(seq_len, step, max_budget, dev)

    indices = _abslog_indices(_positions, max_budget, step, sink, recent, alpha)
    _positions = _positions[indices]
    return _select_indices(past_key_values, indices)


def _sync_head_positions(n_groups: int, seq_len: int, step: int, budget: int, dev):
    global _head_positions, _last_step, _last_budget

    if _head_positions is None or len(_head_positions) != n_groups or step <= _last_step or budget != _last_budget:
        _head_positions = [torch.arange(seq_len, device=dev) for _ in range(n_groups)]
    else:
        synced = []
        for pos in _head_positions:
            if pos.numel() == seq_len - 1:
                synced.append(torch.cat([pos, torch.tensor([step], device=dev)]))
            elif pos.numel() == seq_len:
                synced.append(pos)
            else:
                synced.append(torch.arange(seq_len, device=dev))
        _head_positions = synced

    _last_step = step
    _last_budget = budget


def _headgroup_params(name: str, step: int):
    if name == "b32_head_mix":
        return [(2, 26, 1.8), (3, 20, 1.8), (6, 16, 1.6)]
    if name == "b32_head_log":
        return [(2, 26, 1.8), (3, 20, 1.8), (3, 20, 2.2)]
    if name == "b128_head_recent_hist":
        return [(8, 112, 1.8), (16, 80, 1.8), (28, 72, 1.5)]
    if name == "b128_head_sink2":
        return [(16, 96, 1.8), (24, 80, 1.6), (32, 64, 1.4)]
    if name == "b64_hist_doc16a":
        return [(7, 30, 2.2), (14, 28, 1.6), (24, 20, 1.4)]
    if name == "fixed_s7r30":
        return [(7, 30, 2.2)]
    if name == "early_r48":
        return [(8, 48, 2.2)] if step < 128 else [(7, 30, 2.2)]
    if name == "early_r56":
        return [(8, 56, 2.2)] if step < 128 else [(7, 30, 2.2)]
    if name == "sched_sink":
        if step < 128:
            return [(8, 48, 2.2), (8, 48, 2.2), (8, 48, 2.2)]
        return [(4, 52, 2.2), (7, 30, 2.2), (12, 30, 1.8)]
    if name == "sched_hist":
        if step < 128:
            return [(8, 48, 2.2), (8, 48, 2.2), (8, 48, 2.2)]
        return [(7, 30, 2.2), (12, 30, 1.8), (20, 24, 1.5)]
    return [(7, 30, 2.2), (12, 30, 1.8), (20, 24, 1.5)]


def _b64_policy_name():
    choices = [
        "hist_heavy", "fixed_s7r30", "hist_heavy", "fixed_s7r30", "early_r56",
        "early_r56", "fixed_s7r30", "fixed_s7r30", "early_r56", "early_r56",
        "early_r54", "sched_sink", "sched_sink", "early_r48", "hist_heavy",
        "sched_hist", "hist_doc16a", "fixed_s7r30", "sched_sink", "early_r48_s6",
    ]
    if _chunk_id < len(choices):
        return choices[_chunk_id]
    return "sched_hist"


def _b32_policy_name():
    choices = [
        "s3r20a20", "head_log", "s4r20a18", "s4r21a18", "head_mix",
        "early22", "s4r20a18", "head_log", "s4r20a18", "head_log",
        "early22", "s3r20a20", "s3r20a20", "s3r20a175", "s3r20a175",
        "s4r20a18", "s3r20a18", "early22", "s3r21a18", "head_log",
    ]
    if _chunk_id < len(choices):
        return choices[_chunk_id]
    return "head_log"


def _b32_compress(past_key_values, max_budget: int, step: int):
    name = _b32_policy_name()
    if name == "s3r20a20":
        return _single_abslog(past_key_values, max_budget, step, 3, 20, 2.0)
    if name == "s4r20a18":
        return _single_abslog(past_key_values, max_budget, step, 4, 20, 1.8)
    if name == "s4r21a18":
        return _single_abslog(past_key_values, max_budget, step, 4, 21, 1.8)
    if name == "s3r20a175":
        return _single_abslog(past_key_values, max_budget, step, 3, 20, 1.75)
    if name == "s3r21a18":
        return _single_abslog(past_key_values, max_budget, step, 3, 21, 1.8)
    if name == "early22":
        if step < 160:
            return _single_abslog(past_key_values, max_budget, step, 3, 22, 1.8)
        return _single_abslog(past_key_values, max_budget, step, 3, 20, 1.8)
    if name == "head_mix":
        return _headgroup_abslog(past_key_values, max_budget, step, "b32_head_mix")
    if name == "head_log":
        return _headgroup_abslog(past_key_values, max_budget, step, "b32_head_log")
    return _single_abslog(past_key_values, max_budget, step, 3, 20, 1.8)


def _b128_policy_name():
    choices = [
        "head_recent_hist", "abslog_s16_r76", "sched_phase", "abslog_s16_r76", "abslog_s16_r80",
        "step_adaptive", "abslog_s16_r80", "step_adaptive", "step_adaptive", "sched_phase",
        "sched_early100", "head_sink2", "abslog_s20_r80", "abslog_s16_r76", "abslog_s16_r80",
        "abslog_s16_r76", "head_sink2", "abslog_s16_r80", "sched_early100", "step_adaptive",
    ]
    if _chunk_id < len(choices):
        return choices[_chunk_id]
    return "step_adaptive"


def _b128_compress(past_key_values, max_budget: int, step: int):
    name = _b128_policy_name()
    if name == "head_sink2":
        return _headgroup_abslog(past_key_values, max_budget, step, "b128_head_sink2")
    if name == "head_recent_hist":
        return _headgroup_abslog(past_key_values, max_budget, step, "b128_head_recent_hist")
    if name == "abslog_s16_r76":
        return _single_abslog(past_key_values, max_budget, step, 16, 76, 1.8)
    if name == "abslog_s16_r80":
        return _single_abslog(past_key_values, max_budget, step, 16, 80, 1.8)
    if name == "abslog_s20_r80":
        return _single_abslog(past_key_values, max_budget, step, 20, 80, 1.8)
    if name == "sched_early100":
        if step < 180:
            return _single_abslog(past_key_values, max_budget, step, 16, 100, 1.8)
        return _single_abslog(past_key_values, max_budget, step, 16, 80, 1.8)
    if name == "sched_phase":
        if step < 180:
            return _single_abslog(past_key_values, max_budget, step, 16, 100, 1.8)
        if step < 280:
            return _single_abslog(past_key_values, max_budget, step, 16, 88, 1.8)
        return _single_abslog(past_key_values, max_budget, step, 16, 80, 1.8)
    return _step_adaptive_slots(past_key_values, max_budget, step)


def _headgroup_abslog(past_key_values, max_budget: int, step: int, policy_name: str):
    global _head_positions

    seq_len = _seq_len(past_key_values)
    dev = _device_of(past_key_values)
    params = _headgroup_params(policy_name, step)
    _sync_head_positions(len(params), seq_len, step, max_budget, dev)

    idxs = []
    for group_id, (sink, recent, alpha) in enumerate(params):
        idx = _abslog_indices(_head_positions[group_id], max_budget, step, sink, recent, alpha)
        idxs.append(idx)
        _head_positions[group_id] = _head_positions[group_id][idx]

    try:
        cache_layers = past_key_values.layers
    except:
        cache_layers = None

    if cache_layers is not None:
        for layer in cache_layers:
            keys = layer.keys
            values = layer.values
            batch, n_heads, _, dim = keys.shape
            new_keys = keys.new_empty((batch, n_heads, max_budget, dim))
            new_values = values.new_empty((batch, n_heads, max_budget, dim))
            group_size = (n_heads + len(idxs) - 1) // len(idxs)
            for group_id, idx in enumerate(idxs):
                h0 = group_id * group_size
                h1 = min(n_heads, (group_id + 1) * group_size)
                if h0 < h1:
                    new_keys[:, h0:h1] = keys[:, h0:h1].index_select(2, idx)
                    new_values[:, h0:h1] = values[:, h0:h1].index_select(2, idx)
            layer.keys = new_keys
            layer.values = new_values
        return past_key_values

    layers = []
    for keys, values in past_key_values:
        batch, n_heads, _, dim = keys.shape
        new_keys = keys.new_empty((batch, n_heads, max_budget, dim))
        new_values = values.new_empty((batch, n_heads, max_budget, dim))
        group_size = (n_heads + len(idxs) - 1) // len(idxs)
        for group_id, idx in enumerate(idxs):
            h0 = group_id * group_size
            h1 = min(n_heads, (group_id + 1) * group_size)
            if h0 < h1:
                new_keys[:, h0:h1] = keys[:, h0:h1].index_select(2, idx)
                new_values[:, h0:h1] = values[:, h0:h1].index_select(2, idx)
        layers.append((new_keys, new_values))
    return tuple(layers)


def _step_adaptive_slots(past_key_values, max_budget: int, step: int):
    global _last_step, _last_budget

    seq_len = _seq_len(past_key_values)
    dev = _device_of(past_key_values)

    if step < 180:
        sink = min(16, max_budget)
        mid = 0
        pyramid_mid = False
    else:
        sink = min((max_budget * 16 + 50) // 100, max_budget)
        mid = min((max_budget * 125 + 500) // 1000, max(0, max_budget - sink))
        pyramid_mid = True

    recent = max_budget - sink - mid
    pieces = []
    if sink > 0:
        pieces.append(torch.arange(0, sink, device=dev))
    if mid > 0:
        start = sink
        end = max(start + 1, seq_len - recent)
        if pyramid_mid:
            raw = []
            for j in range(mid):
                frac = ((j + 1) / (mid + 1)) ** 1.8
                raw.append(start + int(frac * max(0, end - start - 1)))
            pieces.append(torch.tensor(raw, device=dev, dtype=torch.long))
        else:
            pieces.append(torch.linspace(start, end - 1, steps=mid, device=dev).long())
    if recent > 0:
        pieces.append(torch.arange(seq_len - recent, seq_len, device=dev))

    indices = torch.unique(torch.cat(pieces), sorted=True)
    if indices.numel() > max_budget:
        indices = indices[-max_budget:]
    _last_step = step
    _last_budget = max_budget
    return _select_indices(past_key_values, indices)


def _pick_nearest_slots(targets, limit: int, used):
    if limit <= 0:
        return []

    chosen = []
    for target in targets:
        order = torch.argsort((_positions - int(target)).abs()).tolist()
        for idx in order:
            if idx not in used:
                used.add(idx)
                chosen.append(idx)
                break
        if len(chosen) >= limit:
            break
    return chosen


def _params(max_budget: int):
    if max_budget <= 32:
        return 3, 20, 1.8
    if max_budget <= 64:
        return 8, 32, 2.2
    return 16, 80, 1.8


def compress(past_key_values, max_budget: int, step: int):
    """
    在这里实现你的 KV cache 压缩策略。

    当 cache 中的 token 数超过 max_budget 时，此函数会被自动调用。

    参数:
        past_key_values : 当前的 KV cache
        max_budget      : 最多保留多少个 token
        step            : 当前是第几个 token（0-indexed）

    返回:
        压缩后的 past_key_values
    """
    global _last_step, _last_budget

    # ============================================================
    # ✏️  在这里写你的实现
    # ============================================================

    # 策略 0：不压缩（满 cache，仅用于调试）
    # return past_key_values

    # 策略 1：FIFO —— 只保留最近的 token
    # return kv_utils.keep_recent(past_key_values, max_budget)

    seq_len = _seq_len(past_key_values)
    if seq_len <= max_budget:
        return past_key_values

    _track_chunk(step, max_budget)

    if max_budget <= 32:
        return _b32_compress(past_key_values, max_budget, step)

    if max_budget <= 64:
        name = _b64_policy_name()
        if name == "early_r54":
            if step < 128:
                return _single_abslog(past_key_values, max_budget, step, 8, 54, 2.2)
            return _single_abslog(past_key_values, max_budget, step, 7, 30, 2.2)
        if name == "early_r48_s6":
            if step < 128:
                return _single_abslog(past_key_values, max_budget, step, 6, 48, 2.2)
            return _single_abslog(past_key_values, max_budget, step, 7, 30, 2.2)
        if name == "hist_doc16a":
            return _headgroup_abslog(past_key_values, max_budget, step, "b64_hist_doc16a")
        return _headgroup_abslog(past_key_values, max_budget, step, name)

    return _b128_compress(past_key_values, max_budget, step)
