# 实验七：Actor-Critic 算法

## 一、 实验概述
本实验探究 Actor-Critic 混合学习架构，实现策略搜索与价值评估的有机结合，并分析基线与方差控制：
*   **Q 学习 Actor-Critic (QAC)**：策略更新由 Critic 估计的单步 TD 动作值 $q(s,a,w)$ 驱动，Critic 本身使用 SARSA 一阶时序差分进行参数更新。
*   **优势 Actor-Critic (A2C)**：引入状态价值函数基线 $v(s,w)$，将 TD 误差 $\delta_t = r + \gamma v(s') - v(s)$ 近似表达优势函数 $A(s,a)$，利用单步优势更新策略参数，极大地削弱了梯度估计的方差。
*   **重要性采样异策略 AC**：使用带有 $\epsilon=0.3$ 的偏置行为策略产生 5000 步的固定数据集，通过重要性权重修正更新概率，测试离线数据的高效再利用。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个子文件：
```bash
python task1_qac.py                 # QAC 实验
python task2_a2c.py                 # A2C 10次独立方差对照实验
python task3_off_policy.py          # 异策略 AC 离线数据实验
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `gridworld.py`：环境包装器。从项目根目录导入共享 `GridWorld` 环境并重写为专用无禁区参数类。
*   `visualization.py`：可视化包装器。从项目根目录导入共享值热力图、策略图及对比学习曲线接口。
*   `task1_qac.py`：QAC 算法的具体运行，支持动作价值 Critic 和偏好 Actor 更新。
*   `task2_a2c.py`：A2C 算法及方差定量测试。运行 10 次独立随机种子，统计 QAC 与 A2C 在最后 100 episodes 中的均值及标准差。
*   `task3_off_policy.py`：异策略 IS 修正 AC 算法，包含 5000 步探索轨迹采集以及目标策略评估。
*   `run_all.py`：主脚本，统一运行实验并保存图表。

### 2. 结果图表 (`results/`)
*   `task1_qac_learning.png`：QAC 算法的回报收敛图。
*   `task1_qac_values.png` / `task1_qac_policy.png`：QAC 估算出的价值热力图与最优路径。
*   `task2_qac_vs_a2c.png`：QAC（无基线）与 A2C（状态值基线）的均值对比线及标准差阴影区域对比图。
*   `task2_a2c_values.png` / `task2_a2c_policy.png`：A2C 生成的值函数及完美避障策略。
*   `task3_off_policy_learning.png`：异策略离线评估奖励的增长收敛曲线。
*   `task3_behavior_policy.png` / `task3_target_policy.png`：含高随机度的行为采样策略与学习到的纯贪心目标路径策略对比图。

---

## 四、 实验关键设置参数

*   **策略网络 (Actor) 学习率**：$\alpha_\theta = 0.001$。
*   **价值网络 (Critic) 学习率**：$\alpha_w = 0.01$。
*   **独立实验次数**：对比方差时, `num_runs = 10`。
*   **状态特征向量**：$\phi(s) = [1, x, y]^T$。
*   **重要性采样截断值**：$\rho_t \leftarrow \min(\rho_t, 10.0)$，保证梯度不会在训练中产生数值爆炸。
