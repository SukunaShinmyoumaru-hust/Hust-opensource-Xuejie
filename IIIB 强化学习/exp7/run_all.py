"""
运行所有任务并生成实验图表
"""

import subprocess
import os
import sys

def main():
    print("====================================================")
    print("       开始执行 实验七：Actor-Critic 算法 所有任务     ")
    print("====================================================")
    
    # 确保在当前目录下运行
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    
    # 创建结果目录
    os.makedirs("results", exist_ok=True)
    
    # 执行任务 1
    print("\n--- 正在运行 任务 1: QAC 算法 ---")
    result1 = subprocess.run([sys.executable, "task1_qac.py"], capture_output=True, text=True)
    print(result1.stdout)
    if result1.returncode != 0:
        print("任务 1 运行失败！错误信息：", result1.stderr)
        sys.exit(1)
        
    # 执行任务 2
    print("\n--- 正在运行 任务 2: A2C 算法与对比 ---")
    result2 = subprocess.run([sys.executable, "task2_a2c.py"], capture_output=True, text=True)
    print(result2.stdout)
    if result2.returncode != 0:
        print("任务 2 运行失败！错误信息：", result2.stderr)
        sys.exit(1)
        
    # 执行任务 3
    print("\n--- 正在运行 任务 3: 异策略 Actor-Critic ---")
    result3 = subprocess.run([sys.executable, "task3_off_policy.py"], capture_output=True, text=True)
    print(result3.stdout)
    if result3.returncode != 0:
        print("任务 3 运行失败！错误信息：", result3.stderr)
        sys.exit(1)
        
    print("\n====================================================")
    print("所有任务成功执行！已生成以下结果文件于 exp7/results/ 目录：")
    print("1. task1_qac_learning.png  - QAC 算法学习曲线")
    print("2. task1_qac_values.png    - QAC 最终状态值函数热力图")
    print("3. task1_qac_policy.png    - QAC 最终最优策略箭头图")
    print("4. task2_qac_vs_a2c.png    - QAC 与 A2C 独立运行均值/标准差对比图")
    print("5. task2_a2c_values.png    - A2C 最终状态值函数热力图")
    print("6. task2_a2c_policy.png    - A2C 最终最优策略箭头图")
    print("7. task3_off_policy_learning.png - 异策略目标策略离线评估学习曲线")
    print("8. task3_behavior_policy.png     - 用于采集数据的 ε-greedy 行为策略箭头图")
    print("9. task3_target_policy.png       - 学习到的异策略目标策略箭头图")
    print("====================================================")

if __name__ == "__main__":
    main()
