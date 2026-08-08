import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from gridworld import GridWorld
from policy import SoftmaxPolicy
from task1 import plot_policy_probs
from visualization import plot_policy_arrows

# Setup directories
results_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results")
os.makedirs(results_dir, exist_ok=True)

# 确保中文显示正常
plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'Source Han Sans CN',
                                    'Noto Sans CJK JP', 'SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

def run_reinforce(env, policy, num_episodes=1000, max_steps=500, gamma=0.9):
    """
    Train policy using the REINFORCE update specified in the task book.
    """
    episode_rewards = []
    
    # For Task 3: track action probabilities for start state (1,1) -> state index 0
    # Let's save them at each episode
    start_state_probs = []
    
    for ep in range(num_episodes):
        s = 0  # Start state: top-left corner (0,0) or state index 0
        trajectory = []
        total_reward = 0
        
        # Track start state probabilities before updating
        start_state_probs.append(policy.get_probs(0))
        
        for step in range(max_steps):
            a = policy.choose_action(s)
            next_s, reward = env.step(s, a)
            trajectory.append((s, a, reward))
            total_reward += reward
            
            if env.is_terminal(next_s):
                break
            s = next_s
            
        episode_rewards.append(total_reward)
        
        policy.update_trajectory(trajectory, gamma=gamma)
        
        if (ep + 1) % 100 == 0:
            avg_rew = np.mean(episode_rewards[-100:])
            print(f"Episode {ep+1}/{num_episodes} - Last 100 Avg Reward: {avg_rew:.2f} - Steps: {len(trajectory)}")
            
    return episode_rewards, np.array(start_state_probs)

if __name__ == "__main__":
    # Fix seed for reproducibility
    np.random.seed(42)
    
    # Initialize environment and policy
    # environment: 5x5, every step -1, boundary bump -1, target +1
    env = GridWorld(r_step=-1.0, r_boundary=-1.0, r_target=1.0, gamma=0.9)
    policy = SoftmaxPolicy(seed=42, lr=0.001)
    
    # Run REINFORCE training
    num_episodes = 1000
    print("Running REINFORCE...")
    rewards, start_probs = run_reinforce(
        env, policy, num_episodes=num_episodes, max_steps=500, gamma=0.9
    )
    
    # 1. Plot reward curve
    plt.figure(figsize=(10, 5))
    plt.plot(rewards, color='blue', alpha=0.3, label='每回合奖励 (Raw)')
    # Compute moving average
    window = 50
    smoothed_rewards = np.convolve(rewards, np.ones(window)/window, mode='valid')
    plt.plot(range(window-1, num_episodes), smoothed_rewards, color='red', linewidth=2, label=f'{window}回合滑动平均')
    plt.title("原始 REINFORCE 训练曲线（回合总奖励）", fontsize=14)
    plt.xlabel("训练回合 (Episode)", fontsize=12)
    plt.ylabel("总奖励 (Total Reward)", fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend(fontsize=11)
    reward_plot_path = os.path.join(results_dir, "task2_reward_curve.png")
    plt.savefig(reward_plot_path, dpi=150)
    print(f"[保存] {reward_plot_path}")
    plt.close()

    # 2. Visualize final policy as greedy actions
    # Extract greedy actions for non-terminal states
    greedy_policy = np.zeros(env.num_states, dtype=int)
    for s in range(env.num_states):
        if env.is_terminal(s):
            continue
        greedy_policy[s] = np.argmax(policy.get_probs(s))
        
    final_greedy_path = os.path.join(results_dir, "task2_final_greedy_policy.png")
    plot_policy_arrows(greedy_policy, env, title="原始 REINFORCE 学习到的贪心策略", save_path=final_greedy_path)
    
    # 3. Visualize final policy probability distributions
    final_policy_probs_path = os.path.join(results_dir, "task2_final_policy_probs.png")
    plot_policy_probs(policy, env, title="原始 REINFORCE 训练后策略概率", save_path=final_policy_probs_path)
    
    # Save the tracked start state probabilities for task 3 to load or we can use it in task 3
    # Let's save them as a numpy array for easy access
    np.save(os.path.join(results_dir, "start_probs.npy"), start_probs)
    np.save(os.path.join(results_dir, "rewards.npy"), rewards)
    print("Task 2 completed successfully.")
