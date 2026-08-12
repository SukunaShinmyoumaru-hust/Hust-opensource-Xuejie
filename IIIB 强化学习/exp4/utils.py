import os
import importlib.util

curr_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(curr_dir)
shared_path = os.path.join(parent_dir, "visualization.py")

spec = importlib.util.spec_from_file_location("shared_visualization", shared_path)
shared_visualization = importlib.util.module_from_spec(spec)
spec.loader.exec_module(shared_visualization)

ensure_dir = shared_visualization.ensure_dir
plot_heatmap = shared_visualization.plot_heatmap
plot_gridworld_schematic = shared_visualization.plot_gridworld_schematic
plot_policy_arrows = shared_visualization.plot_policy_arrows_q
plot_cliffwalking_policy = shared_visualization.plot_cliffwalking_policy
