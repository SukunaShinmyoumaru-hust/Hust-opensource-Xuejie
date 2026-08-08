# 实验六：策略函数近似 (REINFORCE)

## 一、 实验概述
本实验探究无模型策略搜索（Policy Search）方法，直接参数化表达概率分布策略并在策略梯度空间进行寻优：
*   **Softmax 参数化策略**：使用线性状态特征 $\phi(s) = [1, x, y]^T$ 作为输入，动作参数向量 $\theta \in \mathbb{R}^{4 \times 3}$ 给出动作偏好偏导，利用 Softmax 函数提供始终为正的可导概率分布（保证探索连续性）。
*   **REINFORCE 算法**：利用蒙特卡洛（MC）采样生成整条回合轨迹，逆向折扣回溯更新所有步骤的动作似然对数梯度（$\nabla_\theta \ln \pi(a_t|s_t, \theta)$）。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个子文件：
```bash
python task1.py                 # 初始化策略偏好可视化
python task2.py                 # 运行原始 REINFORCE 训练
python task3.py                 # 运行起点状态动作概率演变分析
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `gridworld.py`：环境包装器。从项目根目录导入共享 `GridWorld` 环境。
*   `visualization.py`：可视化包装器。从项目根目录导入共享热力图与策略图接口。
*   `policy.py`：定义 Softmax 策略类，包含动作采样、折扣回报计算和策略梯度更新。
*   `task1.py`：可视化随机初始化偏好权重下特定点 $(3, 3)$ 的动作概率和整体概率流向。
*   `task2.py`：原始 REINFORCE 训练脚本。支持每 100 episodes 的平均表现评估并输出收敛曲线与策略。
*   `task3.py`：提取训练中起点 $(1,1)$ 四个动作在 1000 episodes 演变中的变化记录并绘制分析图。
*   `run_all.py`：主脚本，统一运行实验并保存图表。

### 2. 结果图表 (`results/`)
*   `task1_state_33_probs.png`：$(3, 3)$ 状态在随机初始化下各动作的选择概率（条形图）。
*   `task1_initial_policy.png`：初始未训练策略的动作箭头流动图。
*   `task2_reward_curve.png`：REINFORCE 收敛曲线，包含每回合的实际回报和 50 回合滑动平均线。
*   `task2_final_greedy_policy.png`：REINFORCE 学到的最大概率贪心策略图。
*   `task2_final_policy_probs.png`：最终策略在全图 25 个状态上的概率分布图（多箭头，透明度代表大小）。
*   `task3_start_state_probs.png`：起点 $(1,1)$ 的上、下、左、右四个动作选择概率随 Episode 演变而变化的平滑收敛图。

---

## 四、 实验关键设置参数

*   **学习率**：$\alpha = 0.001$。
*   **状态特征表示**：$\phi(s) = [1, x, y]^T$，一维坐标 $x, y \in \{1, 2, 3, 4, 5\}$。
*   **训练规模**：总 episodes = 1000，单回合最大步数 $T_{\text{max}} = 500$。
*   **衰减因子**：$\gamma = 0.9$。
