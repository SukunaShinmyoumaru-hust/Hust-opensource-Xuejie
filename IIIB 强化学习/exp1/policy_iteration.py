"""
任务 2：策略迭代算法实现与分析
================================
在 5×5 GridWorld 上实现策略迭代算法，分析策略演变和收敛特性。
"""

import numpy as np
from gridworld import GridWorld, NUM_ACTIONS
from visualization import (
    plot_value_heatmap, plot_policy_arrows,
    plot_convergence_curve, plot_policy_evolution, print_q_table
)


def policy_evaluation(env: GridWorld, policy: np.ndarray,
                      theta: float = 1e-6, max_iter: int = 1000,
                      V_init: np.ndarray = None) -> np.ndarray:
    """策略评价：给定策略 π，迭代求解贝尔曼方程。

    Parameters
    ----------
    env : GridWorld
    policy : np.ndarray, shape (num_states,)
        当前策略。
    theta : float
        收敛阈值。
    max_iter : int
        最大迭代次数。
    V_init : np.ndarray, optional
        初始值函数。

    Returns
    -------
    V : np.ndarray, shape (num_states,)
        策略 π 下的状态值函数。
    pe_iters : int
        策略评价的迭代次数。
    """
    V = V_init.copy() if V_init is not None else np.zeros(env.num_states)

    for it in range(1, max_iter + 1):
        V_old = V.copy()
        for s in env.get_non_terminal_states():
            a = int(policy[s])
            v_new = 0.0
            for prob, next_s, reward in env.get_transitions(s, a):
                v_new += prob * (reward + env.gamma * V_old[next_s])
            V[s] = v_new

        delta = np.max(np.abs(V - V_old))
        if delta < theta:
            return V, it

    return V, max_iter


def policy_improvement(env: GridWorld, V: np.ndarray):
    """策略改进：根据 V 计算 Q 值，取贪心策略。

    Returns
    -------
    new_policy : np.ndarray, shape (num_states,)
    Q : np.ndarray, shape (num_states, num_actions)
    stable : bool
        策略是否稳定（未发生变化）。
    """
    Q = np.zeros((env.num_states, env.num_actions))
    for s in env.get_non_terminal_states():
        for a in range(env.num_actions):
            for prob, next_s, reward in env.get_transitions(s, a):
                Q[s, a] += prob * (reward + env.gamma * V[next_s])

    new_policy = np.argmax(Q, axis=1)
    return new_policy, Q


def policy_iteration(env: GridWorld, theta: float = 1e-6, max_iter: int = 100,
                     verbose: bool = True):
    """策略迭代算法。

    Parameters
    ----------
    env : GridWorld
    theta : float
        策略评价的收敛阈值。
    max_iter : int
        最大外循环迭代次数。
    verbose : bool

    Returns
    -------
    V : np.ndarray
    policy : np.ndarray
    Q : np.ndarray
    errors : list of float
        每次外循环的最大值函数变化。
    policy_history : list of np.ndarray
    V_history : list of np.ndarray
    pe_iterations : list of int
        每次策略评价的迭代次数。
    """
    # 初始化随机策略
    rng = np.random.RandomState(42)
    policy = rng.randint(0, env.num_actions, size=env.num_states)
    V = np.zeros(env.num_states)

    errors = []
    policy_history = [policy.copy()]
    V_history = [V.copy()]
    pe_iterations = []

    for iteration in range(1, max_iter + 1):
        V_old = V.copy()

        # 策略评价
        V, pe_iters = policy_evaluation(env, policy, theta=theta, V_init=V)
        pe_iterations.append(pe_iters)

        # 策略改进
        new_policy, Q = policy_improvement(env, V)

        # 记录
        delta = np.max(np.abs(V - V_old))
        errors.append(delta)
        V_history.append(V.copy())
        policy_history.append(new_policy.copy())

        stable = np.array_equal(policy, new_policy)

        if verbose:
            print(f"  迭代 {iteration:3d}: 策略评价 {pe_iters:3d} 步, "
                  f"max|ΔV| = {delta:.8f}, 策略{'稳定 ✓' if stable else '更新'}")

        policy = new_policy

        if stable:
            if verbose:
                print(f"\n✓ 策略迭代在第 {iteration} 次迭代收敛（策略稳定）")
            break
    else:
        if verbose:
            print(f"\n⚠ 达到最大迭代次数 {max_iter}")

    return V, policy, Q, errors, policy_history, V_history, pe_iterations


