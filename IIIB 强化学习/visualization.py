"""
共享可视化工具模块
==================
集中管理项目所有实验的图形绘制和表格打印功能，包含中文字体配置与后台 Agg 后端设定。
"""

import os
import numpy as np
import matplotlib
matplotlib.use('Agg')  # 后台 Agg 后端模式，不弹窗，防止 headless 环境卡死
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.colors import LinearSegmentedColormap

# 确保中文显示正常
plt.rcParams['font.sans-serif'] = ['WenQuanYi Micro Hei', 'Source Han Sans CN',
                                    'Noto Sans CJK JP', 'SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

# 动作到箭头方向的映射 (dx, dy in matplotlib坐标系)
_ARROW_MAP = {
    0: (0, 0.3),    # 上 → y+
    1: (0, -0.3),   # 下 → y-
    2: (-0.3, 0),   # 左 → x-
    3: (0.3, 0),    # 右 → x+
}
_ACTION_SYMBOLS = {0: '↑', 1: '↓', 2: '←', 3: '→'}


# ==================== 通用辅助函数 ====================
def ensure_dir(path: str):
    """确保目录存在。"""
    directory = os.path.dirname(path)
    if directory and not os.path.exists(directory):
        os.makedirs(directory, exist_ok=True)


# ==================== EXP1 / EXP2 / EXP5 / EXP6 / EXP7 绘图 ====================

def _annotation_color(im, value):
    """Choose black or white text from the rendered cell background luminance."""
    r, g, b, _ = im.cmap(im.norm(value))
    # WCAG relative luminance is more reliable than comparing raw data values.
    channels = []
    for channel in (r, g, b):
        channels.append(channel / 12.92 if channel <= 0.04045
                        else ((channel + 0.055) / 1.055) ** 2.4)
    luminance = 0.2126 * channels[0] + 0.7152 * channels[1] + 0.0722 * channels[2]
    return 'black' if luminance > 0.45 else 'white'


def plot_value_heatmap(V: np.ndarray, env, title: str = "状态值函数",
                       ax=None, save_path: str = None):
    """绘制值函数热力图。"""
    show = ax is None
    if ax is None:
        fig, ax = plt.subplots(figsize=(6, 5))

    V_grid = V.reshape(env.size, env.size)

    im = ax.imshow(V_grid, cmap='RdYlGn', interpolation='nearest')
    plt.colorbar(im, ax=ax, shrink=0.8)

    # 在每个格子中显示数值
    for i in range(env.size):
        for j in range(env.size):
            s = env.pos_to_state(i, j)
            color = _annotation_color(im, V_grid[i, j])
            text = f"{V_grid[i, j]:.2f}"
            if env.is_terminal(s):
                text += "\n" + r"$\mathbf{\bigstar}$"
            elif hasattr(env, 'forbidden_states') and (i, j) in env.forbidden_states:
                text += "\n" + r"$\mathbf{\times}$"
            ax.text(j, i, text, ha='center', va='center', fontsize=9,
                    fontweight='bold', color=color)

    ax.set_title(title, fontsize=13)
    ax.set_xticks(range(env.size))
    ax.set_yticks(range(env.size))
    ax.set_xlabel("列")
    ax.set_ylabel("行")

    if save_path:
        ensure_dir(save_path)
        plt.tight_layout()
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"[保存] {save_path}")
    if show:
        plt.tight_layout()
        plt.close()


