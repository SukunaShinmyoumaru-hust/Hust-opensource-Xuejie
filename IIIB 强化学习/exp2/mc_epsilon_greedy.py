"""
任务 2 (后半部分)：MC ε-Greedy 算法实现与对比
===============================================
实现 MC ε-Greedy 算法，测试不同 ε 值（0、0.1、0.2、0.5）以及动态衰减 ε 对算法收敛性、最优性的影响。
绘制学习曲线和状态值矩阵对比图，分析探索与利用的权衡。
"""

import os
import numpy as np
from gridworld import GridWorld, NUM_ACTIONS
from visualization import plot_value_heatmap, plot_policy_arrows, plot_learning_curves
from mc_exploring_starts import run_value_iteration_ground_truth, evaluate_policy_return


def generate_epsilon_greedy_episode(env: GridWorld, Q: np.ndarray, epsilon: float, max_steps: int = 100) -> list:
    """使用 ε-Greedy 策略在 GridWorld 中生成一个回合。
    
    起点从所有非终止状态中随机选择。
    """
    non_terminal_states = env.get_non_terminal_states()
    s = np.random.choice(non_terminal_states)
    
    trajectory = []
    steps = 0
    
    while not env.is_terminal(s) and steps < max_steps:
        # ε-greedy 动作选择
        if np.random.rand() < epsilon:
            a = np.random.randint(0, env.num_actions)
        else:
            # 取最大 Q 值的动作，如有多个则随机选
            q_vals = Q[s, :]
            a = np.random.choice(np.flatnonzero(q_vals == np.max(q_vals)))
            
        next_s, r = env.step(s, a)
        trajectory.append((s, a, r))
        s = next_s
        steps += 1
        
    return trajectory


def mc_epsilon_greedy(env: GridWorld, epsilon_mode: str = 'fixed', epsilon_val: float = 0.1,
                      num_episodes: int = 5000, seed: int = 42, V_star: np.ndarray = None):
    """MC ε-Greedy 算法。
    
    epsilon_mode: 'fixed' (固定值) 或 'decay' (动态衰减)
    """
    np.random.seed(seed)
    n_states = env.num_states
    n_actions = env.num_actions
    
    Q = np.zeros((n_states, n_actions))
    N = np.zeros((n_states, n_actions))
    
    rmse_history = []
    eval_returns = []
    
    # 动态衰减参数
    epsilon_start = 1.0
    epsilon_min = 0.01
    
    for ep in range(1, num_episodes + 1):
        # 1. 计算当前 ε
        if epsilon_mode == 'decay':
            # 线性衰减或指数衰减
            epsilon = max(epsilon_min, epsilon_start * (0.9992 ** ep))
        else:
            epsilon = epsilon_val
            
        # 2. 采样回合
        trajectory = generate_epsilon_greedy_episode(env, Q, epsilon)
        
        # 3. 每次访问蒙特卡罗评估 (every-visit)，从后往前计算
        G = 0.0
        for s_t, a_t, r_tp1 in reversed(trajectory):
            G = r_tp1 + env.gamma * G
            N[s_t, a_t] += 1
            Q[s_t, a_t] += (1.0 / N[s_t, a_t]) * (G - Q[s_t, a_t])
            
        # 记录评估与统计
        V = np.max(Q, axis=1)
        for s in range(n_states):
            if env.is_terminal(s):
                V[s] = 0.0
                
        if V_star is not None:
            rmse = np.sqrt(np.mean((V - V_star)**2))
            rmse_history.append(rmse)
            
        # 每 10 回合评估一次当前贪心策略的性能（从左上角 (0,0) 开始的累计回报）
        if ep % 10 == 0:
            # 提取贪心策略用于测试
            policy_greedy = np.argmax(Q, axis=1)
            eval_ret = evaluate_policy_return(env, policy_greedy, start_pos=(0, 0))
            eval_returns.append(eval_ret)
            
    # 提取最终贪心策略
    final_policy = np.argmax(Q, axis=1)
    return V, final_policy, Q, rmse_history, eval_returns


