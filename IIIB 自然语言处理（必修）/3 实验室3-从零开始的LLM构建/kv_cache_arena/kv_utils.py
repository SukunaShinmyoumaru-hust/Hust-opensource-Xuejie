"""
KV Cache 工具函数 — 学生可直接调用，请勿修改此文件。

支持 transformers 5.x DynamicCache 与旧版 tuple-of-tuples 两种格式。
但是我只在新版 DynamicCache 上测试过，旧版接口可能需要调整。
"""
import torch


# ─── 基础查询 ────────────────────────────────────────────────────────────────

def get_seq_len(pkv) -> int:
    """返回 cache 中当前缓存的 token 数。"""
    if hasattr(pkv, 'layers'):
        return pkv.layers[0].keys.shape[-2]
    return pkv[0][0].shape[2]


def get_device(pkv) -> torch.device:
    """返回 cache 所在的设备。"""
    if hasattr(pkv, 'layers'):
        return pkv.layers[0].keys.device
    return pkv[0][0].device


def get_num_layers(pkv) -> int:
    """返回 cache 的层数。"""
    if hasattr(pkv, 'layers'):
        return len(pkv.layers)
    return len(pkv)


# ─── 核心操作 ────────────────────────────────────────────────────────────────

def select_indices(pkv, indices: torch.Tensor):
    """
    按 indices 在 seq_len 维度上保留指定位置的 KV 向量。

    参数:
        pkv     : past_key_values (DynamicCache 或 tuple)
        indices : LongTensor，要保留的 token 位置

    返回:
        裁剪后的 past_key_values（与输入同格式）

    示例:
        # 保留位置 0, 5, 10
        indices = torch.tensor([0, 5, 10])
        pkv = kv_utils.select_indices(pkv, indices)
    """
    if hasattr(pkv, 'layers'):
        for layer in pkv.layers:
            layer.keys   = layer.keys[...,   indices, :]
            layer.values = layer.values[..., indices, :]
        return pkv
    return tuple(
        (k[:, :, indices, :], v[:, :, indices, :])
        for k, v in pkv
    )


# ─── 常用策略封装 ─────────────────────────────────────────────────────────────

def keep_recent(pkv, n: int):
    """
    FIFO：只保留最近的 n 个 token，丢弃最旧的。

    示例:
        pkv = kv_utils.keep_recent(pkv, max_budget)
    """
    seq_len = get_seq_len(pkv)
    if seq_len <= n:
        return pkv
    dev = get_device(pkv)
    indices = torch.arange(seq_len - n, seq_len, device=dev)
    return select_indices(pkv, indices)



def keep_random(pkv, n: int, seed: int = None):
    """
    随机保留 n 个 token（每次调用结果不同，除非固定 seed）。

    示例:
        pkv = kv_utils.keep_random(pkv, max_budget)
    """
    seq_len = get_seq_len(pkv)
    if seq_len <= n:
        return pkv
    dev = get_device(pkv)
    if seed is not None:
        torch.manual_seed(seed)
    indices = torch.sort(torch.randperm(seq_len, device=dev)[:n]).values
    return select_indices(pkv, indices)