def plot_policy_arrows(policy: np.ndarray, env, title: str = "最优策略",
                       V: np.ndarray = None, ax=None, save_path: str = None):
    """绘制策略箭头图。"""
    show = ax is None
    if ax is None:
        fig, ax = plt.subplots(figsize=(6, 5))

    # 背景色
    if V is not None:
        V_grid = V.reshape(env.size, env.size)
        ax.imshow(V_grid, cmap='RdYlGn', alpha=0.3, interpolation='nearest')
    else:
        bg = np.zeros((env.size, env.size))
        ax.imshow(bg, cmap='Greys', alpha=0.1, interpolation='nearest')

    for s in range(env.num_states):
        row, col = env.state_to_pos(s)
        if env.is_terminal(s):
            ax.text(col, row, r'$\mathbf{\bigstar}$', ha='center', va='center', fontsize=18, color='gold')
            continue
        if hasattr(env, 'forbidden_states') and (row, col) in env.forbidden_states:
            ax.add_patch(plt.Rectangle((col - 0.5, row - 0.5), 1, 1,
                                       facecolor='red', alpha=0.2))
            ax.text(col, row, r'$\mathbf{\times}$', ha='center', va='center', fontsize=18, color='red')
            
        a = int(policy[s])
        dx, dy = _ARROW_MAP[a]
        ax.annotate('', xy=(col + dx, row - dy), xytext=(col, row),
                    arrowprops=dict(arrowstyle='->', color='#2c3e50', lw=2))

    ax.set_xlim(-0.5, env.size - 0.5)
    ax.set_ylim(env.size - 0.5, -0.5)
    ax.set_xticks(range(env.size))
    ax.set_yticks(range(env.size))
    ax.set_title(title, fontsize=13)
    ax.set_xlabel("列")
    ax.set_ylabel("行")
    ax.set_aspect('equal')
    ax.grid(True, linewidth=0.5, alpha=0.3)

    if save_path:
        ensure_dir(save_path)
        plt.tight_layout()
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"[保存] {save_path}")
    if show:
        plt.tight_layout()
        plt.close()


def plot_convergence_curve(errors: list, title: str = "值函数收敛曲线",
                           xlabel: str = "迭代次数", ylabel: str = "最大误差",
                           log_scale: bool = True, ax=None, save_path: str = None,
                           label: str = None):
    """绘制收敛曲线。"""
    show = ax is None
    if ax is None:
        fig, ax = plt.subplots(figsize=(8, 5))

    ax.plot(range(1, len(errors) + 1), errors, 'o-', markersize=3, linewidth=1.5,
            label=label)
    if log_scale and all(e > 0 for e in errors):
        ax.set_yscale('log')
    ax.set_xlabel(xlabel, fontsize=11)
    ax.set_ylabel(ylabel, fontsize=11)
    ax.set_title(title, fontsize=13)
    ax.grid(True, alpha=0.3)
    if label:
        ax.legend()

    if save_path:
        ensure_dir(save_path)
        plt.tight_layout()
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"[保存] {save_path}")
    if show:
        plt.tight_layout()
        plt.close()


def plot_comparison(results: dict, title: str = "算法收敛对比",
                    xlabel: str = "迭代次数", ylabel: str = "最大误差",
                    log_scale: bool = True, save_path: str = None):
    """绘制多算法收敛曲线对比。"""
    fig, ax = plt.subplots(figsize=(10, 6))
    markers = ['o', 's', '^', 'D', 'v', '<', '>', 'p']
    colors = ['#e74c3c', '#3498db', '#2ecc71', '#f39c12', '#9b59b6', '#1abc9c']

    for i, (name, errors) in enumerate(results.items()):
        ax.plot(range(1, len(errors) + 1), errors,
                marker=markers[i % len(markers)], markersize=4,
                color=colors[i % len(colors)], linewidth=1.5,
                label=name)

    if log_scale:
        ax.set_yscale('log')
    ax.set_xlabel(xlabel, fontsize=11)
    ax.set_ylabel(ylabel, fontsize=11)
    ax.set_title(title, fontsize=13)
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)

    plt.tight_layout()
    if save_path:
        ensure_dir(save_path)
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"[保存] {save_path}")
    plt.close()


def plot_policy_evolution(policies: list, env, titles: list = None,
                          V_list: list = None, save_path: str = None):
    """绘制策略演变过程（多子图）。"""
    n = len(policies)
    cols = min(n, 4)
    rows = (n + cols - 1) // cols
    fig, axes = plt.subplots(rows, cols, figsize=(5 * cols, 4.5 * rows))
    if n == 1:
        axes = np.array([axes])
    axes = np.atleast_2d(axes)

    for idx in range(n):
        r, c = divmod(idx, cols)
        ax = axes[r, c]
        t = titles[idx] if titles else f"迭代 {idx}"
        V = V_list[idx] if V_list else None
        plot_policy_arrows(policies[idx], env, title=t, V=V, ax=ax)

    # 隐藏多余的子图
    for idx in range(n, rows * cols):
        r, c = divmod(idx, cols)
        axes[r, c].set_visible(False)

    plt.tight_layout()
    if save_path:
        ensure_dir(save_path)
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"[保存] {save_path}")
    plt.close()


