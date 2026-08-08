import os
import numpy as np
from env import GridWorld
from utils import plot_heatmap, plot_gridworld_schematic

def run_dp_policy_evaluation(size=5, gamma=0.9, theta=1e-10):
    env = GridWorld(size=size, gamma=gamma)
    V = np.zeros((size, size))
    
    # Mapping actions: 0: Up, 1: Down, 2: Left, 3: Right
    actions = [0, 1, 2, 3]
    
    iteration = 0
    while True:
        delta = 0
        new_V = np.zeros((size, size))
        for r in range(size):
            for c in range(size):
                if (r, c) == env.goal:
                    new_V[r, c] = 0.0
                    continue
                
                v_temp = 0.0
                for action in actions:
                    env.state = (r, c)
                    next_state, reward, done, _ = env.step(action)
                    next_r, next_c = next_state
                    
                    # Bellman expectation equation update for random policy (prob = 0.25)
                    v_temp += 0.25 * (reward + gamma * V[next_r, next_c])
                
                new_V[r, c] = v_temp
                delta = max(delta, abs(new_V[r, c] - V[r, c]))
                
        V = new_V
        iteration += 1
        if delta < theta:
            break
            
    print(f"DP Policy Evaluation converged in {iteration} iterations.")
    print("Benchmark State Value V_pi:")
    print(np.round(V, 4))
    return V

if __name__ == "__main__":
    # Ensure results directory exists
    os.makedirs("results", exist_ok=True)
    
    # Calculate DP baseline
    V_pi = run_dp_policy_evaluation()
    
    # Save the baseline to files
    np.save("results/dp_benchmark.npy", V_pi)
    with open("results/dp_benchmark.txt", "w") as f:
        f.write("Benchmark State Value V_pi:\n")
        for r in range(5):
            line = " ".join([f"{V_pi[r, c]:.8f}" for c in range(5)])
            f.write(line + "\n")
            
    # Generate plots
    plot_gridworld_schematic(size=5, start=(0, 0), goal=(4, 4), filepath="results/gridworld_schematic.png")
    plot_heatmap(V_pi, "Benchmark State Value Heatmap ($V_\\pi$)", "results/dp_benchmark_heatmap.png")
    print("DP Benchmark generated and plots saved in 'results/'.")