def analyze_convergence_by_distance(env: GridWorld, V_history: list):
    """分析不同距离的状态收敛速度。"""
    import matplotlib.pyplot as plt

    goal_row, goal_col = env.goal
    distances = {}
    for s in env.get_non_terminal_states():
        r, c = env.state_to_pos(s)
        d = abs(r - goal_row) + abs(c - goal_col)  # 曼哈顿距离
        if d not in distances:
            distances[d] = []
        distances[d].append(s)

    fig, ax = plt.subplots(figsize=(10, 6))
    for d in sorted(distances.keys()):
        states = distances[d]
        # 计算每次迭代中这些状态的平均值变化
        changes = []
        for i in range(1, len(V_history)):
            change = np.mean(np.abs(V_history[i][states] - V_history[i - 1][states]))
            changes.append(change)
        ax.plot(range(1, len(changes) + 1), changes, 'o-', markersize=3,
                label=f"距离={d}")

    ax.set_xlabel("外循环迭代次数", fontsize=11)
    ax.set_ylabel("平均|ΔV|", fontsize=11)
    ax.set_title("策略迭代 — 不同距离状态的收敛速度", fontsize=13)
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_yscale('log')
    plt.tight_layout()
    return fig


def run_task2(save_dir: str = "results"):
    """运行任务 2 的全部实验。"""
    import os
    os.makedirs(save_dir, exist_ok=True)

    print("\n" + "=" * 70)
    print("任务 2：策略迭代算法实现与分析")
    print("=" * 70)

    # ---- 环境配置（注意 r_forbidden = -10） ----
    env = GridWorld(
        size=5,
        goal=(4, 4),
        forbidden_states=[(1, 1), (2, 3)],
        r_boundary=-1.0,
        r_forbidden=-10.0,
        r_target=1.0,
        r_step=0.0,
        gamma=0.9,
    )
    print(f"\n环境: {env}")

    # ---- 运行策略迭代 ----
    print("\n--- 策略迭代 ---")
    V, policy, Q, errors, policy_history, V_history, pe_iters = policy_iteration(env)

    # ---- 最终值函数和策略 ----
    print("\n--- 最终值函数矩阵 ---")
    print(V.reshape(env.size, env.size).round(4))
    print(f"\n策略评价平均迭代次数: {np.mean(pe_iters):.1f}")

    # ---- 可视化 ----
    plot_value_heatmap(V, env, title="策略迭代 — 最终状态值函数",
                       save_path=os.path.join(save_dir, "task2_value_heatmap.png"))
    plot_policy_arrows(policy, env, title="策略迭代 — 最优策略", V=V,
                       save_path=os.path.join(save_dir, "task2_policy.png"))
    plot_convergence_curve(errors, title="策略迭代 — 收敛曲线",
                           ylabel="max|ΔV|（外循环）",
                           save_path=os.path.join(save_dir, "task2_convergence.png"))

    # ---- 策略演变图 ----
    # 选取部分迭代展示
    show_indices = list(range(min(8, len(policy_history))))
    show_policies = [policy_history[i] for i in show_indices]
    show_V = [V_history[i] for i in show_indices]
    show_titles = [f"迭代 {i}" for i in show_indices]
    plot_policy_evolution(show_policies, env, titles=show_titles, V_list=show_V,
                          save_path=os.path.join(save_dir, "task2_policy_evolution.png"))

    # ---- 距离分析 ----
    fig = analyze_convergence_by_distance(env, V_history)
    fig.savefig(os.path.join(save_dir, "task2_distance_analysis.png"),
                dpi=150, bbox_inches='tight')
    print(f"[保存] {os.path.join(save_dir, 'task2_distance_analysis.png')}")
    import matplotlib.pyplot as plt
    plt.show()

    return V, policy, Q, errors


if __name__ == "__main__":
    run_task2()
