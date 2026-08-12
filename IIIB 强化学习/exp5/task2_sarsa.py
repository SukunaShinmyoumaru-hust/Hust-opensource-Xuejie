import os
import numpy as np
import matplotlib.pyplot as plt
from gridworld import GridWorld
from visualization import plot_policy_arrows
from visualization import shared_visualization

def set_seed(seed=42):
    np.random.seed(seed)

def get_joint_features(s, a, env, state_dim=6):
    """生成状态与动作联合特征向量 phi(s, a)"""
    row, col = env.state_to_pos(s)
    # 归一化坐标
    x = (col + 1.0) / 5.0
    y = (row + 1.0) / 5.0
    if state_dim == 3:
        phi_s = np.array([1.0, x, y])
    elif state_dim == 6:
        phi_s = np.array([1.0, x, y, x**2, y**2, x*y])
    else:
        raise ValueError("Unsupported state dim")
    
    # 与动作的一独热编码结合 (维度为 state_dim * 4)
    phi = np.zeros(state_dim * env.num_actions)
    phi[a * state_dim : (a + 1) * state_dim] = phi_s
    return phi

def get_q_values(s, w, env, state_dim=6):
    """计算状态 s 下所有动作的估计值"""
    q_vals = np.zeros(env.num_actions)
    for a in range(env.num_actions):
        phi = get_joint_features(s, a, env, state_dim)
        q_vals[a] = np.dot(phi, w)
    return q_vals

def select_action(s, w, env, state_dim=6, epsilon=0.1):
    """Epsilon-Greedy 动作选择"""
    if np.random.rand() < epsilon:
        return np.random.choice(env.num_actions)
    else:
        q_vals = get_q_values(s, w, env, state_dim)
        max_q = np.max(q_vals)
        best_actions = np.where(q_vals == max_q)[0]
        return np.random.choice(best_actions)

def extract_policy(w, env, state_dim=6, random_ties=False):
    """Extract the greedy policy represented by the current weights."""
    policy = np.zeros(env.num_states, dtype=int)
    for s in range(env.num_states):
        if env.is_terminal(s):
            continue
        q_vals = get_q_values(s, w, env, state_dim)
        best_actions = np.flatnonzero(np.isclose(q_vals, np.max(q_vals)))
        policy[s] = np.random.choice(best_actions) if random_ties else best_actions[0]
    return policy

def run_sarsa(env, state_dim=6, alpha=0.01, epsilon=0.1, episodes=1000, max_steps=100,
              snapshot_episodes=None):
    """Sarsa 控制算法"""
    w = np.zeros(state_dim * env.num_actions)
    returns = []
    success_rates = []
    success_window = []
    snapshot_episodes = set(snapshot_episodes or [])
    snapshots = {}

    if 0 in snapshot_episodes:
        snapshots[0] = extract_policy(w, env, state_dim, random_ties=True)
    
    for ep in range(episodes):
        # 随机从非终止状态开始
        s = np.random.choice(env.get_non_terminal_states())
        a = select_action(s, w, env, state_dim, epsilon)
        
        ep_reward = 0.0
        step_count = 0
        success = 0
        
        while not env.is_terminal(s) and step_count < max_steps:
            s_next, reward = env.step(s, a)
            ep_reward += reward
            
            phi_sa = get_joint_features(s, a, env, state_dim)
            q_sa = np.dot(phi_sa, w)
            
            if env.is_terminal(s_next):
                q_next = 0.0
                success = 1
                a_next = 0 # Dummy action
            else:
                a_next = select_action(s_next, w, env, state_dim, epsilon)
                phi_next = get_joint_features(s_next, a_next, env, state_dim)
                q_next = np.dot(phi_next, w)
                
            td_target = reward + env.gamma * q_next
            td_error = td_target - q_sa
            w += alpha * td_error * phi_sa
            
            s = s_next
            a = a_next
            step_count += 1
            
        returns.append(ep_reward)
        success_window.append(success)
        if len(success_window) > 50:
            success_window.pop(0)
        success_rates.append(np.mean(success_window))

        completed_episodes = ep + 1
        if completed_episodes in snapshot_episodes:
            snapshots[completed_episodes] = extract_policy(w, env, state_dim)
        
    if snapshot_episodes:
        return w, returns, success_rates, snapshots
    return w, returns, success_rates

def main():
    set_seed(42)
    os.makedirs("results", exist_ok=True)
    
    env = GridWorld()
    state_dim = 6
    alpha = 0.05
    epsilon = 0.1
    episodes = 1000
    
    print("Running Sarsa with Linear Function Approximation...")
    snapshot_episodes = [0, 50, 200, 1000]
    w, returns, success_rates, snapshots = run_sarsa(
        env, state_dim=state_dim, alpha=alpha, epsilon=epsilon, episodes=episodes,
        snapshot_episodes=snapshot_episodes
    )
    
    # 提取最终策略与动作值
    policy = np.zeros(env.num_states)
    V_est = np.zeros(env.num_states)
    for s in range(env.num_states):
        if env.is_terminal(s):
            policy[s] = 0
            V_est[s] = 0.0
        else:
            q_vals = get_q_values(s, w, env, state_dim)
            policy[s] = np.argmax(q_vals)
            V_est[s] = np.max(q_vals)
            
    print("Sarsa Final Estimated Values:")
    print(V_est.reshape(env.size, env.size))
    
    # 绘制最终策略箭头图
    plot_policy_arrows(policy, env, "Sarsa Linear Policy", V=V_est, save_path="results/task2_sarsa_policy.png")

    shared_visualization.plot_policy_evolution(
        [snapshots[ep] for ep in snapshot_episodes],
        env,
        titles=[f"Episode {ep}" for ep in snapshot_episodes],
        save_path="results/task2_sarsa_policy_evolution.png"
    )
    
    # 绘制收敛曲线
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
    
    # 绘制滑动平均后的回报
    window = 50
    returns_smoothed = np.convolve(returns, np.ones(window)/window, mode='valid')
    ax1.plot(returns, alpha=0.3, label="Raw Return", color='#3498db')
    ax1.plot(np.arange(window-1, len(returns)), returns_smoothed, label="Smoothed Return (window=50)", color='#2c3e50', linewidth=2)
    ax1.set_xlabel("Episodes")
    ax1.set_ylabel("Returns")
    ax1.set_title("Sarsa Return Convergence")
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # 绘制成功率
    ax2.plot(success_rates, label="Success Rate (window=50)", color='#2ecc71', linewidth=2)
    ax2.set_xlabel("Episodes")
    ax2.set_ylabel("Success Rate")
    ax2.set_title("Sarsa Success Rate Convergence")
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig("results/task2_sarsa_convergence.png", dpi=150, bbox_inches='tight')
    plt.close()
    print("Sarsa simulation complete. Saved results to results/.")

if __name__ == "__main__":
    main()
