# 数据集获取与本地生成文件

为控制仓库体积并避免重新分发外部数据集，本发布目录不包含 PKU 扩展语料、MSRA/RMRB 语料或它们合并后的训练快照。

## 中文分词扩展语料

`1 实验室1-中文分词源码/my_bert/download_and_merge_pku.py` 会从脚本内记录的 PKU 语料地址下载 `pku_training.utf8`，并与课程提供的基础训练数据合并，生成本地文件：

```text
my_bert/data/pku_training_raw.txt
my_bert/data/train_expanded.txt
```

这些文件是运行时生成物，已在 `.gitignore` 中排除。下载前请确认 PKU/SIGHAN 数据的使用和再分发许可。

## 命名实体识别语料

`2 实验室2-中文命名实体识别源码/实验二NER_基于深度学习版（Bi-LSTM+CRF）(1)/merge_and_split.py` 需要用户自行准备：

```text
data/data/RMRB_NER_CORPUS.txt
data/external_datasets/msra/train.bio.txt
data/external_datasets/msra/dev.bio.txt
data/external_datasets/msra/test.bio.txt
```

脚本将不同标注格式统一后生成 `ner_train.txt` 和 `ner_valid.txt`，供基线和 `my_bert_ner` 使用。生成文件不提交；数据来源和许可由使用者自行确认。

## 模型与评测数据

- `hfl/chinese-roberta-wwm-ext`：由 Transformers 在首次运行时下载并缓存。
- `gpt2`：KV Cache Arena 在首次评测时自动下载。
- `wikitext-2-raw-v1`：优先通过 `datasets` 获取，失败时尝试 Hugging Face Hub parquet，最后使用代码内置的小语料 fallback。
- 课程中文分词测试输入保留在仓库根目录，文件名为 `4 test_data(需要提交分词结果)20250414.txt`。

模型缓存、数据缓存和训练 checkpoint 都只保留在本地环境，不提交到 Git。
