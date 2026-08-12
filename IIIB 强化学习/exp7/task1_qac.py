"""
任务 1: QAC 算法 (Q Actor-Critic) 实现
"""

import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from gridworld import GridWorld
from visualization import plot_value_heatmap, plot_policy_arrows

def get_state_feature(s, env):
    """
    根据状态索引 s 计算特征向量 phi(s) = [1, x, y]^T
    坐标范围 x, y in {1, 2, 3, 4, 5}
    """
    row, col = env.state_to_pos(s)
    x = row + 1
    y = col + 1
    return np.array([1.0, x, y], dtype=np.float64)

def softmax_policy(s, theta, env):
    """
    根据参数 theta 计算状态 s 下的动作概率分布
    theta 的形状为 (num_actions, feature_dim) = (4, 3)
    """
    phi = get_state_feature(s, env)
    h = theta.dot(phi)  # shape: (4,)
    # 减去最大值防止溢出
    h_stable = h - np.max(h)
    exp_h = np.exp(h_stable)
    probs = exp_h / np.sum(exp_h)
    return probs

def sample_action(probs):
    """根据概率分布采样动作"""
    return np.random.choice(len(probs), p=probs)

def run_qac(env, num_episodes=1000, max_steps=500, alpha_theta=0.001, alpha_w=0.01, seed=42):
    # 固定随机种子
    np.random.seed(seed)
    
    # 状态和动作维度
    num_actions = env.num_actions
    feature_dim = 3  # phi(s) = [1, x, y]
    
    # 初始化参数
    theta = np.zeros((num_actions, feature_dim), dtype=np.float64)  # Actor 参数
    w = np.zeros((num_actions, feature_dim), dtype=np.float64)      # Critic 参数 (QAC 中每一动作有独立的 w_a)
    
    episode_rewards = []
    
    # 记录最终动作概率和状态值，用于画图
    non_terminal_states = env.get_non_terminal_states()
    
    for ep in range(num_episodes):
        # 随机选择一个非终止状态作为起点
        s = np.random.choice(non_terminal_states)
        
        # 采样动作
        probs = softmax_policy(s, theta, env)
        a = sample_action(probs)
        
        ep_reward = 0.0
        step = 0
        I = 1.0  # 折扣因子追踪器
        
        while step < max_steps:
            # 状态特征
            phi_s = get_state_feature(s, env)
            q_sa = phi_s.dot(w[a])
            
            # 环境步进
            s_next, r = env.step(s, a)
            ep_reward += r
            
            if env.is_terminal(s_next):
                # 终止状态，未来价值为 0
                delta = r - q_sa
                # 更新 Critic
                w[a] += alpha_w * delta * phi_s
                # 更新 Actor
                for c in range(num_actions):
                    grad_ln_pi = phi_s * ((1.0 if c == a else 0.0) - probs[c])
                    theta[c] += alpha_theta * I * grad_ln_pi * q_sa
                break
            else:
                # 采样下一步动作
                probs_next = softmax_policy(s_next, theta, env)
                a_next = sample_action(probs_next)
                
                # 计算特征和下一步 Q 值
                phi_s_next = get_state_feature(s_next, env)
                q_s_next_a_next = phi_s_next.dot(w[a_next])
                
                # TD 误差 (Sarsa 形式)
                delta = r + env.gamma * q_s_next_a_next - q_sa
                
                # 更新 Critic
                w[a] += alpha_w * delta * phi_s
                # 更新 Actor
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

if __name__ == "__main__":
    env = GridWorld()
    
    # 运行 QAC
    theta, w, rewards = run_qac(env, num_episodes=1000, max_steps=500, alpha_theta=0.001, alpha_w=0.01)
    
    # 创建结果目录
    os.makedirs("results", exist_ok=True)
    
    # 计算最终策略和状态值
    V_final = np.zeros(env.num_states)
    pi_final = np.zeros(env.num_states)
    for s in range(env.num_states):
        if env.is_terminal(s):
            V_final[s] = 0.0
            pi_final[s] = 0  # 终止状态动作无所谓
        else:
            phi = get_state_feature(s, env)
            probs = softmax_policy(s, theta, env)
            q_vals = np.array([phi.dot(w[a]) for a in range(env.num_actions)])
            V_final[s] = np.sum(probs * q_vals)
            pi_final[s] = np.argmax(probs)
            
    # 打印最终值
    print("QAC 训练完成！最终状态值：")
    print(V_final.reshape(env.size, env.size))
    
    # 绘制学习曲线 (滑动平均)
    window = 30
    smoothed_rewards = np.convolve(rewards, np.ones(window)/window, mode='valid')
    plt.figure(figsize=(8, 5))
    plt.plot(range(window, len(rewards)+1), smoothed_rewards, label="QAC", color="#3498db")
    plt.title("QAC 算法学习曲线 (滑动平均窗口=30)")
    plt.xlabel("Episode")
    plt.ylabel("总奖励")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig("results/task1_qac_learning.png", dpi=150)
    plt.close()
    
    # 绘制状态值热力图
    plot_value_heatmap(V_final, env, title="QAC 最终状态值函数", save_path="results/task1_qac_values.png")
    
    # 绘制策略箭头图
    plot_policy_arrows(pi_final, env, title="QAC 最终最优策略", V=V_final, save_path="results/task1_qac_policy.png")
