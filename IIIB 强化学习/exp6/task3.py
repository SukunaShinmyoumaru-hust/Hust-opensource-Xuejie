import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Setup directories
results_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results")
os.makedirs(results_dir, exist_ok=True)

# 确保中文显示正常
plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'Source Han Sans CN',
                                    'Noto Sans CJK JP', 'SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

if __name__ == "__main__":
    # Load tracked probabilities
    probs_file = os.path.join(results_dir, "start_probs.npy")
    if not os.path.exists(probs_file):
        raise FileNotFoundError(f"Cannot find {probs_file}. Please run task2.py first.")
        
    start_probs = np.load(probs_file)
    num_episodes = len(start_probs)
    
    # Actions: 0: Up (↑), 1: Down (↓), 2: Left (←), 3: Right (→)
    actions = ['上 (↑)', '下 (↓)', '左 (←)', '右 (→)']
    colors = ['#3498db', '#e74c3c', '#2ecc71', '#f39c12']
    
    plt.figure(figsize=(10, 5))
    for a in range(4):
        plt.plot(start_probs[:, a], label=actions[a], color=colors[a], alpha=0.8, linewidth=2)
        
    plt.title("起点状态 (1,1) 各动作概率随训练过程的变化曲线", fontsize=14)
    plt.xlabel("训练回合 (Episode)", fontsize=12)
    plt.ylabel("选择概率 (Probability)", fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend(fontsize=11)
    
    plot_path = os.path.join(results_dir, "task3_start_state_probs.png")
    plt.savefig(plot_path, dpi=150)
    print(f"[保存] {plot_path}")
    plt.close()
    
    print("Task 3 completed successfully.")
