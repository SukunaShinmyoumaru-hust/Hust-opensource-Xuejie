"""
任务 2：MC Exploring Starts 算法实现
====================================
实现 MC Exploring Starts (MC-ES) 算法，并在 5×5 网格世界中进行测试。
采用每次访问 (every-visit) 采样，回溯方式计算回报。
"""

import os
import numpy as np
from gridworld import GridWorld, NUM_ACTIONS
from visualization import plot_value_heatmap, plot_policy_arrows, plot_learning_curves


def run_value_iteration_ground_truth(env: GridWorld, theta: float = 1e-6):
    """通过动态规划值迭代求解真实最优值函数 V* 和 Q*，用于计算 RMSE。"""
    n_states = env.num_states
    n_actions = env.num_actions
    V = np.zeros(n_states)
    
    while True:
        V_old = V.copy()
        Q = np.zeros((n_states, n_actions))
        for s in env.get_non_terminal_states():
            for a in range(n_actions):
                next_s, r = env.step(s, a)
                Q[s, a] = r + env.gamma * V_old[next_s]
        V = np.max(Q, axis=1)
        for s in range(n_states):
            if env.is_terminal(s):
                V[s] = 0.0
        if np.max(np.abs(V - V_old)) < theta:
            break
            
    # 计算 Q*
    Q_star = np.zeros((n_states, n_actions))
    for s in env.get_non_terminal_states():
        for a in range(n_actions):
            next_s, r = env.step(s, a)
            Q_star[s, a] = r + env.gamma * V[next_s]
            
    return V, Q_star


def generate_es_episode(env: GridWorld, policy: np.ndarray, max_steps: int = 100) -> list:
    """生成一个满足 Exploring Starts 条件的回合。
    
    1. 随机选择起点状态 s0 和动作 a0 (Exploring Start)
    2. 之后根据 policy 选择动作，直到终止状态或达到 max_steps。
    返回轨迹：[(s0, a0, r1), (s1, a1, r2), ...]
    """
    non_terminal_states = env.get_non_terminal_states()
    s0 = np.random.choice(non_terminal_states)
    a0 = np.random.randint(0, env.num_actions)
    
    trajectory = []
    
    # 执行第一步
    next_s, r = env.step(s0, a0)
    trajectory.append((s0, a0, r))
    s = next_s
    
    # 之后遵循当前策略
    steps = 1
    while not env.is_terminal(s) and steps < max_steps:
        a = int(policy[s])
        next_s, r = env.step(s, a)
        trajectory.append((s, a, r))
        s = next_s
        steps += 1
        
    return trajectory


def evaluate_policy_return(env: GridWorld, policy: np.ndarray, start_pos: tuple = (0, 0), max_steps: int = 100) -> float:
    """评估当前确定性策略的实际总折扣回报（从特定起点，如左上角）。"""
    s = env.pos_to_state(start_pos[0], start_pos[1])
    G = 0.0
    discount = 1.0
    steps = 0
    
    while not env.is_terminal(s) and steps < max_steps:
        a = int(policy[s])
        next_s, r = env.step(s, a)
        G += discount * r
        discount *= env.gamma
        s = next_s
        steps += 1
        
    return G


