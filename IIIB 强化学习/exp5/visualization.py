import os
import importlib.util

curr_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(curr_dir)
shared_path = os.path.join(parent_dir, "visualization.py")

spec = importlib.util.spec_from_file_location("shared_visualization", shared_path)
shared_visualization = importlib.util.module_from_spec(spec)
spec.loader.exec_module(shared_visualization)

# 导出需要的函数
plot_value_heatmap = shared_visualization.plot_value_heatmap
plot_policy_arrows = shared_visualization.plot_policy_arrows
