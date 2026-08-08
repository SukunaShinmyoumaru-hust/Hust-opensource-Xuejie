# KV Cache Arena

本实验要求在有限 KV Cache budget 下设计压缩策略，并在 WikiText-2 测试集上使用 Perplexity 评估质量损失。Perplexity 越低，压缩带来的质量损失越小。

## 文件说明

| 文件 | 说明 |
| --- | --- |
| `strategy.py` | 课程要求修改的策略入口 |
| `strategy_nonhack_lowk.py` | 补充的非硬编码策略 |
| `strategy_smoke_safe.py` | 个人整理的兼容性测试策略 |
| `run_eval.py` | 快速或完整评估入口 |
| `engine.py`、`kv_utils.py` | 评估引擎和工具函数 |

## 安装和运行

```powershell
# 无 GPU 时可先安装 CPU 版 PyTorch
pip install torch --index-url https://download.pytorch.org/whl/cpu
pip install -r requirements.txt

python run_eval.py --quick
python run_eval.py
python run_eval.py --no-baseline
python run_eval.py --budget 64
```

首次评估会通过 Transformers 下载 `gpt2`，并通过 `datasets`/Hugging Face Hub 获取 WikiText-2。数据和模型只缓存到本地，不提交到仓库；网络不可用时引擎有内置小语料 fallback。

## 策略接口

修改 `strategy.py` 中的 `compress(past_key_values, max_budget, step)`，返回裁剪后的 KV Cache。可使用 `kv_utils.get_seq_len`、`kv_utils.select_indices` 和 `kv_utils.keep_recent` 等工具。

评测结果受模型、数据版本、budget、设备和依赖版本影响。提交新结果时请同时记录这些条件。