def print_q_table(Q: np.ndarray, env, iteration: int = None):
    """打印 Q-table（格式化输出）。"""
    header = f"===== Q-Table (迭代 {iteration}) =====" if iteration is not None else "===== Q-Table ====="
    print(header)
    print(f"{'状态':>6s} | {'↑':>8s} {'↓':>8s} {'←':>8s} {'→':>8s} | {'V(s)':>8s} {'π(s)':>4s}")
    print("-" * 65)
    for s in range(env.num_states):
        row, col = env.state_to_pos(s)
        q_vals = Q[s]
        v = np.max(q_vals)
        best_a = np.argmax(q_vals)
        label = "GOAL" if env.is_terminal(s) else (
            "FORB" if hasattr(env, 'forbidden_states') and (row, col) in env.forbidden_states else ""
        )
        print(f"({row},{col}){label:>4s} | {q_vals[0]:8.4f} {q_vals[1]:8.4f} "
              f"{q_vals[2]:8.4f} {q_vals[3]:8.4f} | {v:8.4f} {_ACTION_SYMBOLS[best_a]:>4s}")
    print()


def plot_learning_curves(curves_dict: dict, window: int = 50, title: str = "学习曲线",
                         xlabel: str = "回合数 (Episodes)", ylabel: str = "累计回报",
                         save_path: str = None):
    """绘制多个算法或参数的滑动平均累计回报学习曲线。"""
    fig, ax = plt.subplots(figsize=(10, 6))
    colors = ['#e74c3c', '#3498db', '#2ecc71', '#f39c12', '#9b59b6', '#1abc9c']

    for i, (name, rewards) in enumerate(curves_dict.items()):
        rewards = np.array(rewards)
        if len(rewards) >= window:
            smoothed = np.convolve(rewards, np.ones(window)/window, mode='valid')
            x_range = range(window, len(rewards) + 1)
        else:
            smoothed = rewards
            x_range = range(1, len(rewards) + 1)
            
        ax.plot(x_range, smoothed, color=colors[i % len(colors)], linewidth=2, label=name)
        ax.plot(range(1, len(rewards) + 1), rewards, color=colors[i % len(colors)], alpha=0.15, linewidth=0.5)

    ax.set_xlabel(xlabel, fontsize=11)
    ax.set_ylabel(ylabel, fontsize=11)
    ax.set_title(title, fontsize=13)
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)
    
    plt.tight_layout()
    if save_path:
        ensure_dir(save_path)
        plt.savefig(save_path, dpi=150, bbox_inches='tight')
        print(f"[保存] {save_path}")
    plt.close()


# ==================== EXP4 专用绘图函数 ====================

def plot_heatmap(V, title, filepath):
    """画值热力图（使用 matplotlib 代替 seaborn，防止没有安装 seaborn）。"""
    ensure_dir(filepath)
    plt.figure(figsize=(6, 5))
    im = plt.imshow(V, cmap="YlGnBu", interpolation='nearest')
    plt.colorbar(im, shrink=0.8)
    
    # 在每个格子中显示数值
    h, w = V.shape
    for i in range(h):
        for j in range(w):
            text = f"{V[i, j]:.4f}"
            color = _annotation_color(im, V[i, j])
            plt.text(j, i, text, ha='center', va='center', fontsize=9,
                     fontweight='bold', color=color)
                     
    plt.title(title)
    plt.tight_layout()
    plt.savefig(filepath, dpi=300)
    plt.close()