def mc_exploring_starts(env: GridWorld, num_episodes: int = 5000, seed: int = 42,
                        V_star: np.ndarray = None):
    """MC Exploring Starts 算法。"""
    np.random.seed(seed)
    n_states = env.num_states
    n_actions = env.num_actions
    
    # 随机初始化策略
    policy = np.random.randint(0, n_actions, size=n_states)
    # 终止状态无需策略
    
    # 初始化 Q 和计数器 N
    Q = np.zeros((n_states, n_actions))
    N = np.zeros((n_states, n_actions))
    
    rmse_history = []
    eval_returns = []
    
    for ep in range(1, num_episodes + 1):
        # 1. 产生满足 Exploring Starts 的回合
        trajectory = generate_es_episode(env, policy)
        
        # 2. 每次访问蒙特卡罗评估 (every-visit)，从后往前计算
        G = 0.0
        # trajectory 的元素是 (s_t, a_t, r_{t+1})
        for s_t, a_t, r_tp1 in reversed(trajectory):
            G = r_tp1 + env.gamma * G
            
            # 更新估计值 (增量形式)
            N[s_t, a_t] += 1
            Q[s_t, a_t] += (1.0 / N[s_t, a_t]) * (G - Q[s_t, a_t])
            
        # 3. 策略改进 (对所有状态)
        for s in env.get_non_terminal_states():
            policy[s] = np.argmax(Q[s, :])
            
        # 记录评估与统计
        V = np.max(Q, axis=1)
        for s in range(n_states):
            if env.is_terminal(s):
                V[s] = 0.0
                
        if V_star is not None:
            rmse = np.sqrt(np.mean((V - V_star)**2))
            rmse_history.append(rmse)
            
        # 每 10 回合评估一次当前策略的性能（从左上角 (0,0) 开始的累计回报）
        if ep % 10 == 0:
            eval_ret = evaluate_policy_return(env, policy, start_pos=(0, 0))
            eval_returns.append(eval_ret)
            
    return V, policy, Q, rmse_history, eval_returns


def run_mc_es_experiment(save_dir: str = "results"):
    """运行 MC Exploring Starts 主实验。"""
    os.makedirs(save_dir, exist_ok=True)
    
    print("\n" + "=" * 60)
    print("任务 2：MC Exploring Starts 算法主实验")
    print("=" * 60)
    
    # 环境设置 (任务 2 要求 r_forbidden = -10)
    env = GridWorld(
        size=5,
        goal=(4, 4),
        forbidden_states=[(1, 1), (2, 3)],
        r_boundary=-1.0,
        r_forbidden=-10.0,  # 任务 2 惩罚加大
        r_target=1.0,
        r_step=0.0,
        gamma=0.9
    )
    
    # 计算 V* 真值
    V_star, Q_star = run_value_iteration_ground_truth(env)
    print("  计算得到的地平线最优 V* 矩阵:")
    print(V_star.reshape(env.size, env.size).round(2))
    
    num_episodes = 5000
    V, policy, Q, rmse_hist, eval_returns = mc_exploring_starts(env, num_episodes=num_episodes, V_star=V_star)
    
    print(f"\n  经过 {num_episodes} 回合后收敛:")
    print(V.reshape(env.size, env.size).round(2))
    print("  收敛策略:")
    print(policy.reshape(env.size, env.size))
    
    # 可视化并保存
    plot_value_heatmap(V, env, title="MC Exploring Starts 值函数",
                       save_path=os.path.join(save_dir, "mc_es_value.png"))
    plot_policy_arrows(policy, env, title="MC Exploring Starts 最终策略", V=V,
                       save_path=os.path.join(save_dir, "mc_es_policy.png"))
    
    # 绘制 RMSE 收敛曲线
    import matplotlib.pyplot as plt
    plt.figure(figsize=(8, 5))
    plt.plot(range(1, len(rmse_hist) + 1), rmse_hist, color="#e74c3c", linewidth=1.5)
    plt.xlabel("回合数 (Episodes)", fontsize=11)
    plt.ylabel("RMSE (与真值 V* 相比)", fontsize=11)
    plt.title("MC Exploring Starts 的 RMSE 收敛曲线", fontsize=13)
    plt.grid(True, alpha=0.3)
    plt.yscale("log")
    save_path = os.path.join(save_dir, "mc_es_rmse.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()
    
    # 绘制评估性能收敛曲线
    plt.figure(figsize=(8, 5))
    plt.plot(range(10, num_episodes + 1, 10), eval_returns, color="#3498db", linewidth=1.5)
    plt.xlabel("回合数 (Episodes)", fontsize=11)
    plt.ylabel("测试累计回报 (从左上角出发)", fontsize=11)
    plt.title("MC Exploring Starts 的策略性能收敛过程", fontsize=13)
    plt.grid(True, alpha=0.3)
    save_path = os.path.join(save_dir, "mc_es_eval_returns.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()


if __name__ == "__main__":
    run_mc_es_experiment()
