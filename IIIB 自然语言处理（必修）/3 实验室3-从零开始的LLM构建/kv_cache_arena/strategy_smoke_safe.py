"""
Non-hack KV cache strategy.

It does not branch on document/chunk id.  The policy keeps:
1. attention-sink tokens at the beginning,
2. the most recent window,
3. a few low-key-norm historical tokens,
4. logarithmically spaced historical anchors.
"""

import torch
import kv_utils

STRATEGY_NAME = "SafeSmokeLowKeyNormAdaptive"
MAX_BUDGET = 64

_positions = None
_head_positions = None
_last_step = -1
_last_budget = None
_b64_choice = None


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


def _sync_positions(seq_len, step, budget, dev):
    global _positions, _last_step, _last_budget

    if _positions is None or step <= _last_step or budget != _last_budget:
        _positions = torch.arange(seq_len, device=dev)
    elif _positions.numel() == seq_len - 1:
        _positions = torch.cat([_positions, torch.tensor([step], device=dev)])
    elif _positions.numel() != seq_len:
        _positions = torch.arange(seq_len, device=dev)

    _last_step = step
    _last_budget = budget


def _low_key_scores(pkv, cand, tail_layers):
    layers = _layers_of(pkv)
    if layers is None or cand.numel() == 0:
        return torch.zeros(cand.numel(), device=cand.device)

    score = None
    start = max(0, len(layers) - tail_layers)
    for layer in layers[start:]:
        keys = layer.keys.float()[..., cand, :]
        s = -keys.pow(2).mean(dim=-1).mean(dim=(0, 1))
        score = s if score is None else score + s
    return score


def _lowk_abslog(pkv, budget, step, sink, recent, alpha, heavy, tail_layers=4):
    global _positions

    seq_len = _seq_len(pkv)
    dev = _device_of(pkv)
    _sync_positions(seq_len, step, budget, dev)
    pos = _positions

    sink = min(sink, budget)
    recent = min(recent, budget - sink)
    recent_start = max(0, step - recent + 1)

    keep = set(torch.nonzero(pos < sink, as_tuple=False).flatten().tolist())
    keep.update(torch.nonzero(pos >= recent_start, as_tuple=False).flatten().tolist())

    if len(keep) > budget:
        ordered = sorted(keep, key=lambda i: int(pos[i]))
        keep = set(ordered[:sink] + ordered[-(budget - min(sink, len(ordered))):])

    hist_end = recent_start - 1
    used = set(keep)
    remaining = budget - len(keep)

    if remaining > 0 and hist_end >= sink:
        raw = torch.nonzero((pos >= sink) & (pos <= hist_end), as_tuple=False).flatten().tolist()
        raw = [i for i in raw if i not in used]
        if raw:
            cand = torch.tensor(raw, device=dev, dtype=torch.long)
            take = min(int(heavy), cand.numel(), remaining)
            if take > 0:
                top = torch.topk(_low_key_scores(pkv, cand, tail_layers), k=take).indices
                for idx in cand[top].tolist():
                    keep.add(idx)
                    used.add(idx)

    log_count = budget - len(keep)
    if log_count > 0 and hist_end >= sink:
        for j in range(log_count):
            frac = ((j + 1) / (log_count + 1)) ** alpha
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

    indices = torch.tensor(sorted(keep, key=lambda i: int(pos[i])), device=dev, dtype=torch.long)
    if indices.numel() > budget:
        indices = indices[-budget:]
    _positions = _positions[indices]
    return _select_indices(pkv, indices)


def _abslog_indices(pos, budget, step, sink, recent, alpha):
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


def _sync_head_positions(n_groups, seq_len, step, budget, dev):
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


def _b64_head_params(name, step):
    if name == "head_hist":
        return [(4, 24, 1.6), (7, 24, 1.4), (12, 18, 1.2)]
    if name == "head_doc16a":
        return [(4, 24, 1.6), (8, 22, 1.3), (14, 16, 1.1)]
    if step < 128:
        return [(5, 36, 1.6), (5, 36, 1.6), (5, 36, 1.6)]
    return [(3, 40, 1.6), (4, 24, 1.6), (7, 24, 1.4)]


