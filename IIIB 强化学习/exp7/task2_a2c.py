"""
任务 2: A2C 算法 (Advantage Actor-Critic) 实现与对比
"""

import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from gridworld import GridWorld
from visualization import plot_value_heatmap, plot_policy_arrows, plot_learning_curves

def get_state_feature(s, env):
    row, col = env.state_to_pos(s)
    x = row + 1
    y = col + 1
    return np.array([1.0, x, y], dtype=np.float64)

def softmax_policy(s, theta, env):
    phi = get_state_feature(s, env)
    h = theta.dot(phi)  # shape: (4,)
    h_stable = h - np.max(h)
    exp_h = np.exp(h_stable)
    probs = exp_h / np.sum(exp_h)
    return probs

def sample_action(probs):
    return np.random.choice(len(probs), p=probs)

def run_qac(env, num_episodes=1000, max_steps=500, alpha_theta=0.001, alpha_w=0.01, seed=42):
    np.random.seed(seed)
    num_actions = env.num_actions
    feature_dim = 3
    
    theta = np.zeros((num_actions, feature_dim), dtype=np.float64)
    w = np.zeros((num_actions, feature_dim), dtype=np.float64)
    
    episode_rewards = []
    non_terminal_states = env.get_non_terminal_states()
    
    for ep in range(num_episodes):
        s = np.random.choice(non_terminal_states)
        probs = softmax_policy(s, theta, env)
        a = sample_action(probs)
        ep_reward = 0.0
        step = 0
        I = 1.0
        
        while step < max_steps:
            phi_s = get_state_feature(s, env)
            q_sa = phi_s.dot(w[a])
            
            s_next, r = env.step(s, a)
            ep_reward += r
            
            if env.is_terminal(s_next):
                delta = r - q_sa
                w[a] += alpha_w * delta * phi_s
                for c in range(num_actions):
                    grad_ln_pi = phi_s * ((1.0 if c == a else 0.0) - probs[c])
                    theta[c] += alpha_theta * I * grad_ln_pi * q_sa
                break
            else:
                probs_next = softmax_policy(s_next, theta, env)
                a_next = sample_action(probs_next)
                
                phi_s_next = get_state_feature(s_next, env)
                q_s_next_a_next = phi_s_next.dot(w[a_next])
                
                delta = r + env.gamma * q_s_next_a_next - q_sa
                w[a] += alpha_w * delta * phi_s
                for c in range(num_actions):
                    grad_ln_pi = phi_s * ((1.0 if c == a else 0.0) - probs[c])
                    theta[c] += alpha_theta * I * grad_ln_pi * q_sa
                
                s = s_next
                a = a_next
                probs = probs_next
                I *= env.gamma
            step += 1
        episode_rewards.append(ep_reward)
        
    return theta, w, episode_rewards

def run_a2c(env, num_episodes=1000, max_steps=500, alpha_theta=0.001, alpha_w=0.01, seed=42):
    np.random.seed(seed)
    num_actions = env.num_actions
    feature_dim = 3
    
    theta = np.zeros((num_actions, feature_dim), dtype=np.float64)  # Actor 参数
    w = np.zeros(feature_dim, dtype=np.float64)                     # Critic 参数 (A2C 中状态价值 v(s, w) = w^T phi(s))
    
    episode_rewards = []
    non_terminal_states = env.get_non_terminal_states()
    
    for ep in range(num_episodes):
        s = np.random.choice(non_terminal_states)
        ep_reward = 0.0
        step = 0
        I = 1.0
        
        while step < max_steps:
            probs = softmax_policy(s, theta, env)
            a = sample_action(probs)
            
            phi_s = get_state_feature(s, env)
            v_s = phi_s.dot(w)
            
            s_next, r = env.step(s, a)
            ep_reward += r
            
            if env.is_terminal(s_next):
                delta = r - v_s  # TD 误差，即优势估计
                # 更新 Critic
                w += alpha_w * delta * phi_s
                # 更新 Actor
                for c in range(num_actions):
                    grad_ln_pi = phi_s * ((1.0 if c == a else 0.0) - probs[c])
                    theta[c] += alpha_theta * I * grad_ln_pi * delta
                break
            else:
                phi_s_next = get_state_feature(s_next, env)
                v_s_next = phi_s_next.dot(w)
                
                delta = r + env.gamma * v_s_next - v_s  # TD 误差，即优势估计
                # 更新 Critic
                w += alpha_w * delta * phi_s
                # 更新 Actor
                for c in range(num_actions):
                    grad_ln_pi = phi_s * ((1.0 if c == a else 0.0) - probs[c])
                    theta[c] += alpha_theta * I * grad_ln_pi * delta
                
                s = s_next
                I *= env.gamma
            step += 1
        episode_rewards.append(ep_reward)
        
    return theta, w, episode_rewards

