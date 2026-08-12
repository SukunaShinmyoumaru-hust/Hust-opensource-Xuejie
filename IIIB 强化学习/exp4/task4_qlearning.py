import os
import numpy as np
import matplotlib.pyplot as plt
from env import GridWorld, CliffWalking
from utils import plot_policy_arrows, plot_cliffwalking_policy, ensure_dir

def choose_action(state, Q, epsilon):
    if np.random.rand() < epsilon:
        return np.random.randint(4)
    else:
        q_vals = Q[state[0], state[1]]
        max_q = np.max(q_vals)
        best_actions = np.where(q_vals == max_q)[0]
        return np.random.choice(best_actions)

def run_sarsa_trial(env, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=500, max_steps=200):
    Q = np.zeros((env.size, env.size, 4))
    rewards_history = []
    steps_history = []
    success_history = []
    
    for ep in range(num_episodes):
        state = env.reset()
        action = choose_action(state, Q, epsilon)
        cumulative_reward = 0
        steps = 0
        done = False
        
        while not done and steps < max_steps:
            next_state, reward, done, _ = env.step(action)
            cumulative_reward += reward
            
            if done:
                td_target = reward
                next_action = 0  # dummy
            else:
                next_action = choose_action(next_state, Q, epsilon)
                td_target = reward + gamma * Q[next_state[0], next_state[1], next_action]
                
            Q[state[0], state[1], action] += alpha * (td_target - Q[state[0], state[1], action])
            state = next_state
            action = next_action
            steps += 1
            
        rewards_history.append(cumulative_reward)
        steps_history.append(steps)
        success_history.append(1 if done else 0)
        
    return Q, rewards_history, steps_history, success_history

def run_qlearning_trial(env, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=500, max_steps=200):
    Q = np.zeros((env.size, env.size, 4))
    rewards_history = []
    steps_history = []
    success_history = []
    
    for ep in range(num_episodes):
        state = env.reset()
        cumulative_reward = 0
        steps = 0
        done = False
        
        while not done and steps < max_steps:
            action = choose_action(state, Q, epsilon)
            next_state, reward, done, _ = env.step(action)
            cumulative_reward += reward
            
            if done:
                td_target = reward
            else:
                td_target = reward + gamma * np.max(Q[next_state[0], next_state[1]])
                
            Q[state[0], state[1], action] += alpha * (td_target - Q[state[0], state[1], action])
            state = next_state
            steps += 1
            
        rewards_history.append(cumulative_reward)
        steps_history.append(steps)
        success_history.append(1 if done else 0)
        
    return Q, rewards_history, steps_history, success_history

def run_offline_qlearning(env, dataset_size=50000, train_steps=100000, alpha=0.1, gamma=0.9):
    # 1. Generate dataset using random policy
    dataset = []
    state = env.reset()
    for _ in range(dataset_size):
        action = np.random.randint(4)
        next_state, reward, done, _ = env.step(action)
        dataset.append((state, action, reward, next_state, done))
        if done:
            state = env.reset()
        else:
            state = next_state
            
    # 2. Train Q-table offline
    Q = np.zeros((env.size, env.size, 4))
    for step in range(train_steps):
        # Sample randomly from dataset
        idx = np.random.randint(dataset_size)
        s, a, r, s_next, done = dataset[idx]
        
        if done:
            td_target = r
        else:
            td_target = r + gamma * np.max(Q[s_next[0], s_next[1]])
            
        Q[s[0], s[1], a] += alpha * (td_target - Q[s[0], s[1], a])
        
    return Q

