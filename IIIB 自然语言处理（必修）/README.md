# 自然语言处理（必修）实验资料

这是华中科技大学自然语言处理课程实验资料的整理版，包含课程组提供的实验基线、官方实验材料，以及个人在课程实验基础上的实现与实验记录。

## 资料归属

| 路径 | 内容 | 归属说明 |
| --- | --- | --- |
| `docs/official/` | 任务书、实验指导、报告格式示例、昇腾/ModelArts 手册 | 课程组或华为技术有限公司原始资料，详见 `docs/official/SOURCES.md` |
| `1 实验室1-中文分词源码/1 基于字典版/` | 字典分词基线 | 课程实验基线 |
| `1 实验室1-中文分词源码/2 基于深度学习版 （Bi-LSTM+CRF）/` | Bi-LSTM+CRF 分词基线 | 课程实验基线 |
| `2 实验室2-中文命名实体识别源码/实验二NER_基于深度学习版（Bi-LSTM+CRF）(1)/` | Bi-LSTM+CRF NER 基线 | 课程实验基线 |
| `my_bert/`、`my_bert_ner/`、实验三补充实现 | BERT、数据增强、融合实验、KV Cache 策略等 | 学生实验实现，结果仅代表本地实验 |
| `report/` | 脱敏后的 LaTeX 报告源码 | 个人报告源码；只保留 `.tex` 和编译所需的 `HUSTBlack.eps` |

教师资料和实验实现分开保存，实验实现不代表课程组官方答案。

## 目录结构

```text
docs/official/                           课程组、华为原始资料
report/                                  脱敏后的 LaTeX 报告源码
1 实验室1-中文分词源码/                  中文分词：基线与 BERT+CRF
2 实验室2-中文命名实体识别源码/          NER：基线与 BERT+BiLSTM+CRF
3 实验室3-从零开始的LLM构建/              MiniLLM 与 KV Cache Arena
5 昇腾基座实验/                           MindSpore/PyTorch 官方样例 Notebook
4 test_data(需要提交分词结果)20250414.txt  课程测试输入
```

## 文件取舍与数据政策

本目录不提交以下本地生成内容：

- 模型权重和 checkpoint（`*.pkl`）；
- Python 缓存、IDE 配置、训练日志和 LaTeX 编译产物；
- PKU 扩展语料、MSRA/RMRB 等外部或合并后的训练集快照；
- 带个人身份信息的报告 PDF。

外部数据集的获取和准备方式记录在 `data/README.md`。使用前请确认数据集的来源、许可和课程要求。

## 环境准备

建议使用 Python 3.10 或更高版本，并根据目标实验安装对应目录下的 `requirements.txt`。CPU 环境可以使用 PyTorch 官方 CPU 源；已有 CUDA/昇腾环境时，应安装与本机驱动和框架匹配的版本。

### 预训练模型

实验一和实验二通过 Hugging Face Transformers 自动获取：

```text
hfl/chinese-roberta-wwm-ext
```

首次执行 `run_bert.py`、`run_bert_ner.py` 或推理脚本时，`AutoTokenizer.from_pretrained` / `AutoModel.from_pretrained` 会将模型缓存到本机，不要把缓存目录复制进仓库。网络受限时，可在本机配置 Hugging Face 镜像或先离线准备同名模型目录。

KV Cache Arena 使用 `gpt2`，评测数据默认从 Hugging Face `wikitext-2-raw-v1` 获取；代码包含网络失败时的内置语料 fallback，但 fallback 结果不能与标准 WikiText-2 分数直接比较。

## 实验一：中文分词

教师基线位于 `1 实验室1-中文分词源码/`。个人 BERT+CRF 实现位于 `my_bert/`。

```powershell
Set-Location '1 实验室1-中文分词源码\my_bert'
pip install -r '..\2 基于深度学习版 （Bi-LSTM+CRF）\requirements.txt'
python download_and_merge_pku.py
python run_bert.py
python infer_bert.py --input_path '..\..\4 test_data(需要提交分词结果)20250414.txt' --output_path cws_result_submit.txt
```

`download_and_merge_pku.py` 只在本地生成扩展训练集；该训练集不随仓库发布。若只想使用课程提供的基础训练集，应按实验设置准备训练输入。

## 实验二：中文命名实体识别

教师基线和数据准备脚本位于 `2 实验室2-中文命名实体识别源码/实验二NER_基于深度学习版（Bi-LSTM+CRF）(1)/`，个人 BERT+BiLSTM+CRF 实现位于其 `my_bert_ner/` 子目录。

外部语料不在本仓库中。准备好课程允许使用的 RMRB 和 MSRA 数据后，按脚本要求放置文件，再运行：

```powershell
Set-Location '2 实验室2-中文命名实体识别源码\实验二NER_基于深度学习版（Bi-LSTM+CRF）(1)'
python merge_and_split.py
Set-Location my_bert_ner
python run_bert_ner.py
python infer_bert_ner.py
```

训练完成后，模型 checkpoint 和标签映射只保存在本地 `save/` 目录，不提交到仓库。

## 实验三：从零构建 LLM 与 KV Cache 压缩

先按 `3 实验室3-从零开始的LLM构建/README.md` 运行 Notebook。KV Cache Arena 的最小评测方式：

```powershell
Set-Location '3 实验室3-从零开始的LLM构建\kv_cache_arena'
pip install -r requirements.txt
python run_eval.py --quick
```

评测入口会自动获取 `gpt2` 和 WikiText-2；`strategy.py` 是课程要求的策略入口，`strategy_nonhack_lowk.py` 等是补充策略版本。

## 报告源码

报告源码位于 `report/nlp_report.tex`，编译依赖同目录的 `HUSTBlack.eps`。发布版已删除 PDF，并脱敏了学号、姓名、专业班级和 Notebook 中的个人训练文本。报告中的结果来自本地实验，重新训练时应记录 Python、PyTorch、Transformers、设备、数据版本和随机种子。

## 复现边界

本仓库提供代码、课程测试输入、数据准备脚本和运行说明，不保证在不同版本依赖、硬件和外部数据版本下得到完全相同的数值结果。提交新的实验结果时，请同时说明数据来源、模型来源、训练参数和评测方式。
