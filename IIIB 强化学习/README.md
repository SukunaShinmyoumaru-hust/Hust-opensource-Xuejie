# IIIB 强化学习课程实验材料

本目录保存 `IIIB 强化学习` 课程实验材料的可公开整理版本，包含实验代码、结果图片、课程任务材料索引，以及实验报告的 TeX 源文件。

## 目录概览

```text
IIIB 强化学习/
├── README.md
├── .gitignore
├── gridworld.py
├── visualization.py
├── RL_Experiment_Report.tex
├── HUSTBlack.eps
├── exp1/
├── exp2/
├── exp3/
├── exp4/
├── exp5/
├── exp6/
└── exp7/
```

- `gridworld.py`：根目录共享环境模块，供多个实验复用。
- `visualization.py`：根目录共享可视化模块，供多个实验复用。
- `RL_Experiment_Report.tex`：实验报告的 TeX 源文件，保留在仓库中。
- `HUSTBlack.eps`：报告模板使用的图形资源，保留在仓库中。
- 根目录中的课程课件、任务书、封面模板等文件为课程原始材料，按原文件名保留。

## 七个实验目录

每个实验目录都保留了各自的 `run_all.py`，并按现有脚本和文件名组织内容：

1. `exp1`：动态规划基础实验，包含 `value_iteration.py`、`policy_iteration.py`、`truncated_policy_iteration.py`，覆盖值迭代、策略迭代和截断策略迭代。
2. `exp2`：蒙特卡罗方法实验，包含 `mc_basic.py`、`mc_exploring_starts.py`、`mc_epsilon_greedy.py`、`coin_flip.py`，覆盖基本 MC、探索起点、`ε`-greedy 控制和相关对照任务。
3. `exp3`：随机近似与随机优化实验，包含 `task1_sample_mean.py`、`task2_robbins_monro.py`、`task3_stochastic_optimization.py`、`task4_convergence_analysis.py`。
4. `exp4`：时序差分学习实验，包含 `task2_td0_eval.py`、`task3_sarsa.py`、`task4_qlearning.py`，并配有 `env.py`、`dp_benchmark.py` 等辅助脚本。
5. `exp5`：值函数近似与深度 Q 学习实验，包含 `task1_td_linear.py`、`task2_sarsa.py`、`task3_q_learning.py`、`task4_dqn.py`。
6. `exp6`：策略函数近似实验，现有文件包括 `policy.py`、`task1.py`、`task2.py`、`task3.py`，覆盖策略参数化、训练与分析。
7. `exp7`：Actor-Critic 实验，包含 `task1_qac.py`、`task2_a2c.py`、`task3_off_policy.py`，覆盖 QAC、A2C 和异策略 Actor-Critic。

除根目录共享模块外，各实验目录通常还包含本实验的说明文件、脚本和 `results/` 输出目录。

## 运行方式

建议使用 Python 3.8 及以上版本。如果本地环境中 `python` 不是目标解释器，可改用对应环境中的可执行文件。

运行单个实验时，在目标实验目录中执行：

```bash
cd exp1
python run_all.py
```

按顺序完整重跑七个实验时，可从仓库根目录依次进入每个实验目录执行各自的 `run_all.py`：

```bash
cd exp1
python run_all.py
cd ..

cd exp2
python run_all.py
cd ..

cd exp3
python run_all.py
cd ..

cd exp4
python run_all.py
cd ..

cd exp5
python run_all.py
cd ..

cd exp6
python run_all.py
cd ..

cd exp7
python run_all.py
```

## 依赖说明

根据现有脚本依赖，运行本目录内容通常需要：

- Python 3.8+
- NumPy
- Matplotlib
- PyTorch（`exp5/task4_dqn.py` 的 DQN 实验需要）

本文档不声明未经核实的具体包版本。

## 结果文件与保留内容

- 各实验生成的图像和结果文件保存在对应实验目录的 `results/` 下。
- 根目录保留 `RL_Experiment_Report.tex`，用于保存实验报告的 TeX 源。
- 根目录保留 `HUSTBlack.eps`，用于报告模板相关资源。
- 各实验目录中的源码、说明文档和已提交的结果 PNG 图片一并保留。

## 结果图像重跑说明

当前结果图片是通过顺序执行七个实验目录中的 `run_all.py` 重新生成得到的。热力图标注文字会根据渲染后背景亮度自动选择浅色背景用黑字、深色背景用白字的显示方式。若要复现这批修正后的图片，需要完整重跑全部七个实验，而不是只替换单张图片。

## 官方材料说明

根目录中的 `26-HUSTCS-RL课件-马志远.pdf`、`RL课程-基础实验-任务书.pdf`、`RL_task_book.txt`、`RL任务书.docx` 和 `RL实验报告封面模板.docx` 为课程课件、任务书和封面模板等原始材料，不将其表述为本目录整理生成的内容。

## 发布时不包含的内容

为保持公开材料目录整洁，下列内容不作为发布内容保留：

- 个人实验报告 PDF
- LaTeX 中间文件和构建产物
- Python 缓存目录与字节码文件
- 本地临时目录与临时日志文件
