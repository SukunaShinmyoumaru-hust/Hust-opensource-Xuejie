# 实验一：BERT+CRF 中文分词

本目录是课程基线之上的补充实现，不代表课程组官方答案。模型采用 `hfl/chinese-roberta-wwm-ext` + 线性层 + CRF，评估使用词级 Precision/Recall/F1。

## 文件说明

- `model_bert.py`：BERT+CRF 模型；
- `dataloader_bert.py`：数据加载和 BMES 标签处理；
- `run_bert.py`：训练与验证，生成本地 `save/best_model.pkl`；
- `infer_bert.py`：加载本地 checkpoint 执行分词；
- `eval_bert.py`：验证集或结果文件评测；
- `download_and_merge_pku.py`：下载 PKU 扩展语料并生成本地训练集；
- `build_pku_dict.py`、`eval_dict_on_cws_val.py`：可选词典后处理实验。

仓库不包含外部 PKU 语料、合并训练集或模型权重。请先阅读根目录的 `data/README.md`。

## 运行

```powershell
pip install -r '..\2 基于深度学习版 （Bi-LSTM+CRF）\requirements.txt'
python download_and_merge_pku.py
python run_bert.py
python infer_bert.py --input_path '..\..\4 test_data(需要提交分词结果)20250414.txt' --output_path cws_result_submit.txt
```

首次运行会通过 Transformers 下载并缓存 `hfl/chinese-roberta-wwm-ext`。训练产生的 checkpoint、日志和缓存只保留在本地，已由上层 `.gitignore` 排除。

## 复现说明

扩展语料、依赖版本、设备和随机种子会影响结果。报告中的指标是本地实验记录，重新运行时请记录训练数据版本、模型缓存来源、batch size、epoch、学习率和评测文件。
