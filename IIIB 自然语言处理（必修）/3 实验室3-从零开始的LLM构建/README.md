# 实验三：从零开始构建 LLM

本目录包含两部分：一个从零实现 Transformer/LLM 核心组件的 Notebook，以及一个使用真实 GPT-2 评估 KV Cache 压缩策略的实验。Notebook 中的填空、实验输出和策略文件属于补充实验实现，不代表课程组官方答案。

## MiniLLM Notebook

`llm-from-scratch.ipynb` 按章节实现：

1. 字符分词、Token/Position Embedding；
2. 缩放点积注意力、因果掩码、多头注意力；
3. RMSNorm、SwiGLU、残差连接和 Pre-Norm；
4. Weight Tying、交叉熵、AdamW、梯度裁剪；
5. KV Cache 推理。

Notebook 使用小型随机模型从头训练，不依赖外部预训练权重。建议使用 Python 3.10、PyTorch 和 Jupyter Kernel。

`validation_files/` 中的 `expected_output.pt` 是 Notebook 验证用的参考输出，不是训练模型权重。

## KV Cache Arena

进入 `kv_cache_arena/` 后安装 `requirements.txt`：

```powershell
pip install -r requirements.txt
python run_eval.py --quick
```

评估引擎首次运行会自动获取 `gpt2` 和 WikiText-2；网络不可用时会尝试 Hub parquet，最后退回内置语料。`strategy.py` 是课程策略入口，`strategy_nonhack_lowk.py` 和 `strategy_smoke_safe.py` 是补充策略版本。

模型缓存、数据缓存、日志和调参归档不属于发布内容。
