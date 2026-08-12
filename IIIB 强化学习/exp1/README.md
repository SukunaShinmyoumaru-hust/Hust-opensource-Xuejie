# 实验一：值迭代和策略迭代

## 一、 实验概述
本实验在 $5 \times 5$ 网格世界中实现了三种基于模型的动态规划（Dynamic Programming）算法：
*   **值迭代（Value Iteration）**：利用贝尔曼最优方程进行状态值函数的迭代更新与最优策略提取。
*   **策略迭代（Policy Iteration）**：通过策略评价（解贝尔曼期望方程）和策略改进两个核心步骤交替迭代收敛至最优策略。
*   **截断策略迭代（Truncated Policy Iteration）**：泛化了值迭代与策略迭代，允许在策略评价步骤设置截断评估次数 $j$，折中收敛速度与单步计算成本。

---

## 二、 快速运行
进入当前目录后，运行一键执行脚本：
```bash
python run_all.py
```
或者单独运行各个算法文件：
```bash
python value_iteration.py            # 任务 1
python policy_iteration.py           # 任务 2
python truncated_policy_iteration.py    # 任务 3
```

---

## 三、 文件结构说明

### 1. 源代码文件
*   `gridworld.py`：环境包装器。从项目根目录 [gridworld.py](../gridworld.py) 导入共享 `GridWorld` 环境。
*   `visualization.py`：可视化包装器。从项目根目录 [visualization.py](../visualization.py) 导入共享热力图、策略图及收敛曲线绘制接口。
*   `value_iteration.py`：值迭代算法的实现，支持前 3 次迭代 Q 表打印、不同初始值收敛性对比与 2x2 验证。
*   `policy_iteration.py`：策略迭代算法的实现，支持策略逐步演变可视化及近目标状态收敛速度分析。
*   `truncated_policy_iteration.py`：截断策略迭代算法的实现，支持 $j \in \{1, 3, 5, 10, \infty\}$ 性能与效率的横向量化分析。
*   `run_all.py`：主入口脚本，依次运行上述任务并生成所有图表到 `results/`。

### 2. 结果图表 (`results/`)
*   `task1_value_heatmap.png`：值迭代最终状态值函数热力图（标有目标 ★ 与禁区 ✕）。
*   `task1_policy.png`：值迭代最终最优策略箭头指示图。
*   `task1_convergence.png`：值迭代在三种不同初始值（全零、随机、乐观）下的收敛误差曲线。
*   `task2_value_heatmap.png` / `task2_policy.png`：策略迭代最终价值热力图与最优策略图。
*   `task2_policy_evolution.png`：展示策略迭代从初始随机到最终最优的演化过程多子图。
*   `task2_distance_analysis.png`：按起点与终点曼哈顿距离分组的收敛曲线（验证“靠近目标的状态更早收敛”）。
*   `task3_convergence_comparison.png`：不同截断次数下外循环收敛曲线对比图。
*   `task3_efficiency_comparison.png`：各截断参数下的总贝尔曼更新次数与时间开销对比柱状图。

---

## 四、 环境关键参数配置

| 参数 | 任务 1 (值迭代) | 任务 2 & 3 (策略迭代) |
|---|:---:|:---:|
| 网格大小 | $5 \times 5$ | $5 \times 5$ |
| 目标位置 | (4, 4) 右下角 | (4, 4) 右下角 |
| 禁止区域 | (1, 1), (2, 3) | (1, 1), (2, 3) |
| 边界惩罚 $r_\text{boundary}$ | -1.0 | -1.0 |
| 禁止区奖励 $r_\text{forbidden}$ | **-1.0** | **-10.0** |
| 目标奖励 $r_\text{target}$ | +1.0 | +1.0 |
| 普通移动奖励 $r_\text{step}$ | 0.0 | 0.0 |
| 折扣因子 $\gamma$ | 0.9 | 0.9 |
