# 实验五：值函数近似

## 一、 实验概述
本实验将表格型值网络拓展为参数化状态特征函数表示，解决了高维或连续状态空间无法建表的问题，并涉及深度强化学习基本机制：
*   **线性状态值估计 (TD-Linear)**：设计 $\phi(s) = [1, x, y]^T$ 及其高阶多项式，估计随机策略下的状态值，并与 TD-Table 对比。
*   **联合特征控制 (FA-Sarsa & FA-Q-learning)**：设计状态-动作联合独热特征，比较同策略与异策略的值函数近似控制。
*   **深度 Q 网络 (DQN) 初步**：实现一个具有单隐层（100 个 ReLU 神经元）的参数化深度网络，并验证经验回放缓冲区（Replay Buffer）和独立的延迟更新目标网络（Target Network）对打破样本时间相关性和稳定拟合目标的突出作用。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个文件：
```bash
python task1_td_linear.py           # 任务 1：值近似状态评估
python task2_sarsa.py               # 任务 2：函数近似 Sarsa 控制
python task3_q_learning.py          # 任务 3：函数近似 Q-learning 控制
python task4_dqn.py                 # 任务 4：DQN 控制对比
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `gridworld.py`：环境包装器。从项目根目录导入共享 `GridWorld` 环境。
*   `visualization.py`：可视化包装器。从项目根目录导入共享热力图和策略箭头图绘制接口。
*   `task1_td_linear.py`：对比 3维/6维/10维 特征以及 TD-Table 在状态评估中的 RMSE 性能。
*   `task2_sarsa.py`：线性函数近似的 Sarsa 控制，使用独热联合特征更新参数矩阵。
*   `task3_q_learning.py`：在线近似 Q-learning，并与函数近似 Sarsa 比较。
*   `task4_dqn.py`：实现 PyTorch DQN 算法，对比表格型 Q-learning 的收敛曲线，并输出损失图。
*   `run_all.py`：主脚本，统一运行实验并保存图表。

### 2. 结果图表 (`results/`)
*   `task1_rmse_comparison.png`：TD-Table 与三种多项式特征维度下线性值逼近的 RMSE 降速曲线。
*   `task1_*d_val.png`：各种特征空间下训练出的价值拟合热力图。
*   `task2_sarsa_convergence.png` / `task2_sarsa_policy.png`：FA-Sarsa 收敛曲线（回报 + 成功率）与最终贪婪箭头策略。
*   `task2_sarsa_policy_evolution.png`：FA-Sarsa 在不同训练回合的策略演变。
*   `task3_sarsa_vs_q_convergence.png`：线性近似下 Sarsa 和 Q-learning 的收敛速度对比。
*   `task4_dqn_vs_tabular.png`：DQN 和 Table Q-learning 在样本学习效率（测试成功率及最优路径长度）上的对比图。
*   `task4_dqn_loss.png` / `task4_dqn_policy.png`：DQN 的网络拟合 TD 损失曲线与学到的最终策略。

---

## 四、 实验关键设置参数

*   **TD-Linear 步长**：$\alpha = 0.001$。
*   **线性多项式特征**：3维 ($[1, x, y]^T$)、6维 ($3\text{维} + [x^2, y^2, xy]$)、10维 ($6\text{维} + [x^3, y^3, x^2y, xy^2]$)。其中坐标 $x, y$ 进行了归一化缩放（映射在 $[0.2, 1.0]$）。
*   **动作特征设计**：$\phi(s, a) = \text{one-hot}(a) \otimes \phi(s)$，维度大小为 $4 \times \phi(s)$。
*   **DQN 经验回放**：缓冲区容量 500，Batch Size = 32。
*   **DQN 目标网络**：每 50 步硬同步（将主网络权重赋予目标网络）。
