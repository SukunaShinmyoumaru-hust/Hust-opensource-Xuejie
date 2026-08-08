"""
KV Cache Arena — 评估入口
用法:
    python run_eval.py                # 完整评估
    python run_eval.py --quick        # 快速测试（前 50 条）
    python run_eval.py --no-baseline  # 只跑自己的策略，跳过基线
    python run_eval.py --budget 64    # 覆盖 strategy.py 中的 MAX_BUDGET
"""
import argparse
import sys
import os

# 让 import 找到同级文件
sys.path.insert(0, os.path.dirname(__file__))

import strategy
import engine
from engine import _cache_len

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--quick',       action='store_true',
                        help='只跑前 50 条样本（快速验证）')
    parser.add_argument('--no-baseline', action='store_true',
                        help='跳过无压缩基线，只评估学生策略')
    parser.add_argument('--budget',      type=int, default=None,
                        help='覆盖 strategy.MAX_BUDGET')
    args = parser.parse_args()

    budget = args.budget if args.budget is not None else strategy.MAX_BUDGET

    def safe_compress_wrapper(pkv, current_budget, step):
        # 1. 调用学生自己写的压缩函数
        compressed_kv = strategy.compress(pkv, current_budget, step)
        
        # 2. 调用 engine 原生的长度计算函数
        try:
            returned_len = _cache_len(compressed_kv)
        except Exception:
            print("\n🚨 [错误] 返回的 KV Cache 格式被破坏，无法解析长度！请确保返回的格式与输入一致。")
            sys.exit(1)

        # 3. 严格检查是否超出 budget
        if returned_len > current_budget:
            print(f"\n🚨 [违规拦截] 压缩后的 KV 长度 ({returned_len}) 超过了当前预算 ({current_budget})！")
            print("❌ 你不能直接返回未压缩的数据，请修改 strategy.py。")
            sys.exit(1)  # 直接终止本地运行
            
        return compressed_kv

    engine.evaluate(
        compress_fn   = safe_compress_wrapper,
        max_budget    = budget,
        strategy_name = getattr(strategy, 'STRATEGY_NAME', 'MyStrategy'),
        n_docs        = 5 if args.quick else None,
        show_baseline = not args.no_baseline,
    )


if __name__ == '__main__':
    main()
