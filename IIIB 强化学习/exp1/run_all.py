"""
实验一：值迭代和策略迭代 — 主入口
===================================
运行全部三个任务，生成完整的实验结果和可视化图表。

用法：
    cd exp1
    python run_all.py
"""

import os
import sys
import matplotlib
matplotlib.use('Agg')  # 非交互模式，直接保存图片

# 确保当前目录在 sys.path 中
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from value_iteration import run_task1
from policy_iteration import run_task2
from truncated_policy_iteration import run_task3


def main():
    save_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results")
    os.makedirs(save_dir, exist_ok=True)

    print("╔" + "═" * 68 + "╗")
    print("║" + "实验一：值迭代和策略迭代".center(58) + "║")
    print("╚" + "═" * 68 + "╝")
    print(f"\n结果保存目录: {save_dir}\n")

    # ---- 任务 1 ----
    run_task1(save_dir=save_dir)

    # ---- 任务 2 ----
    run_task2(save_dir=save_dir)

    # ---- 任务 3 ----
    run_task3(save_dir=save_dir)

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
