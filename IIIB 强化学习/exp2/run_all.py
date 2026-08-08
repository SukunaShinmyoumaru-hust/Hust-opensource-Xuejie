"""
实验二：蒙特卡罗方法 — 主入口与算法对比分析
==========================================
运行任务1的硬币投掷实验、MC Basic不同H的实验，
运行任务2的MC Exploring Starts和MC ε-Greedy实验，
并在同一坐标系下对比这三种算法的收敛速度与样本效率。

用法：
    cd exp2
    python run_all.py
"""

import os
import sys
import numpy as np
import matplotlib
matplotlib.use('Agg')  # 非交互模式，直接保存图片
import matplotlib.pyplot as plt

# 确保当前目录在 sys.path 中
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from coin_flip import run_coin_flip_experiment
from mc_basic import run_mc_basic_experiments, mc_basic
from mc_exploring_starts import run_mc_es_experiment, mc_exploring_starts, run_value_iteration_ground_truth, evaluate_policy_return
from mc_epsilon_greedy import run_mc_epsilon_greedy_experiments, mc_epsilon_greedy


def run_comprehensive_comparison(save_dir: str = "results"):
    """在相同的 r_forbidden = -10.0 环境下，对比三种蒙特卡罗算法的收敛速度与性能。"""
    print("\n" + "=" * 70)
    print(" 综合对比实验：三种蒙特卡罗算法的收敛性与样本效率 ")
    print("=" * 70)
    
    # 统一环境参数
    env = GridWorld(
        size=5,
        goal=(4, 4),
        forbidden_states=[(1, 1), (2, 3)],
        r_boundary=-1.0,
        r_forbidden=-10.0,
        r_target=1.0,
        r_step=0.0,
        gamma=0.9
    )
    
    V_star, _ = run_value_iteration_ground_truth(env)
    num_episodes = 5000
    
    # 1. 运行 MC Exploring Starts
    print("运行 MC Exploring Starts...")
    _, _, _, rmse_es, returns_es = mc_exploring_starts(
        env, num_episodes=num_episodes, seed=42, V_star=V_star
    )
    
    # 2. 运行 MC ε-Greedy (ε = 0.1)
    print("运行 MC ε-Greedy (ε = 0.1)...")
    _, _, _, rmse_eg_01, returns_eg_01 = mc_epsilon_greedy(
        env, epsilon_mode='fixed', epsilon_val=0.1, num_episodes=num_episodes, seed=42, V_star=V_star
    )
    
    # 3. 运行 MC ε-Greedy (动态衰减 ε)
    print("运行 MC ε-Greedy (动态衰减)...")
    _, _, _, rmse_eg_decay, returns_eg_decay = mc_epsilon_greedy(
        env, epsilon_mode='decay', num_episodes=num_episodes, seed=42, V_star=V_star
    )
    
    # 4. 运行 MC Basic (H = 30) 并评估每一步策略
    # 由于 MC Basic 是批处理式的，我们在每个策略迭代步后计算累积回报，
    # 每次迭代耗费 24 * 4 = 96 个回合。
    print("运行 MC Basic (H = 30)...")
    n_states = env.num_states
    n_actions = env.num_actions
    policy = np.zeros(n_states)
    
    # 记录每个迭代（每96个回合）产生的 policy 和 value
    policies_history = [policy.copy()]
    rmses_history = []
    
    # 计算初始 policy 的 RMSE 和评估值
    V_init = np.zeros(n_states)
    rmse_init = np.sqrt(np.mean((V_init - V_star)**2))
    rmses_history.append(rmse_init)
    
    max_basic_iters = 53 # 53 * 96 = 5088 episodes
    for it in range(1, max_basic_iters + 1):
        Q_new = np.zeros((n_states, n_actions))
        for s in env.get_non_terminal_states():
            for a in range(n_actions):
                from mc_basic import generate_mc_basic_episode
                G = generate_mc_basic_episode(env, s, a, policy, horizon=30)
                Q_new[s, a] = G
                
        for s in range(n_states):
            if env.is_terminal(s):
                Q_new[s, :] = 0.0
                
        policy = np.argmax(Q_new, axis=1)
        policies_history.append(policy.copy())
        
        # 计算当前值函数的 RMSE
        V = np.max(Q_new, axis=1)
        for s in range(n_states):
            if env.is_terminal(s):
                V[s] = 0.0
        rmse = np.sqrt(np.mean((V - V_star)**2))
        rmses_history.append(rmse)
        
    # 现在根据特征回合点抽取评估值和 RMSE
    returns_basic = []
    rmse_basic = []
    
    # 对于每个评估回合点 (10, 20, ..., 5000)
    for ep in range(10, num_episodes + 1, 10):
        it_idx = ep // 96
        active_policy = policies_history[min(it_idx, len(policies_history) - 1)]
        eval_ret = evaluate_policy_return(env, active_policy, start_pos=(0, 0))
        returns_basic.append(eval_ret)
        
    # 对于每个训练回合点 (1, 2, ..., 5000)
    for ep in range(1, num_episodes + 1):
        it_idx = ep // 96
        rmse_val = rmses_history[min(it_idx, len(rmses_history) - 1)]
        rmse_basic.append(rmse_val)
    
    # 5. 绘制三个算法的累计回报对比图
    plt.figure(figsize=(10, 6))
    x_eval = np.arange(10, num_episodes + 1, 10)
    
    # 平滑窗口
    window = 10
    def smooth(y):
        return np.convolve(y, np.ones(window)/window, mode='valid')
        
    x_smooth = x_eval[window-1:]
    
    plt.plot(x_smooth, smooth(returns_basic[:len(x_eval)]), color='#e74c3c', linewidth=2, label="MC Basic (H=30)")
    plt.plot(x_smooth, smooth(returns_es), color='#3498db', linewidth=2, label="MC Exploring Starts")
    plt.plot(x_smooth, smooth(returns_eg_01), color='#e67e22', linewidth=2, label="MC ε-Greedy (ε=0.1)")
    plt.plot(x_smooth, smooth(returns_eg_decay), color='#2ecc71', linewidth=2, label="MC ε-Greedy (动态衰减 ε)")
    
    plt.xlabel("回合数 (Episodes)", fontsize=11)
    plt.ylabel("测试累计回报 (从左上角出发，滑动平均)", fontsize=11)
    plt.title("三种蒙特卡罗控制算法的学习曲线对比 (r_forbidden=-10.0)", fontsize=13)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    
    save_path = os.path.join(save_dir, "mc_comparison_rewards.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()
    
    # 6. 绘制三个算法的 RMSE 对比图
    plt.figure(figsize=(10, 6))
    plt.plot(range(1, len(rmse_basic) + 1), rmse_basic, color='#e74c3c', alpha=0.8, label="MC Basic (H=30)")
    plt.plot(range(1, len(rmse_es) + 1), rmse_es, color='#3498db', alpha=0.8, label="MC Exploring Starts")
    plt.plot(range(1, len(rmse_eg_01) + 1), rmse_eg_01, color='#e67e22', alpha=0.8, label="MC ε-Greedy (ε=0.1)")
    plt.plot(range(1, len(rmse_eg_decay) + 1), rmse_eg_decay, color='#2ecc71', alpha=0.8, label="MC ε-Greedy (动态衰减 ε)")
    
    plt.xlabel("回合数 (Episodes)", fontsize=11)
    plt.ylabel("RMSE (与真值 V* 相比)", fontsize=11)
    plt.yscale("log")
    plt.title("三种蒙特卡罗算法的 RMSE 收敛速度对比", fontsize=13)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    
    save_path = os.path.join(save_dir, "mc_comparison_rmse.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()


def main():
    save_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results")
    os.makedirs(save_dir, exist_ok=True)

    print("╔" + "═" * 68 + "╗")
    print("║" + "实验二：蒙特卡罗方法".center(58) + "║")
    print("╚" + "═" * 68 + "╝")
    print(f"\n结果保存目录: {save_dir}\n")

    # ---- 任务 1 ----
    run_coin_flip_experiment(save_dir=save_dir)
    run_mc_basic_experiments(save_dir=save_dir)

    # ---- 任务 2 ----
    run_mc_es_experiment(save_dir=save_dir)
    run_mc_epsilon_greedy_experiments(save_dir=save_dir)
    
    # ---- 综合对比 ----
    from gridworld import GridWorld # 必须在函数前确保导入 GridWorld
    global GridWorld
    run_comprehensive_comparison(save_dir=save_dir)

    # ---- 汇总 ----
    print("\n" + "=" * 70)
    print("全部实验完成！")
    print("=" * 70)
    print(f"\n所有图表已保存到: {save_dir}/")
    print("\n生成的文件:")
    for f in sorted(os.listdir(save_dir)):
        filepath = os.path.join(save_dir, f)
        size_kb = os.path.getsize(filepath) / 1024
        print(f"  📊 {f} ({size_kb:.1f} KB)")


if __name__ == "__main__":
    main()
