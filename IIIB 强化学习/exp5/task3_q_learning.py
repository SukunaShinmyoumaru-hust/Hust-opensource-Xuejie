import os
import numpy as np
import matplotlib.pyplot as plt
from gridworld import GridWorld
from visualization import plot_policy_arrows
from task2_sarsa import get_joint_features, get_q_values, select_action, run_sarsa

def set_seed(seed=42):
    np.random.seed(seed)

def run_q_learning(env, state_dim=6, alpha=0.01, epsilon=0.1, episodes=1000, max_steps=100):
    """Q-learning 算法"""
    w = np.zeros(state_dim * env.num_actions)
    returns = []
    success_rates = []
    success_window = []
    
    for ep in range(episodes):
        s = np.random.choice(env.get_non_terminal_states())
        ep_reward = 0.0
        step_count = 0
        success = 0
        
        while not env.is_terminal(s) and step_count < max_steps:
            a = select_action(s, w, env, state_dim, epsilon)
            s_next, reward = env.step(s, a)
            ep_reward += reward
            
            phi_sa = get_joint_features(s, a, env, state_dim)
            q_sa = np.dot(phi_sa, w)
            
            if env.is_terminal(s_next):
                q_next_max = 0.0
                success = 1
            else:
                q_next_vals = get_q_values(s_next, w, env, state_dim)
                q_next_max = np.max(q_next_vals)
                
            td_target = reward + env.gamma * q_next_max
            td_error = td_target - q_sa
            w += alpha * td_error * phi_sa
            
            s = s_next
            step_count += 1
            
        returns.append(ep_reward)
        success_window.append(success)
        if len(success_window) > 50:
            success_window.pop(0)
        success_rates.append(np.mean(success_window))
        
    return w, returns, success_rates

def evaluate_policy(w, env, state_dim=6, max_eval_steps=20):
    """评估贪婪策略在所有非终止起点的成功率与平均最短路径长度"""
    success_count = 0
    total_steps = 0
    non_terminal_states = env.get_non_terminal_states()
    
    for s_start in non_terminal_states:
        s = s_start
        steps = 0
        success = False
        while not env.is_terminal(s) and steps < max_eval_steps:
            q_vals = get_q_values(s, w, env, state_dim)
            a = np.argmax(q_vals)
            s_next, _ = env.step(s, a)
            s = s_next
            steps += 1
            if env.is_terminal(s):
                success = True
                break
        
        if success:
            success_count += 1
            total_steps += steps
            
    success_rate = success_count / len(non_terminal_states)
    avg_path_len = total_steps / success_count if success_count > 0 else float('nan')
    return success_rate, avg_path_len

def main():
    set_seed(42)
    os.makedirs("results", exist_ok=True)
    
    env = GridWorld()
    state_dim = 6
    alpha = 0.05
    epsilon = 0.1
    episodes = 1000
    
    # 1. 运行 Sarsa 并计算其最终评估
    print("Running Sarsa (Online)...")
    w_sarsa, returns_sarsa, success_sarsa = run_sarsa(env, state_dim, alpha, epsilon, episodes)
    sarsa_succ, sarsa_len = evaluate_policy(w_sarsa, env, state_dim)
    print(f"Sarsa Evaluated: Success Rate = {sarsa_succ:.2%}, Avg Path Length = {sarsa_len:.2f}")
    
    # 2. 运行 Q-learning 并计算其最终评估
    print("\nRunning Q-learning (Online)...")
    w_q, returns_q, success_q = run_q_learning(env, state_dim, alpha, epsilon, episodes)
    q_succ, q_len = evaluate_policy(w_q, env, state_dim)
    print(f"Q-learning Evaluated: Success Rate = {q_succ:.2%}, Avg Path Length = {q_len:.2f}")
    
    # 3. 提取策略用于绘图
    policy_sarsa = np.zeros(env.num_states)
    policy_q = np.zeros(env.num_states)
    for s in range(env.num_states):
        if env.is_terminal(s):
            policy_sarsa[s] = 0
            policy_q[s] = 0
        else:
            policy_sarsa[s] = np.argmax(get_q_values(s, w_sarsa, env, state_dim))
            policy_q[s] = np.argmax(get_q_values(s, w_q, env, state_dim))
            
    plot_policy_arrows(policy_sarsa, env, "Sarsa Final Policy", save_path="results/task3_sarsa_policy.png")
    plot_policy_arrows(policy_q, env, "Q-learning Final Policy", save_path="results/task3_q_policy.png")
    
    # 4. 绘制 Sarsa 与 Q-learning 的对比曲线
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
    
    # 绘制滑动平均回报对比
    window = 50
    ret_sarsa_smooth = np.convolve(returns_sarsa, np.ones(window)/window, mode='valid')
    ret_q_smooth = np.convolve(returns_q, np.ones(window)/window, mode='valid')
    
    ax1.plot(ret_sarsa_smooth, label="Sarsa (Online)", color='#3498db', linewidth=2)
    ax1.plot(ret_q_smooth, label="Q-learning (Online)", color='#e74c3c', linewidth=2)
    ax1.set_xlabel("Episodes")
    ax1.set_ylabel("Smoothed Returns")
    ax1.set_title("Sarsa vs Q-learning Returns Convergence (Online)")
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # 绘制成功率对比
    ax2.plot(success_sarsa, label="Sarsa (Online)", color='#3498db', linewidth=2)
    ax2.plot(success_q, label="Q-learning (Online)", color='#e74c3c', linewidth=2)
    ax2.set_xlabel("Episodes")
    ax2.set_ylabel("Success Rate")
    ax2.set_title("Sarsa vs Q-learning Success Rate Convergence")
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig("results/task3_sarsa_vs_q_convergence.png", dpi=150, bbox_inches='tight')
    plt.close()
    
    print("\nTask 3 Simulation Complete! Saved results and comparison plots to results/.")

if __name__ == "__main__":
    main()
