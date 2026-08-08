"""
任务 1 (后半部分)：MC Basic 算法实现
===================================
在 5×5 网格世界中实现 MC Basic 算法。
测试不同回合长度 (Horizon H) 下的状态值函数与最终策略，分析其影响。
"""

import os
import numpy as np
from gridworld import GridWorld, NUM_ACTIONS
from visualization import plot_value_heatmap, plot_policy_arrows


def generate_mc_basic_episode(env: GridWorld, start_state: int, first_action: int,
                              policy: np.ndarray, horizon: int) -> float:
    """生成一个 MC Basic 回合，返回累计折扣回报。
    
    从 start_state 开始，第一步执行 first_action，后续步骤遵循 policy，直到终止状态或达到 horizon。
    """
    s = start_state
    a = first_action
    
    # 记录该回合的奖励序列
    rewards = []
    
    # 执行第一步
    next_s, r = env.step(s, a)
    rewards.append(r)
    s = next_s
    
    # 后续步骤遵循当前策略 policy
    for step in range(1, horizon):
        if env.is_terminal(s):
            break
        a_curr = int(policy[s])
        next_s, r = env.step(s, a_curr)
        rewards.append(r)
        s = next_s
        
    # 计算折扣累计回报 G
    G = 0.0
    for r_val in reversed(rewards):
        G = r_val + env.gamma * G
    return G


def mc_basic(env: GridWorld, horizon: int, max_policy_iters: int = 20, verbose: bool = True):
    """MC Basic 算法。"""
    n_states = env.num_states
    n_actions = env.num_actions
    
    # 初始策略：均匀随机或指向右 (这里初始化为全部动作 0)
    policy = np.zeros(n_states)
    
    # 初始化 Q-table
    Q = np.zeros((n_states, n_actions))
    
    policy_history = [policy.copy()]
    
    for iteration in range(1, max_policy_iters + 1):
        Q_new = np.zeros((n_states, n_actions))
        
        # 策略评估：对于每一个状态 s 和动作 a
        for s in env.get_non_terminal_states():
            for a in range(n_actions):
                # 采样一个回合（在确定性环境下，1次采样即为精确值）
                G = generate_mc_basic_episode(env, s, a, policy, horizon)
                Q_new[s, a] = G
                
        # 终止状态的值保持为 0
        for s in range(n_states):
            if env.is_terminal(s):
                Q_new[s, :] = 0.0
                
        # 策略改进
        new_policy = np.argmax(Q_new, axis=1)
        
        stable = np.array_equal(policy, new_policy)
        policy = new_policy.copy()
        Q = Q_new.copy()
        
        policy_history.append(policy.copy())
        
        if verbose:
            print(f"  [H={horizon}] 迭代 {iteration:2d}: 策略{'稳定 ✓' if stable else '更新'}")
            
        if stable:
            break
            
    # 计算最终值函数 V
    V = np.max(Q, axis=1)
    return V, policy, Q, iteration


def run_mc_basic_experiments(save_dir: str = "results"):
    """测试不同回合长度 (Horizon H) 下的收敛结果。"""
    os.makedirs(save_dir, exist_ok=True)
    
    print("\n" + "=" * 60)
    print("任务 1：MC Basic 算法主实验")
    print("=" * 60)
    
    # 环境设置 (任务 1 中 r_forbidden = -1)
    env = GridWorld(
        size=5,
        goal=(4, 4),
        forbidden_states=[(1, 1), (2, 3)],
        r_boundary=-1.0,
        r_forbidden=-1.0,  # 任务 1 要求
        r_target=1.0,
        r_step=0.0,
        gamma=0.9
    )
    print(f"环境配置: {env}")
    
    horizons = [1, 2, 3, 4, 5, 14, 15, 30, 100]
    
    for H in horizons:
        print(f"\n--- 运行 MC Basic (H = {H}) ---")
        V, policy, Q, iters = mc_basic(env, horizon=H, verbose=True)
        
        # 打印部分状态值
        print(f"  收敛迭代次数: {iters}")
        print("  状态值矩阵 (局部，保留2位):")
        print(V.reshape(env.size, env.size).round(2))
        
        # 可视化并保存
        plot_value_heatmap(V, env, title=f"MC Basic 值函数 (H={H})",
                           save_path=os.path.join(save_dir, f"mc_basic_value_H{H}.png"))
        plot_policy_arrows(policy, env, title=f"MC Basic 最终策略 (H={H})", V=V,
                           save_path=os.path.join(save_dir, f"mc_basic_policy_H{H}.png"))
        

if __name__ == "__main__":
    run_mc_basic_experiments()