def run_sarsa_cliff(env, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=500, max_steps=1000):
    Q = np.zeros((env.height, env.width, 4))
    rewards_history = []
    
    for ep in range(num_episodes):
        state = env.reset()
        action = choose_action(state, Q, epsilon)
        cumulative_reward = 0
        steps = 0
        done = False
        
        while not done and steps < max_steps:
            next_state, reward, done, _ = env.step(action)
            cumulative_reward += reward
            
            if done:
                td_target = reward
                next_action = 0
            else:
                next_action = choose_action(next_state, Q, epsilon)
                td_target = reward + gamma * Q[next_state[0], next_state[1], next_action]
                
            Q[state[0], state[1], action] += alpha * (td_target - Q[state[0], state[1], action])
            state = next_state
            action = next_action
            steps += 1
            
        rewards_history.append(cumulative_reward)
        
    return Q, rewards_history

def run_qlearning_cliff(env, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=500, max_steps=1000):
    Q = np.zeros((env.height, env.width, 4))
    rewards_history = []
    
    for ep in range(num_episodes):
        state = env.reset()
        cumulative_reward = 0
        steps = 0
        done = False
        
        while not done and steps < max_steps:
            action = choose_action(state, Q, epsilon)
            next_state, reward, done, _ = env.step(action)
            cumulative_reward += reward
            
            if done:
                td_target = reward
            else:
                td_target = reward + gamma * np.max(Q[next_state[0], next_state[1]])
                
            Q[state[0], state[1], action] += alpha * (td_target - Q[state[0], state[1], action])
            state = next_state
            steps += 1
            
        rewards_history.append(cumulative_reward)
        
    return Q, rewards_history