if __name__ == "__main__":
    env = GridWorld()
    os.makedirs("results", exist_ok=True)
    
    # 进行多次独立运行以对比方差
    num_runs = 10
    num_episodes = 1000
    
    qac_runs_rewards = []
    a2c_runs_rewards = []
    
    print(f"开始运行 QAC 和 A2C 的 {num_runs} 次独立实验对比...")
    for run in range(num_runs):
        seed = 42 + run
        print(f"正在进行第 {run+1}/{num_runs} 次独立运行，随机种子为 {seed}...")
        
        # 运行 QAC
        _, _, qac_rew = run_qac(env, num_episodes=num_episodes, seed=seed)
        qac_runs_rewards.append(qac_rew)
        
        # 运行 A2C
        _, _, a2c_rew = run_a2c(env, num_episodes=num_episodes, seed=seed)
        a2c_runs_rewards.append(a2c_rew)
        
    qac_runs_rewards = np.array(qac_runs_rewards)
    a2c_runs_rewards = np.array(a2c_runs_rewards)
    
    # 打印最终收敛结果统计（多独立运行最后 100 个 episode 的平均奖励）
    qac_runs_means = np.mean(qac_runs_rewards[:, -100:], axis=1)
    a2c_runs_means = np.mean(a2c_runs_rewards[:, -100:], axis=1)
    
    print("\n" + "=" * 60)
    print(f" 最终收敛结果统计 ({num_runs}次独立运行最后100个episode的平均奖励) ")
    print("=" * 60)
    print(f"QAC: {np.mean(qac_runs_means):.4f} ± {np.std(qac_runs_means):.4f}")
    print(f"A2C: {np.mean(a2c_runs_means):.4f} ± {np.std(a2c_runs_means):.4f}")
    print("=" * 60 + "\n")
    
    # 绘制对比曲线 (使用滑动平均)
    window = 30
    
    # 对每条曲线做平滑
    def smooth_matrix(matrix, w_size):
        smoothed = []
        for row in matrix:
            smoothed.append(np.convolve(row, np.ones(w_size)/w_size, mode='valid'))
        return np.array(smoothed)
        
    qac_smoothed = smooth_matrix(qac_runs_rewards, window)
    a2c_smoothed = smooth_matrix(a2c_runs_rewards, window)
    
    qac_mean = np.mean(qac_smoothed, axis=0)
    qac_std = np.std(qac_smoothed, axis=0)
    
    a2c_mean = np.mean(a2c_smoothed, axis=0)
    a2c_std = np.std(a2c_smoothed, axis=0)
    
    x = range(window, num_episodes + 1)
    
    plt.figure(figsize=(10, 6))
    plt.plot(x, qac_mean, label="QAC (Mean)", color="#3498db", lw=2)
    plt.fill_between(x, qac_mean - qac_std, qac_mean + qac_std, color="#3498db", alpha=0.15)
    
    plt.plot(x, a2c_mean, label="A2C (Mean)", color="#2ecc71", lw=2)
    plt.fill_between(x, a2c_mean - a2c_std, a2c_mean + a2c_std, color="#2ecc71", alpha=0.15)
    
    plt.title(f"QAC 与 A2C 学习曲线对比 ({num_runs}次独立运行均值和标准差)")
    plt.xlabel("Episode")
    plt.ylabel("总奖励")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig("results/task2_qac_vs_a2c.png", dpi=150)
    plt.close()
    
    print("对比图已保存到 results/task2_qac_vs_a2c.png")
    
    # 单独运行一次 A2C 提取模型并可视化其最终的最优策略和状态值
    theta_a2c, w_a2c, _ = run_a2c(env, num_episodes=num_episodes, seed=42)
    
    V_final = np.zeros(env.num_states)
    pi_final = np.zeros(env.num_states)
    for s in range(env.num_states):
        if env.is_terminal(s):
            V_final[s] = 0.0
            pi_final[s] = 0
        else:
            phi = get_state_feature(s, env)
            V_final[s] = phi.dot(w_a2c)
            probs = softmax_policy(s, theta_a2c, env)
            pi_final[s] = np.argmax(probs)
            
    # 绘制状态值热力图和最优策略图
    plot_value_heatmap(V_final, env, title="A2C 最终状态值函数", save_path="results/task2_a2c_values.png")
    plot_policy_arrows(pi_final, env, title="A2C 最终最优策略", V=V_final, save_path="results/task2_a2c_policy.png")
