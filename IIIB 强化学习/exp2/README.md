# 实验二：蒙特卡罗方法

## 一、 实验概述
本实验实现了无模型（Model-Free）强化学习中的蒙特卡罗价值估计与控制算法，探究大数定律与探索机制：
*   **期望估计**：实现投币蒙特卡罗估值，验证估计误差按 $O(1/\sqrt{N})$ 速度衰减的大数定律。
*   **MC Basic**：在经典控制框架下直接进行动作价值估值，测试不同回合长度 $H$ 下的策略收敛上限。
*   **MC Exploring Starts**：基于随机起始状态-动作（Exploring Starts）的每次访问型（Every-visit）蒙特卡罗控制。
*   **MC ε-Greedy**：引入具有探索性的软策略（$\epsilon$-greedy），实现无需 Exploring Starts 条件约束的在线蒙特卡罗控制，并引入动态衰减的探索率 $\epsilon$ 优化性能。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个算法文件：
```bash
python coin_flip.py                  # 投币实验
python mc_basic.py                   # MC Basic 实验
python mc_exploring_starts.py        # MC ES 实验
python mc_epsilon_greedy.py          # MC ε-Greedy 实验
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `gridworld.py`：环境包装器。从项目根目录导入共享 `GridWorld` 环境。
*   `visualization.py`：可视化包装器。从项目根目录导入共享值热力图、策略图及学习曲线接口。
*   `coin_flip.py`：硬币投掷期望估计，生成误差对比数据。
*   `mc_basic.py`：MC Basic 算法实现，测试并生成 $H \in \{1, 2, 3, 4, 5, 14, 15, 30, 100\}$ 的估计数据。
*   `mc_exploring_starts.py`：具有随机起点的回溯每次访问蒙特卡罗控制算法。
*   `mc_epsilon_greedy.py`：具有固定 $\epsilon$ 及指数衰减 $\epsilon$ 机制的蒙特卡罗控制。
*   `run_all.py`：主入口脚本，运行以上全部子实验并在同坐标系下绘制三算法收敛性横向对比图。

### 2. 结果图表 (`results/`)
*   `coin_flip_error.png`：投币估计误差随样本数变化的双对数收敛图。
*   `mc_basic_value_H*.png` / `mc_basic_policy_H*.png`：不同 $H$ 值下估计的值热力图与指向策略图。
*   `mc_es_value.png` / `mc_es_policy.png` / `mc_es_rmse.png`：MC ES 算法最终收敛的状态价值、完美绕行策略与 RMSE 收敛曲线。
*   `mc_eps_value_eps*.png` / `mc_eps_policy_eps*.png`：不同固定 $\epsilon$ 和动态衰减策略下的收敛矩阵和绕行策略。
*   `mc_comparison_rmse.png` / `mc_comparison_rewards.png`：MC Basic、MC ES 与 动态 ε-Greedy 的 RMSE 降速与测试回报样本效率横向对比图。

---

## 四、 环境关键参数配置

| 参数 | 任务 1 (MC Basic) | 任务 2 (MC ES & ε-Greedy) |
|---|:---:|:---:|
| 网格大小 | $5 \times 5$ | $5 \times 5$ |
| 目标位置 | (4, 4) 右下角 | (4, 4) 右下角 |
| 禁止区域 | (1, 1), (2, 3) | (1, 1), (2, 3) |
| 边界惩罚 $r_\text{boundary}$ | -1.0 | -1.0 |
| 禁止区奖励 $r_\text{forbidden}$ | **-1.0** | **-10.0** |
| 目标奖励 $r_\text{target}$ | +1.0 | +1.0 |
| 普通移动奖励 $r_\text{step}$ | 0.0 | 0.0 |
| 折扣因子 $\gamma$ | 0.9 | 0.9 |
