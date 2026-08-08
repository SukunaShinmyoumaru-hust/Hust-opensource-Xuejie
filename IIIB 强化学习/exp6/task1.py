import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from gridworld import GridWorld
from policy import SoftmaxPolicy

# Setup directories
results_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results")
os.makedirs(results_dir, exist_ok=True)

# 确保中文显示正常
plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'Source Han Sans CN',
                                    'Noto Sans CJK JP', 'SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

def plot_initial_state_probs(policy, state_coords=(3, 3), save_path=None):
    """
    Plot bar chart of action probabilities for a specific state.
    state_coords: (row, col) 1-indexed, e.g., (3, 3) corresponds to row=2, col=2 (0-indexed).
    """
    row = state_coords[0] - 1
    col = state_coords[1] - 1
    env = GridWorld()
    s = env.pos_to_state(row, col)
    
    probs = policy.get_probs(s)
    actions = ['上 (↑)', '下 (↓)', '左 (←)', '右 (→)']
    
    plt.figure(figsize=(6, 4))
    bars = plt.bar(actions, probs, color=['#3498db', '#e74c3c', '#2ecc71', '#f39c12'], edgecolor='black', alpha=0.8)
    plt.ylim(0, 1.0)
    plt.title(f"状态 {state_coords} 初始动作概率分布", fontsize=14)
    plt.xlabel("动作", fontsize=12)
    plt.ylabel("选择概率", fontsize=12)
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    # Add values on top of bars
    for bar in bars:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2.0, yval + 0.02, f"{yval:.4f}", ha='center', va='bottom', fontweight='bold')
        
    plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
        print(f"[保存] {save_path}")
    plt.close()

def plot_policy_probs(policy, env, title="策略概率分布", save_path=None):
    """
    Visualize the policy probabilities across the entire gridworld using arrows.
    Arrow directions represent the 4 actions, length and color opacity represent probabilities.
    """
    fig, ax = plt.subplots(figsize=(7, 7))
    bg = np.zeros((env.size, env.size))
    ax.imshow(bg, cmap='Greys', alpha=0.05, interpolation='nearest')
    
    # 0: Up (y decreases), 1: Down (y increases), 2: Left (x decreases), 3: Right (x increases)
    for s in range(env.num_states):
        row, col = env.state_to_pos(s)
        if env.is_terminal(s):
            ax.text(col, row, r'$\mathbf{\bigstar}$', ha='center', va='center', fontsize=24, color='gold')
            continue
            
        probs = policy.get_probs(s)
        for a, p in enumerate(probs):
            if p < 0.01:
                continue
            if a == 0:    # Up (row decreases)
                dx, dy = 0, -0.45 * p
            elif a == 1:  # Down (row increases)
                dx, dy = 0, 0.45 * p
            elif a == 2:  # Left (col decreases)
                dx, dy = -0.45 * p, 0
            elif a == 3:  # Right (col increases)
                dx, dy = 0.45 * p, 0
                
            ax.annotate('', xy=(col + dx, row + dy), xytext=(col, row),
                        arrowprops=dict(arrowstyle='->', color='#2c3e50', alpha=float(p), lw=1.0 + 3.0*p))
            
            # Print value near the arrow head
            # ax.text(col + dx*0.8, row + dy*0.8, f"{p:.2f}", fontsize=7, color='blue', alpha=float(p))

    ax.set_xlim(-0.5, env.size - 0.5)
    ax.set_ylim(env.size - 0.5, -0.5)
    ax.set_xticks(range(env.size))
    ax.set_yticks(range(env.size))
    ax.set_xticklabels(range(1, env.size + 1))
    ax.set_yticklabels(range(1, env.size + 1))
    ax.set_title(title, fontsize=14)
    ax.set_xlabel("列 (y 坐标)", fontsize=12)
    ax.set_ylabel("行 (x 坐标)", fontsize=12)
    ax.grid(True, linewidth=0.5, alpha=0.3)
    plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
        print(f"[保存] {save_path}")
    plt.close()

if __name__ == "__main__":
    # Fix seed
    np.random.seed(42)
    
    # Initialize environment and policy
    env = GridWorld()
    policy = SoftmaxPolicy(seed=42)
    
    # 1. Plot action probabilities for state (3,3)
    state_33_path = os.path.join(results_dir, "task1_state_33_probs.png")
    plot_initial_state_probs(policy, state_coords=(3, 3), save_path=state_33_path)
    
    # 2. Plot overall policy probabilities
    overall_policy_path = os.path.join(results_dir, "task1_initial_policy.png")
    plot_policy_probs(policy, env, title="初始参数化策略概率分布 (Softmax)", save_path=overall_policy_path)
    
    print("Task 1 completed successfully.")
