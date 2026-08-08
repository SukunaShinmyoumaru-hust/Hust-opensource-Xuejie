"""
任务 1：随机变量期望值的蒙特卡罗估计（投掷硬币）
==============================================
实现基于模型和基于采样的期望值估计，绘制样本量与估计误差的关系曲线，验证大数定律。
并对比直接平均与增量平均的计算结果。
"""

import os
import numpy as np
import matplotlib.pyplot as plt

# 确保中文显示正常
plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'Source Han Sans CN',
                                    'Noto Sans CJK JP', 'SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False


def simulate_coin_flip(p: float = 0.5, N: int = 1000, seed: int = 42):
    """模拟投掷硬币，计算直接估计和增量估计的路径及其误差。"""
    np.random.seed(seed)
    
    # 真实期望值
    true_mean = p
    
    # 生成 N 个伯努利样本 (0 或 1)
    samples = np.random.binomial(1, p, N)
    
    # 1. 直接估计 (直接平均)
    direct_means = np.zeros(N)
    for k in range(1, N + 1):
        direct_means[k-1] = np.mean(samples[:k])
        
    # 2. 增量估计 (Incremental Estimation)
    # w_k = w_{k-1} - 1/k * (w_{k-1} - x_k)
    incremental_means = np.zeros(N)
    w = 0.0 # 初始值
    for k in range(1, N + 1):
        x_k = samples[k-1]
        w = w - (1.0 / k) * (w - x_k)
        incremental_means[k-1] = w
        
    # 计算误差
    direct_errors = np.abs(direct_means - true_mean)
    incremental_errors = np.abs(incremental_means - true_mean)
    
    return samples, direct_means, incremental_means, direct_errors, incremental_errors


def run_coin_flip_experiment(save_dir: str = "results"):
    """运行投掷硬币实验并绘制图表。"""
    os.makedirs(save_dir, exist_ok=True)
    
    p = 0.5
    N = 1000
    samples, direct_means, incremental_means, direct_errors, incremental_errors = simulate_coin_flip(p=p, N=N)
    
    print("=" * 60)
    print("任务 1：投掷硬币蒙特卡罗估计")
    print("=" * 60)
    print(f"真实概率 (期望值): {p}")
    print(f"N = {N} 次投掷后的估计结果：")
    print(f"  直接平均估计值: {direct_means[-1]:.6f} (误差: {direct_errors[-1]:.6f})")
    print(f"  增量平均估计值: {incremental_means[-1]:.6f} (误差: {incremental_errors[-1]:.6f})")
    
    # 验证两者是否完全一致
    diff = np.max(np.abs(direct_means - incremental_means))
    print(f"  直接平均与增量平均的最大差异: {diff:.2e}")
    
    # 绘制曲线
    fig, axes = plt.subplots(2, 1, figsize=(10, 8), sharex=True)
    
    # 子图 1：估计值收敛过程
    axes[0].plot(range(1, N + 1), direct_means, label="直接平均估计", color="#3498db", alpha=0.8, linewidth=1.5)
    axes[0].plot(range(1, N + 1), incremental_means, '--', label="增量平均估计", color="#e74c3c", alpha=0.8, linewidth=1.5)
    axes[0].axhline(y=p, color="#2ecc71", linestyle=":", label="真实期望", linewidth=2)
    axes[0].set_ylabel("估计值", fontsize=11)
    axes[0].set_title("投掷硬币实验：期望值估计的收敛过程", fontsize=13)
    axes[0].legend(fontsize=10)
    axes[0].grid(True, alpha=0.3)
    
    # 子图 2：误差下降曲线 (Log scale 可以更清晰地看误差规律)
    axes[1].plot(range(1, N + 1), direct_errors, label="估计误差 |w_k - μ|", color="#2c3e50", alpha=0.8)
    # 绘制 O(1/sqrt(N)) 的参考线
    ref_x = np.arange(1, N + 1)
    ref_y = 0.5 / np.sqrt(ref_x)  # 缩放系数设为 0.5 方便对比
    axes[1].plot(ref_x, ref_y, ':', label="O(1/√N) 理论收敛速度", color="#f39c12", linewidth=2)
    
    axes[1].set_xlabel("样本数量 N", fontsize=11)
    axes[1].set_ylabel("估计误差", fontsize=11)
    axes[1].set_yscale("log")
    axes[1].set_title("估计误差随样本数量的变化曲线 (对数纵轴)", fontsize=13)
    axes[1].legend(fontsize=10)
    axes[1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    save_path = os.path.join(save_dir, "coin_flip_error.png")
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[保存] {save_path}")
    plt.close()


if __name__ == "__main__":
    run_coin_flip_experiment()