def main():
    np.random.seed(42)
    os.makedirs("results", exist_ok=True)
    
    num_trials = 20
    num_episodes = 500
    
    # ------------------ Task 4: Online Q-learning vs Sarsa in GridWorld ------------------
    print("Running GridWorld Q-learning and Sarsa comparison...")
    env_grid = GridWorld(size=5, gamma=0.9)
    
    sarsa_rewards = np.zeros((num_trials, num_episodes))
    sarsa_steps = np.zeros((num_trials, num_episodes))
    sarsa_success = np.zeros((num_trials, num_episodes))
    
    q_rewards = np.zeros((num_trials, num_episodes))
    q_steps = np.zeros((num_trials, num_episodes))
    q_success = np.zeros((num_trials, num_episodes))
    
    rep_Q_sarsa = None
    rep_Q_q = None
    
    for trial in range(num_trials):
        # Sarsa
        Q_s, r_s, steps_s, success_s = run_sarsa_trial(env_grid, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=num_episodes)
        sarsa_rewards[trial] = r_s
        sarsa_steps[trial] = steps_s
        sarsa_success[trial] = success_s
        
        # Q-learning
        Q_q, r_q, steps_q, success_q = run_qlearning_trial(env_grid, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=num_episodes)
        q_rewards[trial] = r_q
        q_steps[trial] = steps_q
        q_success[trial] = success_q
        
        if trial == 0:
            rep_Q_sarsa = Q_s.copy()
            rep_Q_q = Q_q.copy()
            
    # Calculate means
    avg_s_rewards = np.mean(sarsa_rewards, axis=0)
    avg_s_steps = np.mean(sarsa_steps, axis=0)
    avg_s_success = np.mean(sarsa_success, axis=0)
    
    avg_q_rewards = np.mean(q_rewards, axis=0)
    avg_q_steps = np.mean(q_steps, axis=0)
    avg_q_success = np.mean(q_success, axis=0)
    
    # Plot GridWorld Cumulative Rewards comparison
    plt.figure(figsize=(10, 6))
    plt.plot(avg_s_rewards, label='Sarsa', color='tab:orange')
    plt.plot(avg_q_rewards, label='Q-learning', color='tab:blue')
    plt.xlabel('Episode')
    plt.ylabel('Episode Reward')
    plt.title('GridWorld: Episode Reward Comparison (20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task4_grid_rewards.png', dpi=300)
    plt.close()
    
    # Plot GridWorld Steps to Goal comparison
    plt.figure(figsize=(10, 6))
    plt.plot(avg_s_steps, label='Sarsa', color='tab:orange')
    plt.plot(avg_q_steps, label='Q-learning', color='tab:blue')
    plt.xlabel('Episode')
    plt.ylabel('Steps to Goal')
    plt.title('GridWorld: Steps to Goal Comparison (20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task4_grid_steps.png', dpi=300)
    plt.close()
    
    # Plot GridWorld Success Rate comparison
    plt.figure(figsize=(10, 6))
    plt.plot(avg_s_success, label='Sarsa', color='tab:orange')
    plt.plot(avg_q_success, label='Q-learning', color='tab:blue')
    plt.xlabel('Episode')
    plt.ylabel('Success Rate')
    plt.title('GridWorld: Success Rate Comparison (20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task4_grid_success_rate.png', dpi=300)
    plt.close()
    
    # Save GridWorld Policy Arrow Plots
    plot_policy_arrows(rep_Q_sarsa, env_size=5, goal=(4, 4), title="Greedy Policy of Sarsa (Trial 0)", filepath="results/task4_grid_sarsa_policy.png")
    plot_policy_arrows(rep_Q_q, env_size=5, goal=(4, 4), title="Greedy Policy of Q-learning (Trial 0)", filepath="results/task4_grid_q_policy.png")
    
    # ------------------ Task 4: Optional Extension 1 (Offline Q-learning) ------------------
    print("Running Offline Q-learning...")
    Q_offline = run_offline_qlearning(env_grid, dataset_size=50000, train_steps=100000, alpha=0.1, gamma=0.9)
    plot_policy_arrows(Q_offline, env_size=5, goal=(4, 4), title="Greedy Policy of Offline Q-learning", filepath="results/task4_offline_q_policy.png")
    
    # ------------------ Task 4: Optional Extension 2 (Cliff Walking) ------------------
    print("Running Cliff Walking comparison...")
    env_cliff = CliffWalking(gamma=0.9)
    
    cliff_sarsa_rewards = np.zeros((num_trials, num_episodes))
    cliff_q_rewards = np.zeros((num_trials, num_episodes))
    
    rep_Q_cliff_sarsa = None
    rep_Q_cliff_q = None
    
    for trial in range(num_trials):
        Q_cs, r_cs = run_sarsa_cliff(env_cliff, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=num_episodes)
        cliff_sarsa_rewards[trial] = r_cs
        
        Q_cq, r_cq = run_qlearning_cliff(env_cliff, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=num_episodes)
        cliff_q_rewards[trial] = r_cq
        
        if trial == 0:
            rep_Q_cliff_sarsa = Q_cs.copy()
            rep_Q_cliff_q = Q_cq.copy()
            
    avg_cs_rewards = np.mean(cliff_sarsa_rewards, axis=0)
    avg_cq_rewards = np.mean(cliff_q_rewards, axis=0)
    
    # Plot Cliff Walking Episode Returns
    plt.figure(figsize=(10, 6))
    plt.plot(avg_cs_rewards, label='Sarsa (Safe Path)', color='tab:orange')
    plt.plot(avg_cq_rewards, label='Q-learning (Optimal Path)', color='tab:blue')
    plt.xlabel('Episode')
    plt.ylabel('Episode Reward')
    plt.title('Cliff Walking: Episode Reward Comparison (20 Trials)')
    plt.ylim(-150, 0)  # Zoom in on the stable region, early stages can be lower
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task4_cliff_rewards.png', dpi=300)
    plt.close()
    
    # Plot Cliff Walking Policies
    plot_cliffwalking_policy(rep_Q_cliff_sarsa, title="Greedy Policy of Sarsa on Cliff Walking (Trial 0)", filepath="results/task4_cliff_sarsa_policy.png")
    plot_cliffwalking_policy(rep_Q_cliff_q, title="Greedy Policy of Q-learning on Cliff Walking (Trial 0)", filepath="results/task4_cliff_q_policy.png")
    
    print("Task 4 complete. Comparative plots and policy arrows saved.")

if __name__ == '__main__':
    main()
