import os
import numpy as np
import matplotlib.pyplot as plt
from env import GridWorld
from utils import plot_policy_arrows, ensure_dir

def choose_action(state, Q, epsilon):
    if np.random.rand() < epsilon:
        return np.random.randint(4)
    else:
        q_vals = Q[state[0], state[1]]
        max_q = np.max(q_vals)
        best_actions = np.where(q_vals == max_q)[0]
        return np.random.choice(best_actions)

def run_n_step_sarsa_trial(n, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=500, max_steps=200):
    env = GridWorld(size=5, gamma=gamma)
    Q = np.zeros((5, 5, 4))
    
    rewards_history = []
    steps_history = []
    success_history = []
    
    for ep in range(num_episodes):
        state = env.reset()
        action = choose_action(state, Q, epsilon)
        
        states = [state]
        actions = [action]
        rewards = [0.0]
        
        T = float('inf')
        t = 0
        cumulative_reward = 0
        success = 0
        
        while True:
            if t < T:
                next_state, reward, done, _ = env.step(actions[t])
                states.append(next_state)
                rewards.append(reward)
                cumulative_reward += reward
                
                if done:
                    T = t + 1
                    success = 1
                elif t + 1 >= max_steps:
                    T = t + 1
                    success = 0
                else:
                    next_action = choose_action(next_state, Q, epsilon)
                    actions.append(next_action)
                    
            tau = t - n + 1
            if tau >= 0:
                G = 0.0
                for i in range(tau + 1, min(tau + n, T) + 1):
                    G += (gamma ** (i - tau - 1)) * rewards[i]
                if tau + n < T:
                    s_tn = states[tau + n]
                    a_tn = actions[tau + n]
                    G += (gamma ** n) * Q[s_tn[0], s_tn[1], a_tn]
                
                s_tau = states[tau]
                a_tau = actions[tau]
                Q[s_tau[0], s_tau[1], a_tau] += alpha * (G - Q[s_tau[0], s_tau[1], a_tau])
                
            if tau == T - 1:
                break
            t += 1
            
        rewards_history.append(cumulative_reward)
        steps_history.append(min(t, max_steps))
        success_history.append(success)
        
    return Q, rewards_history, steps_history, success_history

def main():
    np.random.seed(42)
    os.makedirs("results", exist_ok=True)
    
    n_values = [1, 3, 5]
    num_trials = 20
    num_episodes = 500
    
    # Store averaged results for plotting
    # Keys: n value -> 1D array of length num_episodes
    all_rewards = {}
    all_steps = {}
    all_success_rates = {}
    
    # Representative Q tables for generating policy arrows (from trial 0)
    rep_Q = {}
    
    for n in n_values:
        print(f"Running n-step Sarsa with n={n}...")
        trial_rewards = np.zeros((num_trials, num_episodes))
        trial_steps = np.zeros((num_trials, num_episodes))
        trial_success = np.zeros((num_trials, num_episodes))
        
        for trial in range(num_trials):
            Q, rewards, steps, success = run_n_step_sarsa_trial(
                n=n, alpha=0.1, epsilon=0.1, gamma=0.9, num_episodes=num_episodes, max_steps=200
            )
            trial_rewards[trial] = rewards
            trial_steps[trial] = steps
            trial_success[trial] = success
            
            if trial == 0:
                rep_Q[n] = Q.copy()
                
        all_rewards[n] = np.mean(trial_rewards, axis=0)
        all_steps[n] = np.mean(trial_steps, axis=0)
        all_success_rates[n] = np.mean(trial_success, axis=0)
        
    # Plot Cumulative Rewards
    plt.figure(figsize=(10, 6))
    for n in n_values:
        plt.plot(all_rewards[n], label=f'{n}-step Sarsa')
    plt.xlabel('Episode')
    plt.ylabel('Episode Reward')
    plt.title('Average Episode Reward (20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task3_rewards.png', dpi=300)
    plt.close()
    
    # Plot Steps to Goal
    plt.figure(figsize=(10, 6))
    for n in n_values:
        plt.plot(all_steps[n], label=f'{n}-step Sarsa')
    plt.xlabel('Episode')
    plt.ylabel('Steps to Goal')
    plt.title('Average Steps to Goal (20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task3_steps.png', dpi=300)
    plt.close()
    
    # Plot Success Rate
    plt.figure(figsize=(10, 6))
    for n in n_values:
        plt.plot(all_success_rates[n], label=f'{n}-step Sarsa')
    plt.xlabel('Episode')
    plt.ylabel('Success Rate')
    plt.title('Success Rate (20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/task3_success_rate.png', dpi=300)
    plt.close()
    
    # Save Policy Arrow Plots
    for n in n_values:
        plot_policy_arrows(rep_Q[n], env_size=5, goal=(4, 4),
                           title=f"Greedy Policy of {n}-step Sarsa (Trial 0)",
                           filepath=f"results/task3_policy_n{n}.png")
        
    print("Task 3 complete. Plots and policy arrows saved.")

if __name__ == '__main__':
    main()
