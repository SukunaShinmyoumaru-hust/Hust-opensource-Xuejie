import os
import numpy as np
import matplotlib.pyplot as plt
from gridworld import GridWorld
from visualization import plot_value_heatmap

def set_seed(seed=42):
    np.random.seed(seed)

def compute_true_value(env):
    """通过动态规划（策略评估）计算随机策略下的精确状态值函数"""
    V = np.zeros(env.num_states)
    threshold = 1e-9
    gamma = env.gamma
    while True:
        delta = 0
        V_new = np.zeros(env.num_states)
        for s in range(env.num_states):
            if env.is_terminal(s):
                V_new[s] = 0.0
                continue
            val = 0.0
            for a in range(env.num_actions):
                next_s, reward = env.step(s, a)
                val += 0.25 * (reward + gamma * V[next_s])
            V_new[s] = val
            delta = max(delta, abs(V_new[s] - V[s]))
        V = V_new
        if delta < threshold:
            break
    return V

def get_features(s, env, dim):
    """根据给定的特征维度，生成归一化状态特征向量 phi(s)"""
    row, col = env.state_to_pos(s)
    # 归一化坐标 x, y 在 [0.2, 1.0] 之间，以保证高阶项稳定
    x = (col + 1.0) / 5.0
    y = (row + 1.0) / 5.0
    if dim == 3:
        return np.array([1.0, x, y])
    elif dim == 6:
        return np.array([1.0, x, y, x**2, y**2, x*y])
    elif dim == 10:
        return np.array([1.0, x, y, x**2, y**2, x*y, x**3, y**3, (x**2)*y, x*(y**2)])
    else:
        raise ValueError("Unsupported dim")

def run_td_linear(env, true_V, dim, alpha=0.001, episodes=2000, max_steps=200):
    """实现 TD-Linear 状态值估计算法"""
    w = np.zeros(dim)
    rmse_history = []
    
    for ep in range(episodes):
        s = np.random.choice(env.get_non_terminal_states())
        step_count = 0
        while not env.is_terminal(s) and step_count < max_steps:
            a = np.random.choice(env.num_actions)
            s_next, reward = env.step(s, a)
            
            phi_s = get_features(s, env, dim)
            val_s = np.dot(phi_s, w)
            
            if env.is_terminal(s_next):
                val_next = 0.0
            else:
                phi_next = get_features(s_next, env, dim)
                val_next = np.dot(phi_next, w)
                
            td_target = reward + env.gamma * val_next
            td_error = td_target - val_s
            w += alpha * td_error * phi_s
            
            s = s_next
            step_count += 1
            
        # 计算当前的估计值并记录 RMSE
        V_est = np.zeros(env.num_states)
        for state in range(env.num_states):
            if env.is_terminal(state):
                V_est[state] = 0.0
            else:
                V_est[state] = np.dot(get_features(state, env, dim), w)
        
        non_term = env.get_non_terminal_states()
        rmse = np.sqrt(np.mean((true_V[non_term] - V_est[non_term])**2))
        rmse_history.append(rmse)
        
    return rmse_history, V_est, w

def run_td_table(env, true_V, alpha=0.05, episodes=2000, max_steps=200):
    """实现表格型 TD 算法 (TD-Table)"""
    V = np.zeros(env.num_states)
    rmse_history = []
    
    for ep in range(episodes):
        s = np.random.choice(env.get_non_terminal_states())
        step_count = 0
        while not env.is_terminal(s) and step_count < max_steps:
            a = np.random.choice(env.num_actions)
            s_next, reward = env.step(s, a)
            
            val_s = V[s]
            if env.is_terminal(s_next):
                val_next = 0.0
            else:
                val_next = V[s_next]
                
            td_target = reward + env.gamma * val_next
            td_error = td_target - val_s
            V[s] += alpha * td_error
            
            s = s_next
            step_count += 1
            
        non_term = env.get_non_terminal_states()
        rmse = np.sqrt(np.mean((true_V[non_term] - V[non_term])**2))
        rmse_history.append(rmse)
        
    return rmse_history, V

def main():
    set_seed(42)
    os.makedirs("results", exist_ok=True)
    
    # 1. 初始化环境
    env = GridWorld()
    
    # 2. 计算真值
    true_V = compute_true_value(env)
    print("True V (Random Policy):")
    print(true_V.reshape(env.size, env.size))
    
    # 3. 运行 TD-Table
    print("\nRunning TD-Table...")
    rmse_table, V_table = run_td_table(env, true_V, alpha=0.05, episodes=2000)
    print("TD-Table Final RMSE:", rmse_table[-1])
    
    # 4. 运行 TD-Linear (3维)
    print("\nRunning TD-Linear (3-dim)...")
    rmse_3d, V_3d, w_3d = run_td_linear(env, true_V, dim=3, alpha=0.001, episodes=2000)
    print("TD-Linear (3-dim) Final RMSE:", rmse_3d[-1])
    print("Weights (3-dim):", w_3d)
    
    # 5. 运行 TD-Linear (6维)
    print("\nRunning TD-Linear (6-dim)...")
    rmse_6d, V_6d, w_6d = run_td_linear(env, true_V, dim=6, alpha=0.001, episodes=2000)
    print("TD-Linear (6-dim) Final RMSE:", rmse_6d[-1])
    print("Weights (6-dim):", w_6d)
    
    # 6. 运行 TD-Linear (10维)
    print("\nRunning TD-Linear (10-dim)...")
    rmse_10d, V_10d, w_10d = run_td_linear(env, true_V, dim=10, alpha=0.001, episodes=2000)
    print("TD-Linear (10-dim) Final RMSE:", rmse_10d[-1])
    print("Weights (10-dim):", w_10d)
    
    # 7. 绘制对比图
    plt.figure(figsize=(10, 6))
    plt.plot(rmse_table, label="TD-Table (alpha=0.05)", alpha=0.8)
    plt.plot(rmse_3d, label="TD-Linear (3-dim, alpha=0.001)", alpha=0.8)
    plt.plot(rmse_6d, label="TD-Linear (6-dim, alpha=0.001)", alpha=0.8)
    plt.plot(rmse_10d, label="TD-Linear (10-dim, alpha=0.001)", alpha=0.8)
    plt.xlabel("Episodes")
    plt.ylabel("RMSE")
    plt.title("State Value Estimation Error (RMSE vs Episodes)")
    plt.legend()
    plt.grid(True, alpha=0.3)
    save_path = "results/task1_rmse_comparison.png"
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"Saved RMSE comparison plot to {save_path}")
    
    # 8. 绘制并保存各种值函数的热图
    plot_value_heatmap(true_V, env, "True State Values (Random Policy)", save_path="results/task1_true_val.png")
    plot_value_heatmap(V_table, env, "TD-Table Estimated Values", save_path="results/task1_td_table_val.png")
    plot_value_heatmap(V_3d, env, "TD-Linear (3-dim) Estimated Values", save_path="results/task1_td_3d_val.png")
    plot_value_heatmap(V_6d, env, "TD-Linear (6-dim) Estimated Values", save_path="results/task1_td_6d_val.png")
    plot_value_heatmap(V_10d, env, "TD-Linear (10-dim) Estimated Values", save_path="results/task1_td_10d_val.png")

if __name__ == "__main__":
    main()
