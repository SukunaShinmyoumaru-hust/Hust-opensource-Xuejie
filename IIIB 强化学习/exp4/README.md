# 实验四：时序差分学习

## 一、 实验概述
本实验探究无模型单步更新的时序差分（Temporal Difference）评估与控制，分析偏差、方差与风险规避机制：
*   **TD(0) 评估**：在随机移动策略下，使用一阶 TD 误差估计状态价值，并同 First-Visit Monte Carlo 进行收敛速度与 RMSE 对比。
*   **Sarsa 控制**：实现同策略（On-policy）动作价值估计控制。支持多步 $n$-step Sarsa 算法，在 TD (低方差、高偏差) 与 MC (高方差、无偏) 之间通过 $n \in \{1, 3, 5\}$ 进行合理折中。
*   **Q-learning 控制**：实现异策略（Off-policy）TD 控制，使用贪心策略计算目标值并使用 $\epsilon$-greedy 采集数据。
*   **安全性路径对比**：在 **Cliff Walking（悬崖寻路）** 任务中对比 Sarsa 与 Q-learning 的最终行为，分析最优性与安全防御性策略的选择差异。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个子文件：
```bash
python dp_benchmark.py           # 计算 DP 基准值
python task2_td0_eval.py         # TD(0) 评估与 MC 对比
python task3_sarsa.py            # Sarsa 与 n-step Sarsa 实验
python task4_qlearning.py        # Q-learning 在线、离线与悬崖路径对比
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `env.py`：环境包装器。从项目根目录导入 Gym 风格的共享网格世界 `GymGridWorld` 与 `CliffWalking` 环境。
*   `utils.py`：可视化包装器。从项目根目录导入共享值热力图、悬崖世界热力图与策略图接口。
*   `dp_benchmark.py`：使用动态规划计算随机策略的精确状态值，保存基准值。
*   `task2_td0_eval.py`：运行 20 次独立实验，对比 TD(0) 和 FV-MC 在 $\alpha \in \{0.05, 0.1, 1/N(s)\}$ 下的状态评估 RMSE。
*   `task3_sarsa.py`：$n$-step Sarsa 动作控制，测试 $n=1$ (普通 Sarsa)、$n=3$、$n=5$ 的学习效果。
*   `task4_qlearning.py`：同/异策略 Q-learning 实验（在线更新，在 50000 离线大样本下的更新，以及悬崖寻路下的测试对比）。
*   `run_all.py`：自动依次调用执行以上所有文件，并存储生成的图片到 `results/`。

### 2. 结果图表 (`results/`)
*   `gridworld_schematic.png`：5x5 网格世界节点与坐标映射结构示意图。
*   `dp_benchmark_heatmap.png`：用于 RMSE 对照的动态规划真值热力图。
*   `rmse_comparison.png`：TD(0) 各种步长与 MC 在 20 次独立运行下的 RMSE 下降对照曲线。
*   `task3_rewards.png` / `task3_steps.png` / `task3_success_rate.png`：$n$-step Sarsa 各参数在收敛过程中的回报、消耗步数与到达率对比。
*   `task3_policy_n*.png`：Sarsa 学到的最终动作贪心方向指向图。
*   `task4_grid_rewards.png` / `task4_grid_steps.png`：在线 Sarsa 与 Q-learning 算法表现对比。
*   `task4_offline_q_policy.png`：异策略离线大样本下学习出的最优路径。
*   `task4_cliff_rewards.png` / `task4_cliff_q_policy.png` / `task4_cliff_sarsa_policy.png`：悬崖环境下的奖励回报对比，以及 Q-learning 选择贴边危险最短路径、Sarsa 选择绕远安全长路径的现象可视化。

---

## 四、 环境关键参数配置

| 参数 | 任务 1、2、3 (网格环境) | 任务 4 选做 (悬崖环境) |
|---|:---:|:---:|
| 环境大小 | $5 \times 5$ 网格 | $4 \times 12$ 网格 |
| 起点位置 | (0, 0) 左上角 | (3, 0) 左下角 |
| 终点位置 | (4, 4) 右下角 | (3, 11) 右下角 |
| 悬崖位置 | 无 | 行坐标 3, 列坐标 $1 \sim 10$ |
| 坠崖惩罚 | 无 | -100.0 (且强制送回起点) |
| 边界碰撞 / 移动奖励 | -1.0 | -1.0 |
| 折扣因子 $\gamma$ | 0.9 | 0.9 |
