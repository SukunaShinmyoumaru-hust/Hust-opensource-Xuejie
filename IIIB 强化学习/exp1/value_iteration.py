"""
任务 1：值迭代算法实现与分析
==============================
在 5×5 GridWorld 上实现值迭代算法，输出 Q-table、收敛曲线和策略图。
"""

import numpy as np
from gridworld import GridWorld, NUM_ACTIONS
from visualization import (
    plot_value_heatmap, plot_policy_arrows,
    plot_convergence_curve, print_q_table
)


def value_iteration(env: GridWorld, theta: float = 1e-6, max_iter: int = 1000,
                    V_init: np.ndarray = None, verbose: bool = True):
    """值迭代算法。

    Parameters
    ----------
    env : GridWorld
        网格世界环境。
    theta : float
        收敛阈值，当 max|V_new - V_old| < theta 时停止。
    max_iter : int
        最大迭代次数。
    V_init : np.ndarray, optional
        初始值函数。如果为 None，则初始化为全零。
    verbose : bool
        是否打印详细信息。

    Returns
    -------
    V : np.ndarray, shape (num_states,)
        最终值函数。
    policy : np.ndarray, shape (num_states,)
        最优策略（每个状态选择的动作索引）。
    Q : np.ndarray, shape (num_states, num_actions)
        最终 Q-table。
    errors : list of float
        每次迭代的最大误差。
    Q_history : list of np.ndarray
        前若干次迭代的 Q-table（用于验证）。
    V_history : list of np.ndarray
        每次迭代的值函数。
    policy_history : list of np.ndarray
        每次迭代的策略。
    """
    n_states = env.num_states
    n_actions = env.num_actions

    # 初始化
    V = V_init.copy() if V_init is not None else np.zeros(n_states)
    errors = []
    Q_history = []
    V_history = [V.copy()]
    policy_history = []

    for iteration in range(1, max_iter + 1):
        V_old = V.copy()
        Q = np.zeros((n_states, n_actions))

        for s in env.get_non_terminal_states():
            for a in range(n_actions):
                for prob, next_s, reward in env.get_transitions(s, a):
                    Q[s, a] += prob * (reward + env.gamma * V_old[next_s])

        # 值更新
        V = np.max(Q, axis=1)
        # 终止状态值为 0
        for s in range(n_states):
            if env.is_terminal(s):
                V[s] = 0.0

        # 策略提取
        policy = np.argmax(Q, axis=1)

        # 记录误差
        delta = np.max(np.abs(V - V_old))
        errors.append(delta)

        # 保存前 3 次迭代的 Q-table
        if iteration <= 3:
            Q_history.append(Q.copy())

        V_history.append(V.copy())
        policy_history.append(policy.copy())

        if verbose and (iteration <= 5 or iteration % 10 == 0):
            print(f"  迭代 {iteration:3d}: max|ΔV| = {delta:.8f}")

        if delta < theta:
            if verbose:
                print(f"\n✓ 值迭代在第 {iteration} 次迭代收敛 (max|ΔV| = {delta:.2e} < {theta})")
            break
    else:
        if verbose:
            print(f"\n⚠ 达到最大迭代次数 {max_iter}，未完全收敛")

    return V, policy, Q, errors, Q_history, V_history, policy_history


def run_task1(save_dir: str = "results"):
    """运行任务 1 的全部实验。"""
    import os
    os.makedirs(save_dir, exist_ok=True)

    print("=" * 70)
    print("任务 1：值迭代算法实现与分析")
    print("=" * 70)

    # ---- 环境配置 ----
    env = GridWorld(
        size=5,
        goal=(4, 4),
        forbidden_states=[(1, 1), (2, 3)],
        r_boundary=-1.0,
        r_forbidden=-1.0,
        r_target=1.0,
        r_step=0.0,
        gamma=0.9,
    )
    print(f"\n环境: {env}")

    # ---- 运行值迭代 ----
    print("\n--- 值迭代 (V_init = 0) ---")
    V, policy, Q, errors, Q_history, V_history, policy_history = value_iteration(env)

    # ---- 打印前 3 次迭代 Q-table ----
    for i, Q_i in enumerate(Q_history):
        print_q_table(Q_i, env, iteration=i + 1)

    # ---- 最终值函数和策略 ----
    print("\n--- 最终值函数矩阵 ---")
    print(V.reshape(env.size, env.size).round(4))

    # ---- 可视化 ----
    plot_value_heatmap(V, env, title="值迭代 — 最终状态值函数",
                       save_path=os.path.join(save_dir, "task1_value_heatmap.png"))
    plot_policy_arrows(policy, env, title="值迭代 — 最优策略", V=V,
                       save_path=os.path.join(save_dir, "task1_policy.png"))
    plot_convergence_curve(errors, title="值迭代 — 收敛曲线",
                           save_path=os.path.join(save_dir, "task1_convergence.png"))

    # ---- 不同初始值的对比 ----
    print("\n--- 不同初始值函数的对比 ---")
    init_configs = {
        "V=0": np.zeros(env.num_states),
        "V=随机": np.random.RandomState(42).randn(env.num_states),
        "V=乐观(+5)": np.full(env.num_states, 5.0),
    }
    comparison = {}
    for name, V_init in init_configs.items():
        print(f"\n  [{name}]:")
        _, _, _, errs, _, _, _ = value_iteration(env, V_init=V_init, verbose=False)
        comparison[name] = errs
        print(f"    收敛迭代次数: {len(errs)}")

    from visualization import plot_comparison
    plot_comparison(comparison, title="值迭代 — 不同初始值函数的收敛对比",
                    save_path=os.path.join(save_dir, "task1_init_comparison.png"))

    # ---- 2×2 小网格验证 ----
    print("\n--- 2×2 网格验证（前 3 次迭代） ---")
    env2 = GridWorld(size=2, goal=(1, 1), forbidden_states=[],
                     r_boundary=-1.0, r_forbidden=-1.0, r_target=1.0,
                     r_step=0.0, gamma=0.9)
    _, _, _, _, Q_hist_2x2, _, _ = value_iteration(env2, verbose=True)
    for i, Q_i in enumerate(Q_hist_2x2):
        print_q_table(Q_i, env2, iteration=i + 1)

    return V, policy, Q, errors


if __name__ == "__main__":
    run_task1()
