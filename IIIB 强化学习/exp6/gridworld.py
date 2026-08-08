import os
import importlib.util

# 使用 SourceFileLoader 直接从根目录绝对路径加载共享环境，防止同名模块导入冲突
curr_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(curr_dir)
shared_path = os.path.join(parent_dir, "gridworld.py")

spec = importlib.util.spec_from_file_location("shared_gridworld", shared_path)
shared_gridworld = importlib.util.module_from_spec(spec)
spec.loader.exec_module(shared_gridworld)

SharedGridWorld = shared_gridworld.GridWorld
NUM_ACTIONS = shared_gridworld.NUM_ACTIONS
ACTIONS = shared_gridworld.ACTIONS
ACTION_NAMES = shared_gridworld.ACTION_NAMES

class GridWorld(SharedGridWorld):
    """EXP6 包装环境"""
    pass