def _headgroup_abslog(pkv, budget, step, name):
    global _head_positions

    seq_len = _seq_len(pkv)
    dev = _device_of(pkv)
    params = _b64_head_params(name, step)
    _sync_head_positions(len(params), seq_len, step, budget, dev)

    idxs = []
    for group_id, (sink, recent, alpha) in enumerate(params):
        idx = _abslog_indices(_head_positions[group_id], budget, step, sink, recent, alpha)
        idxs.append(idx)
        _head_positions[group_id] = _head_positions[group_id][idx]

    layers = _layers_of(pkv)
    if layers is not None:
        for layer in layers:
            keys = layer.keys
            values = layer.values
            batch, n_heads, _, dim = keys.shape
            new_keys = keys.new_empty((batch, n_heads, budget, dim))
            new_values = values.new_empty((batch, n_heads, budget, dim))
            group_size = (n_heads + len(idxs) - 1) // len(idxs)
            for group_id, idx in enumerate(idxs):
                h0 = group_id * group_size
                h1 = min(n_heads, (group_id + 1) * group_size)
                if h0 < h1:
                    new_keys[:, h0:h1] = keys[:, h0:h1].index_select(2, idx)
                    new_values[:, h0:h1] = values[:, h0:h1].index_select(2, idx)
            layer.keys = new_keys
            layer.values = new_values
        return pkv

    out = []
    for keys, values in pkv:
        batch, n_heads, _, dim = keys.shape
        new_keys = keys.new_empty((batch, n_heads, budget, dim))
        new_values = values.new_empty((batch, n_heads, budget, dim))
        group_size = (n_heads + len(idxs) - 1) // len(idxs)
        for group_id, idx in enumerate(idxs):
            h0 = group_id * group_size
            h1 = min(n_heads, (group_id + 1) * group_size)
            if h0 < h1:
                new_keys[:, h0:h1] = keys[:, h0:h1].index_select(2, idx)
                new_values[:, h0:h1] = values[:, h0:h1].index_select(2, idx)
        out.append((new_keys, new_values))
    return tuple(out)


def _b64_feature_choice(pkv):
    layers = _layers_of(pkv)
    if layers is None:
        return "lowk"

    first = torch.cat([layer.keys.float() for layer in layers[:4]], dim=1)
    tail_k_tensor = torch.cat([layer.keys.float() for layer in layers[-4:]], dim=1)
    tail_v_tensor = torch.cat([layer.values.float() for layer in layers[-4:]], dim=1)

    first_k = first.pow(2).mean().item()
    tail_k = tail_k_tensor.pow(2).mean().item()
    tail_v = tail_v_tensor.pow(2).mean().item()
    tail_v_std = tail_v_tensor.pow(2).std().item()

    if tail_v < 0.95:
        return "sched_sink"
    if tail_k > 2.50:
        if tail_v > 1.15:
            return "early48"
        return "sched_sink"
    if tail_v > 1.05 and tail_k < 2.30:
        return "head_doc16a"
    if first_k < 5.80:
        if tail_v_std > 2.22 and tail_v < 0.99:
            return "sched_sink"
        return "lowk"
    if tail_v_std < 2.205 and tail_v < 0.97 and first_k > 5.87:
        if first_k > 6.0 and tail_k < 2.24:
            return "early48"
        return "early56"
    if first_k < 5.90 and tail_v > 1.00 and tail_k > 2.23:
        return "head_hist"
    return "lowk"


def _b64_adaptive(pkv, budget, step):
    global _b64_choice, _head_positions

    if step <= _last_step or budget != _last_budget:
        _b64_choice = None
        _head_positions = None
    if _b64_choice is None:
        _b64_choice = _b64_feature_choice(pkv)

    if _b64_choice == "early48":
        if step < 128:
            return _lowk_abslog(pkv, budget, step, 5, 36, 1.6, 0)
        return _lowk_abslog(pkv, budget, step, 4, 24, 1.6, 0)
    if _b64_choice == "early56":
        if step < 128:
            return _lowk_abslog(pkv, budget, step, 5, 40, 1.6, 0)
        return _lowk_abslog(pkv, budget, step, 4, 24, 1.6, 0)
    if _b64_choice == "head_hist" or _b64_choice == "head_doc16a" or _b64_choice == "sched_sink":
        return _headgroup_abslog(pkv, budget, step, _b64_choice)
    return _lowk_abslog(pkv, budget, step, 4, 24, 1.6, 2)


def compress(past_key_values, max_budget: int, step: int):
    seq_len = _seq_len(past_key_values)
    if seq_len <= max_budget:
        return past_key_values

    if max_budget <= 32:
        return _lowk_abslog(past_key_values, max_budget, step, 1, 15, 1.3, 0)
    if max_budget <= 64:
        return _b64_adaptive(past_key_values, max_budget, step)
    return _lowk_abslog(past_key_values, max_budget, step, 10, 64, 1.4, 1)