def run_mc_epsilon_greedy_experiments(save_dir: str = "results"):
    """测试不同 ε 值和衰减 ε 的表现，并绘制对比图。"""
    os.makedirs(save_dir, exist_ok=True)
    
    print("\n" + "=" * 60)
    print("任务 2：MC ε-Greedy 算法主实验")
    print("=" * 60)
    
    # 环境设置 (任务 2 要求 r_forbidden = -10)
    env = GridWorld(
        size=5,
        goal=(4, 4),
        forbidden_states=[(1, 1), (2, 3)],
        r_boundary=-1.0,
        r_forbidden=-10.0,
        r_target=1.0,
        r_step=0.0,
        gamma=0.9
    )
    
    # 计算真值 V*
    V_star, _ = run_value_iteration_ground_truth(env)
    
    epsilons = [0.0, 0.1, 0.2, 0.5]
    num_episodes = 5000
    
    results = {}
    rmse_results = {}
    
    # 1. 运行固定 ε 实验
    for eps in epsilons:
        print(f"\n--- 运行 MC ε-Greedy (ε = {eps}) ---")
        V, policy, Q, rmse_hist, eval_returns = mc_epsilon_greedy(
            env, epsilon_mode='fixed', epsilon_val=eps, num_episodes=num_episodes, V_star=V_star
        )
        
        # 记录
        label = f"ε = {eps}"
        results[label] = eval_returns
        rmse_results[label] = rmse_hist
        
        # 保存此 ε 下的状态值热力图和策略图
        plot_value_heatmap(V, env, title=f"MC ε-Greedy 值函数 ({label})",
                           save_path=os.path.join(save_dir, f"mc_eps_value_eps{eps}.png"))
        plot_policy_arrows(policy, env, title=f"MC ε-Greedy 最终策略 ({label})", V=V,
                           save_path=os.path.join(save_dir, f"mc_eps_policy_eps{eps}.png"))
        
    # 2. 运行动态衰减 ε 实验
    print(f"\n--- 运行 MC ε-Greedy (动态衰减 ε) ---")
    V_decay, policy_decay, Q_decay, rmse_decay, eval_returns_decay = mc_epsilon_greedy(
        env, epsilon_mode='decay', num_episodes=num_episodes, V_star=V_star
    )
    results["动态衰减 ε"] = eval_returns_decay
    rmse_results["动态衰减 ε"] = rmse_decay
    
    plot_value_heatmap(V_decay, env, title="MC ε-Greedy 值函数 (动态衰减 ε)",
                       save_path=os.path.join(save_dir, "mc_eps_value_decay.png"))
    plot_policy_arrows(policy_decay, env, title="MC ε-Greedy 最终策略 (动态衰减 ε)", V=V_decay,
                       save_path=os.path.join(save_dir, "mc_eps_policy_decay.png"))
    
    # 3. 绘制累计回报收敛对比图（学习曲线）
    # 将 eval_returns 转回原回合刻度（乘 10）
    import matplotlib.pyplot as plt
    plt.figure(figsize=(10, 6))
    colors = ['#95a5a6', '#3498db', '#e67e22', '#e74c3c', '#2ecc71']
    for i, (name, curves) in enumerate(results.items()):
        # 滑动平均平滑
        window = 20
        smoothed = np.convolve(curves, np.ones(window)/window, mode='valid')
        x = np.arange(window, len(curves) + 1) * 10
        plt.plot(x, smoothed, label=name, color=colors[i], linewidth=2)
        
    plt.xlabel("回合数 (Episodes)", fontsize=11)
    plt.ylabel("测试累计回报 (从左上角出发，滑动平均)", fontsize=11)
    plt.title("不同 ε 配置下 MC ε-Greedy 算法的学习曲线对比", fontsize=13)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    save_path = os.path.join(save_dir, "mc_eps_rewards_comparison.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()
    
    # 4. 绘制 RMSE 对比图
    plt.figure(figsize=(10, 6))
    for i, (name, curves) in enumerate(rmse_results.items()):
        plt.plot(range(1, len(curves) + 1), curves, label=name, color=colors[i], alpha=0.8)
    plt.xlabel("回合数 (Episodes)", fontsize=11)
    plt.ylabel("RMSE (与真值 V* 相比)", fontsize=11)
    plt.yscale("log")
    plt.title("不同 ε 配置下 MC ε-Greedy 的 RMSE 收敛速度对比", fontsize=13)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    save_path = os.path.join(save_dir, "mc_eps_rmse_comparison.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()


if __name__ == "__main__":
    run_mc_epsilon_greedy_experiments()
