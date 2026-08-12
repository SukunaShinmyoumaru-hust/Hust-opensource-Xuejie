"""
任务 3：截断策略迭代算法实现与三者对比
========================================
实现截断策略迭代，并与值迭代和策略迭代进行性能对比。
"""

import numpy as np
import time
from gridworld import GridWorld, NUM_ACTIONS
from visualization import (
    plot_value_heatmap, plot_policy_arrows,
    plot_convergence_curve, plot_comparison
)


def truncated_policy_evaluation(env: GridWorld, policy: np.ndarray,
                                 j_truncate: int, V_init: np.ndarray = None):
    """截断策略评价：只迭代 j_truncate 步。

    Parameters
    ----------
    env : GridWorld
    policy : np.ndarray
    j_truncate : int
        截断次数。如果 <= 0 或非常大，则视为完全收敛（策略迭代）。
    V_init : np.ndarray, optional

    Returns
    -------
    V : np.ndarray
    actual_steps : int
    """
    V = V_init.copy() if V_init is not None else np.zeros(env.num_states)

    # 完全收敛模式
    if j_truncate <= 0 or j_truncate > 10000:
        theta = 1e-6
        for it in range(1, 100000):
            V_old = V.copy()
            for s in env.get_non_terminal_states():
                a = int(policy[s])
                v_new = 0.0
                for prob, next_s, reward in env.get_transitions(s, a):
                    v_new += prob * (reward + env.gamma * V_old[next_s])
                V[s] = v_new
            if np.max(np.abs(V - V_old)) < theta:
                return V, it
        return V, 100000

    # 截断模式
    for it in range(j_truncate):
        V_old = V.copy()
        for s in env.get_non_terminal_states():
            a = int(policy[s])
            v_new = 0.0
            for prob, next_s, reward in env.get_transitions(s, a):
                v_new += prob * (reward + env.gamma * V_old[next_s])
            V[s] = v_new

    return V, j_truncate


def truncated_policy_iteration(env: GridWorld, j_truncate: int,
                                theta: float = 1e-6, max_iter: int = 500,
                                verbose: bool = True):
    """截断策略迭代算法。

    Parameters
    ----------
    env : GridWorld
    j_truncate : int
        策略评价的截断次数。
        j_truncate=1 退化为值迭代。
        j_truncate 足够大（或 <=0）退化为策略迭代。
    theta : float
        外循环收敛阈值。
    max_iter : int
        最大外循环次数。
    verbose : bool

    Returns
    -------
    V, policy, errors, total_bellman_updates, elapsed_time
    """
    rng = np.random.RandomState(42)
    policy = rng.randint(0, env.num_actions, size=env.num_states)
    V = np.zeros(env.num_states)
    errors = []
    total_bellman_updates = 0

    t_start = time.time()

    for iteration in range(1, max_iter + 1):
        V_old = V.copy()

        # 截断策略评价
        V, pe_steps = truncated_policy_evaluation(env, policy, j_truncate, V_init=V)
        total_bellman_updates += pe_steps * len(env.get_non_terminal_states())

        # 策略改进
        Q = np.zeros((env.num_states, env.num_actions))
        for s in env.get_non_terminal_states():
            for a in range(env.num_actions):
                for prob, next_s, reward in env.get_transitions(s, a):
                    Q[s, a] += prob * (reward + env.gamma * V[next_s])
        total_bellman_updates += len(env.get_non_terminal_states()) * env.num_actions

        new_policy = np.argmax(Q, axis=1)

        delta = np.max(np.abs(V - V_old))
        errors.append(delta)

        stable = np.array_equal(policy, new_policy)
        policy = new_policy

        if verbose and (iteration <= 5 or iteration % 20 == 0 or delta < theta or stable):
            label = "∞" if j_truncate <= 0 or j_truncate > 10000 else str(j_truncate)
            print(f"  [j={label}] 迭代 {iteration:3d}: PE={pe_steps:3d}步, "
                  f"max|ΔV|={delta:.8f}")

        if delta < theta and stable:
            break

    elapsed = time.time() - t_start

    if verbose:
        label = "∞" if j_truncate <= 0 or j_truncate > 10000 else str(j_truncate)
        print(f"  [j={label}] 收敛: {len(errors)} 次外循环, "
              f"{total_bellman_updates} 次贝尔曼更新, {elapsed:.4f}s")

    return V, policy, errors, total_bellman_updates, elapsed