def plot_gridworld_schematic(size=5, start=(0, 0), goal=(4, 4), filepath="results/gridworld_schematic.png"):
    """画 GridWorld 结构示意图 (用于 EXP4)。"""
    ensure_dir(filepath)
    plt.figure(figsize=(6, 6))
    for i in range(size + 1):
        plt.axhline(i, color='black', lw=1.5)
        plt.axvline(i, color='black', lw=1.5)
        
    for r in range(size):
        for c in range(size):
            x = c + 0.5
            y = size - r - 0.5
            if (r, c) == goal:
                plt.text(x, y, 'Goal\n(4,4)', ha='center', va='center', fontsize=12, fontweight='bold', color='red')
                plt.fill_between([c, c+1], [size-r-1, size-r-1], [size-r, size-r], color='lightcoral', alpha=0.3)
            elif (r, c) == start:
                plt.text(x, y, 'Start\n(0,0)', ha='center', va='center', fontsize=12, fontweight='bold', color='blue')
                plt.fill_between([c, c+1], [size-r-1, size-r-1], [size-r, size-r], color='lightblue', alpha=0.3)
            else:
                plt.text(x, y, f'({r},{c})', ha='center', va='center', fontsize=10, color='gray')
                
    plt.xlim(0, size)
    plt.ylim(0, size)
    plt.title("GridWorld 5x5 Environment Layout")
    plt.gca().set_aspect('equal', adjustable='box')
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(filepath, dpi=300)
    plt.close()


def plot_policy_arrows_q(Q, env_size=5, goal=(4, 4), title="Greedy Policy", filepath="results/policy.png"):
    """从 Q 表画策略箭头图（用于 EXP4）。"""
    ensure_dir(filepath)
    plt.figure(figsize=(6, 6))
    for i in range(env_size + 1):
        plt.axhline(i, color='black', lw=1.5)
        plt.axvline(i, color='black', lw=1.5)
        
    for r in range(env_size):
        for c in range(env_size):
            x_c = c + 0.5
            y_c = env_size - r - 0.5
            if (r, c) == goal:
                plt.text(x_c, y_c, 'G', ha='center', va='center', fontsize=20, fontweight='bold', color='red')
                continue
                
            q_values = Q[r, c]
            max_q = np.max(q_values)
            best_actions = np.where(q_values == max_q)[0]
            
            dx_dy = {0: (0, 0.35), 1: (0, -0.35), 2: (-0.35, 0), 3: (0.35, 0)}
            
            for action in best_actions:
                dx, dy = dx_dy[action]
                plt.arrow(x_c, y_c, dx, dy, 
                          head_width=0.1, head_length=0.1, fc='blue', ec='blue',
                          length_includes_head=True)
                          
    plt.xlim(0, env_size)
    plt.ylim(0, env_size)
    plt.title(title)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(filepath, dpi=300)
    plt.close()


def plot_cliffwalking_policy(Q, title="Cliff Walking Policy", filepath="results/cliffwalking_policy.png"):
    """画 CliffWalking 策略图 (用于 EXP4)。"""
    ensure_dir(filepath)
    h, w = 4, 12
    plt.figure(figsize=(12, 4))
    for i in range(h + 1):
        plt.axhline(i, color='black', lw=1.5)
    for j in range(w + 1):
        plt.axvline(j, color='black', lw=1.5)
        
    for r in range(h):
        for c in range(w):
            x_c = c + 0.5
            y_c = h - r - 0.5
            if (r, c) == (3, 11):  # Goal
                plt.text(x_c, y_c, 'G', ha='center', va='center', fontsize=16, fontweight='bold', color='red')
                continue
            if r == 3 and 1 <= c <= 10:  # Cliff
                plt.fill_between([c, c+1], [h-r-1, h-r-1], [h-r, h-r], color='black', alpha=0.8)
                plt.text(x_c, y_c, 'CLIFF', ha='center', va='center', fontsize=8, color='white', fontweight='bold')
                continue
                
            q_values = Q[r, c]
            max_q = np.max(q_values)
            best_actions = np.where(q_values == max_q)[0]
            
            dx_dy = {0: (0, 0.35), 1: (0, -0.35), 2: (-0.35, 0), 3: (0.35, 0)}
            
            for action in best_actions:
                dx, dy = dx_dy[action]
                plt.arrow(x_c, y_c, dx, dy, 
                          head_width=0.1, head_length=0.1, fc='blue', ec='blue',
                          length_includes_head=True)
                          
    plt.xlim(0, w)
    plt.ylim(0, h)
    plt.title(title)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(filepath, dpi=300)
    plt.close()
