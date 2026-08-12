import os
import numpy as np
import matplotlib.pyplot as plt
from env import GridWorld
from utils import plot_heatmap, ensure_dir

def get_random_non_terminal_state(size=5, goal=(4, 4)):
    while True:
        r = np.random.randint(size)
        c = np.random.randint(size)
        if (r, c) != goal:
            return (r, c)

def run_td0_episode(env, V, N_visits, alpha_type, alpha_val, max_steps=1000):
    state = get_random_non_terminal_state(env.size, env.goal)
    env.reset(state)
    
    steps = 0
    done = False
    while not done and steps < max_steps:
        action = np.random.randint(4)  # random policy
        curr_state = env.state
        next_state, reward, done, _ = env.step(action)
        
        r, c = curr_state
        nr, nc = next_state
        
        # Determine step size
        if alpha_type == 'constant':
            lr = alpha_val
        elif alpha_type == 'decay':
            N_visits[r, c] += 1
            lr = 1.0 / N_visits[r, c]
        else:
            lr = alpha_val
            
        # TD(0) update
        # If next_state is goal, V[nr, nc] is 0
        v_next = V[nr, nc] if not done else 0.0
        td_target = reward + env.gamma * v_next
        V[r, c] += lr * (td_target - V[r, c])
        
        steps += 1

def run_first_visit_mc_episode(env, V, N_visits, max_steps=1000):
    state = get_random_non_terminal_state(env.size, env.goal)
    env.reset(state)
    
    trajectory = []
    steps = 0
    done = False
    while not done and steps < max_steps:
        action = np.random.randint(4)
        curr_state = env.state
        next_state, reward, done, _ = env.step(action)
        trajectory.append((curr_state, reward))
        steps += 1
        
    # Process return and update V
    G = 0
    visited_states = set()
    # Backward sweep for First-Visit MC
    for t in reversed(range(len(trajectory))):
        s, r = trajectory[t]
        G = r + env.gamma * G
        if s not in visited_states:
            visited_states.add(s)
            N_visits[s] += 1
            lr = 1.0 / N_visits[s]
            V[s] += lr * (G - V[s])

def calculate_rmse(V, V_pi):
    return np.sqrt(np.mean((V - V_pi) ** 2))

def main():
    np.random.seed(42)
    os.makedirs("results", exist_ok=True)
    
    # Load DP benchmark
    if not os.path.exists("results/dp_benchmark.npy"):
        print("DP benchmark not found. Running dp_benchmark.py first...")
        import dp_benchmark
        V_pi = dp_benchmark.run_dp_policy_evaluation()
    else:
        V_pi = np.load("results/dp_benchmark.npy")
        
    num_episodes = 500
    num_trials = 20
    
    # Storage for RMSE trajectories
    # Methods: 0: TD_0.05, 1: TD_0.1, 2: TD_1/N, 3: FV_MC
    rmse_history = np.zeros((4, num_trials, num_episodes))
    
    # For saving heatmaps of a single representative run (trial 0)
    final_V_grid = {
        'TD_0.05': None,
        'TD_0.1': None,
        'TD_1/N': None,
        'FV_MC': None
    }
    
    for trial in range(num_trials):
        # Initialize Value tables and visits
        # TD(0) alpha = 0.05
        V_td_05 = np.zeros((5, 5))
        # TD(0) alpha = 0.1
        V_td_1 = np.zeros((5, 5))
        # TD(0) alpha = 1/N
        V_td_decay = np.zeros((5, 5))
        N_td_decay = np.zeros((5, 5))
        # First Visit MC
        V_mc = np.zeros((5, 5))
        N_mc = np.zeros((5, 5))
        
        env = GridWorld(size=5, gamma=0.9)
        
        for ep in range(num_episodes):
            # 1. Run TD(0) alpha = 0.05
            run_td0_episode(env, V_td_05, None, 'constant', 0.05)
            rmse_history[0, trial, ep] = calculate_rmse(V_td_05, V_pi)
            
            # 2. Run TD(0) alpha = 0.1
            run_td0_episode(env, V_td_1, None, 'constant', 0.1)
            rmse_history[1, trial, ep] = calculate_rmse(V_td_1, V_pi)
            
            # 3. Run TD(0) alpha = 1/N
            run_td0_episode(env, V_td_decay, N_td_decay, 'decay', 0.0)
            rmse_history[2, trial, ep] = calculate_rmse(V_td_decay, V_pi)
            
            # 4. Run First-Visit MC
            # Using dict/array for coordinate visits
            # For simplicity, convert 2D coordinates to index in function or pass matrix
            run_first_visit_mc_episode(env, V_mc, N_mc)
            rmse_history[3, trial, ep] = calculate_rmse(V_mc, V_pi)
            
        if trial == 0:
            final_V_grid['TD_0.05'] = V_td_05.copy()
            final_V_grid['TD_0.1'] = V_td_1.copy()
            final_V_grid['TD_1/N'] = V_td_decay.copy()
            final_V_grid['FV_MC'] = V_mc.copy()
            
        print(f"Finished Trial {trial+1}/{num_trials}")
        
    # Calculate average RMSE curves
    avg_rmse = np.mean(rmse_history, axis=1)
    
    # Plot RMSE curves
    plt.figure(figsize=(10, 6))
    plt.plot(avg_rmse[0], label='TD(0) ($\\alpha=0.05$)', color='blue')
    plt.plot(avg_rmse[1], label='TD(0) ($\\alpha=0.1$)', color='cyan')
    plt.plot(avg_rmse[2], label='TD(0) ($\\alpha_t=1/N(s)$)', color='green')
    plt.plot(avg_rmse[3], label='First-Visit MC', color='red')
    plt.xlabel('Episode')
    plt.ylabel('RMSE')
    plt.title('RMSE of Value Function Estimation (Average of 20 Trials)')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.savefig('results/rmse_comparison.png', dpi=300)
    plt.close()
    
    # Save Heatmaps for Trial 0
    for name, V in final_V_grid.items():
        plot_heatmap(V, f"Value Function Map via {name} (Trial 0)", f"results/heatmap_{name.replace('/', '_')}.png")
        
    print("Task 2 complete. RMSE comparison and heatmaps saved.")

if __name__ == '__main__':
    main()