def run_task3(save_dir: str = "results"):
    """运行任务 3 的全部实验。"""
    import os
    import matplotlib.pyplot as plt
    os.makedirs(save_dir, exist_ok=True)

    print("\n" + "=" * 70)
    print("任务 3：截断策略迭代与三者对比")
    print("=" * 70)

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

    # ---- 不同截断次数对比 ----
    j_values = [1, 3, 5, 10, -1]  # -1 表示 ∞（完全收敛）
    j_labels = {1: "j=1 (值迭代)", 3: "j=3", 5: "j=5", 10: "j=10", -1: "j=∞ (策略迭代)"}

    results = {}
    stats = []

    for j in j_values:
        label = j_labels[j]
        print(f"\n--- {label} ---")
        V, policy, errors, bellman_updates, elapsed = truncated_policy_iteration(
            env, j_truncate=j, verbose=True
        )
        results[label] = errors
        stats.append({
            "算法": label,
            "外循环次数": len(errors),
            "总贝尔曼更新": bellman_updates,
            "耗时(s)": f"{elapsed:.4f}",
        })

        # 保存最终策略和值函数
        safe_name = label.replace(" ", "_").replace("(", "").replace(")", "").replace("=", "")
        plot_policy_arrows(policy, env, title=f"{label} — 最优策略", V=V,
                           save_path=os.path.join(save_dir, f"task3_policy_{safe_name}.png"))

    # ---- 打印对比表 ----
    print("\n--- 算法性能对比 ---")
    print(f"{'算法':<25s} {'外循环次数':>10s} {'总贝尔曼更新':>14s} {'耗时(s)':>10s}")
    print("-" * 65)
    for s in stats:
        print(f"{s['算法']:<25s} {s['外循环次数']:>10d} {s['总贝尔曼更新']:>14d} {s['耗时(s)']:>10s}")

    # ---- 收敛曲线对比 ----
    plot_comparison(results, title="截断策略迭代 — 不同截断次数的收敛对比",
                    xlabel="外循环迭代次数", ylabel="max|ΔV|",
                    save_path=os.path.join(save_dir, "task3_convergence_comparison.png"))

    # ---- 计算效率对比（柱状图） ----
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))

    names = [s["算法"] for s in stats]
    outer_iters = [s["外循环次数"] for s in stats]
    bellman = [s["总贝尔曼更新"] for s in stats]

    colors = ['#e74c3c', '#3498db', '#2ecc71', '#f39c12', '#9b59b6']
    short_names = ["j=1", "j=3", "j=5", "j=10", "j=∞"]

    axes[0].bar(short_names, outer_iters, color=colors)
    axes[0].set_ylabel("外循环迭代次数")
    axes[0].set_title("外循环迭代次数对比")
    for i, v in enumerate(outer_iters):
        axes[0].text(i, v + 0.5, str(v), ha='center', fontweight='bold')

    axes[1].bar(short_names, bellman, color=colors)
    axes[1].set_ylabel("总贝尔曼更新次数")
    axes[1].set_title("计算量对比")
    for i, v in enumerate(bellman):
        axes[1].text(i, v + max(bellman) * 0.01, str(v), ha='center', fontweight='bold',
                     fontsize=8)

    plt.suptitle("截断策略迭代 — 计算效率对比", fontsize=14)
    plt.tight_layout()
    plt.savefig(os.path.join(save_dir, "task3_efficiency_comparison.png"),
                dpi=150, bbox_inches='tight')
    print(f"[保存] {os.path.join(save_dir, 'task3_efficiency_comparison.png')}")
    plt.show()

    return results, stats


if __name__ == "__main__":
    run_task3()
