# 实验二：BERT+BiLSTM+CRF 中文命名实体识别

本目录是课程基线之上的补充实现，不代表课程组官方答案。模型采用 `hfl/chinese-roberta-wwm-ext` + 双层 BiLSTM + CRF，评估使用实体级严格匹配 F1。

## 文件说明

- `model_bert_ner.py`：BERT+BiLSTM+CRF 模型；
- `dataloader_bert_ner.py`：BIOES 数据加载和标签映射；
- `run_bert_ner.py`：训练与验证，生成本地 checkpoint；
- `infer_bert_ner.py`：加载 checkpoint 执行实体识别；
- `cws_ner_fusion.py`：CWS 与 NER 边界融合对比实验；
- `eval_fusion_on_cws_val.py`：在分词验证集上比较融合效果。

仓库不包含 RMRB、MSRA 或合并后的 `ner_train.txt`/`ner_valid.txt`。请先按照根目录 `data/README.md` 准备并运行上层目录的 `merge_and_split.py`。

## 运行

```powershell
Set-Location '..\实验二NER_基于深度学习版（Bi-LSTM+CRF）(1)'
python merge_and_split.py
Set-Location '..\my_bert_ner'
python run_bert_ner.py
python infer_bert_ner.py
```

首次运行会通过 Transformers 下载并缓存 `hfl/chinese-roberta-wwm-ext`。训练产生的 checkpoint、标签映射和日志只保留在本地 `save/`，不提交到仓库。

## 复现说明

NER 结果取决于外部语料版本、合并顺序、数据切分随机种子、依赖版本、设备和训练参数。报告中的指标是本地实验记录，重新训练时请同时记录这些信息。
