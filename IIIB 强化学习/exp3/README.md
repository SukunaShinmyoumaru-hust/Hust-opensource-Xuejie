# 实验三：随机近似

## 一、 实验概述
本实验探究随机近似（Stochastic Approximation）的数学收敛机制，重点分析梯度方差与步长条件的理论特征：
*   **均值增量估计**：验证一维高斯分布与二维均匀分布的样本均值在免存历史下的迭代收敛行为。
*   **Robbins-Monro 随机近似**：将估计转化写为求根方程 $g(w)=0$，对比四类经典步长（包括常数步长和衰减步长）在满足/不满足 RM 级数极限下的表现。
*   **随机优化对比**：在二次凸目标下对比批量梯度下降（BGD）、小批量梯度下降（MBGD）和随机梯度下降（SGD）在单步计算量、震荡幅度、学习率机制下的表现。
*   **鲁棒统计与异常分析**：运行 20 次独立种子实验进行蒙特卡罗误差方差统计，并分析各算法在含有大异常值干扰样本下的敏感性。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个任务文件：
```bash
python task1_sample_mean.py             # 任务 1
python task2_robbins_monro.py           # 任务 2
python task3_stochastic_optimization.py # 任务 3
python task4_convergence_analysis.py    # 任务 4
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `task1_sample_mean.py`：生成 1D 和 2D 样本，通过直接与增量方式计算期望，验证均值收敛。
*   `task2_robbins_monro.py`：设置步长为 $1/k, 1/k^{0.6}, 0.05, 1/k^2$ 并设定不同的初始点（$\pm 10$）进行求根迭代。
*   `task3_stochastic_optimization.py`：固定样本集上实现 BGD、MBGD（$m=5,50$）和 SGD 在固定与衰减学习率下的凸优化求导更新。
*   `task4_convergence_analysis.py`：多随机种子运行统计误差，包含异常值脏数据干扰下 BGD 与 SGD 的抗噪表现。
*   `run_all.py`：主脚本，顺序运行所有实验并生成对应的对比图表保存至 `results/`。

### 2. 结果图表 (`results/`)
*   `task1_*d_sample_mean.png`：样本散点图及增量平均数逼近真值的轨迹误差曲线。
*   `task2_*d_*.png`：四类步长在不同初始点下的逼近轨迹与残差曲线。
*   `task3_trajectories_2d.png` / `task3_error_vs_*.png`：三类梯度下降参数向最优解 $(0,0)$ 移动的二维轨迹和随 epoch/step 变化的误差曲线。
*   `task4_statistical_comparison.png`：多种子实验最终误差均值与标准差对比图。
*   `task4_outlier_*.png`：注入异常样本后 BGD 和 SGD 估计重心偏移对比图。

---

## 四、 实验关键设置参数

*   **1D 期望估计**：$X \sim N(2, 1)$，样本数 $N=1000$，真值 $\mu=2.0$。
*   **2D 期望估计**：$X \in \mathbb{R}^2$ 服从原点中心、边长 20 的均匀分布，样本数 $N=100$，真值 $\mu=(0,0)^T$。
*   **Robbins-Monro 初始值**：$w_0 \in \{-10.0, 10.0\}$。
*   **优化迭代轮数**：不少于 30 轮（Epochs），初始参数固定为 $w_0 = (15, 15)^T$，固定学习率 $\alpha = 0.1$，衰减学习率 $\alpha_k = \alpha_0 / (1 + 0.05k)$。
