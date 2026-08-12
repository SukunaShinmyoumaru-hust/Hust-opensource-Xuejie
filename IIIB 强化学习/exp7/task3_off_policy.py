"""
任务 3: 异策略 Actor-Critic (Off-policy Actor-Critic) 实现
"""

import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from gridworld import GridWorld
from visualization import plot_policy_arrows

def solve_gridworld_vi(env):
    """使用值迭代求解 GridWorld 的精确 Q 表，用于构建行为策略"""
    V = np.zeros(env.num_states)
    threshold = 1e-6
    while True:
        delta = 0
        for s in range(env.num_states):
            if env.is_terminal(s):
                continue
            v_old = V[s]
            q_vals = []
            for a in range(env.num_actions):
                next_s, r = env.step(s, a)
                q_vals.append(r + env.gamma * (0.0 if env.is_terminal(next_s) else V[next_s]))
            V[s] = max(q_vals)
            delta = max(delta, abs(v_old - V[s]))
        if delta < threshold:
            break
            
    Q = np.zeros((env.num_states, env.num_actions))
    for s in range(env.num_states):
        if env.is_terminal(s):
            continue
        for a in range(env.num_actions):
            next_s, r = env.step(s, a)
            Q[s, a] = r + env.gamma * (0.0 if env.is_terminal(next_s) else V[next_s])
    return Q

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

def behavior_policy(s, env, Q_star, epsilon=0.3):
    """基于最优 Q 值的固定 epsilon-greedy 行为策略"""
    if env.is_terminal(s):
        return np.ones(env.num_actions) / env.num_actions
    best_a = np.argmax(Q_star[s])
    probs = np.ones(env.num_actions) * (epsilon / env.num_actions)
    probs[best_a] += (1.0 - epsilon)
    return probs

def collect_dataset(env, Q_star, num_steps=5000, seed=42):
    """使用行为策略在环境中采集固定步数的数据集"""
    np.random.seed(seed)
    dataset = []
    
    non_terminal_states = env.get_non_terminal_states()
    s = np.random.choice(non_terminal_states)
    
    for _ in range(num_steps):
        b_probs = behavior_policy(s, env, Q_star, epsilon=0.3)
        a = np.random.choice(env.num_actions, p=b_probs)
        s_next, r = env.step(s, a)
        
        # 存入样本: (s, a, r, s_next, done, beta(a|s))
        dataset.append((s, a, r, s_next, env.is_terminal(s_next), b_probs[a]))
        
        if env.is_terminal(s_next):
            s = np.random.choice(non_terminal_states)
        else:
            s = s_next
            
    return dataset

def evaluate_target_policy(env, theta, num_episodes=15, seed=100):
    """评估目标策略，返回平均累积奖励 (确定性/贪心策略)"""
    np.random.seed(seed)
    total_rewards = []
    non_terminal_states = env.get_non_terminal_states()
    
    for _ in range(num_episodes):
        s = np.random.choice(non_terminal_states)
        ep_reward = 0.0
        step = 0
        while step < 100:
            probs = softmax_policy(s, theta, env)
            a = np.argmax(probs)  # 贪心选择
            s_next, r = env.step(s, a)
            ep_reward += r
            if env.is_terminal(s_next):
                break
            s = s_next
            step += 1
        total_rewards.append(ep_reward)
    return np.mean(total_rewards)

def train_off_policy(env, dataset, num_epochs=100, alpha_theta=0.001, alpha_w=0.01, seed=42):
    np.random.seed(seed)
    num_actions = env.num_actions
    feature_dim = 3
    
    # 初始化目标策略参数 theta 和状态值网络参数 w
    theta = np.zeros((num_actions, feature_dim), dtype=np.float64)
    w = np.zeros(feature_dim, dtype=np.float64)
    
    epoch_rewards = []
    
    for epoch in range(num_epochs):
        # 打乱数据集
        indices = np.arange(len(dataset))
        np.random.shuffle(indices)
        
        for idx in indices:
            s, a, r, s_next, done, beta_a = dataset[idx]
            
            # 特征表示
            phi_s = get_state_feature(s, env)
            v_s = phi_s.dot(w)
            
            # 计算 TD 误差
            if done:
                v_s_next = 0.0
            else:
                phi_s_next = get_state_feature(s_next, env)
                v_s_next = phi_s_next.dot(w)
                
            delta = r + env.gamma * v_s_next - v_s
            
            # 计算目标策略概率
            pi_probs = softmax_policy(s, theta, env)
            pi_a = pi_probs[a]
            
            # 重要性采样权重 rho
            rho = pi_a / beta_a
            # 剪枝以增强数值稳定性
            rho = np.clip(rho, 0.0, 10.0)
            
            # 更新 Critic
            w += alpha_w * rho * delta * phi_s
            
            # 更新 Actor
            for c in range(num_actions):
                grad_ln_pi = phi_s * ((1.0 if c == a else 0.0) - pi_probs[c])
                theta[c] += alpha_theta * rho * grad_ln_pi * delta
                
        # 评估当前目标策略
        mean_eval_reward = evaluate_target_policy(env, theta, num_episodes=20)
        epoch_rewards.append(mean_eval_reward)
        
    return theta, w, epoch_rewards

if __name__ == "__main__":
    env = GridWorld()
    os.makedirs("results", exist_ok=True)
    
    # 1. 求解 GridWorld 精确值以构建行为策略
    print("正在计算精确的 Q 值以构造行为策略...")
    Q_star = solve_gridworld_vi(env)
    
    # 2. 收集数据集 (5000步)
    print("行为策略正在收集 5000 步的经验样本...")
    dataset = collect_dataset(env, Q_star, num_steps=5000, seed=42)
    
    # 3. 离线训练目标策略
    print("开始使用重要性采样的异策略 Actor-Critic 离线训练...")
    theta_target, w_target, epoch_rewards = train_off_policy(env, dataset, num_epochs=100, alpha_theta=0.0005, alpha_w=0.005)
    print("异策略 Actor-Critic 训练完成！")
    
    # 绘制目标策略离线学习曲线
    plt.figure(figsize=(8, 5))
    plt.plot(range(1, len(epoch_rewards) + 1), epoch_rewards, color="#e74c3c", lw=2)
    plt.title("异策略 Actor-Critic 离线训练评估曲线")
    plt.xlabel("Epoch")
    plt.ylabel("评估平均总奖励")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig("results/task3_off_policy_learning.png", dpi=150)
    plt.close()
    
    # 4. 对比行为策略与最终学习到的目标策略
    pi_behavior = np.zeros(env.num_states)
    pi_target = np.zeros(env.num_states)
    
    for s in range(env.num_states):
        if env.is_terminal(s):
            pi_behavior[s] = 0
            pi_target[s] = 0
        else:
            pi_behavior[s] = np.argmax(Q_star[s])
            probs_target = softmax_policy(s, theta_target, env)
            pi_target[s] = np.argmax(probs_target)
            
    # 绘制策略图对比
    plot_policy_arrows(pi_behavior, env, title="行为策略 (ε-greedy, ε=0.3)", save_path="results/task3_behavior_policy.png")
    plot_policy_arrows(pi_target, env, title="异策略学习到的目标策略 (Target Policy)", save_path="results/task3_target_policy.png")
    
    print("行为策略和目标策略的对比图已生成并保存。")
