import os
import importlib.util

curr_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(curr_dir)
shared_path = os.path.join(parent_dir, "gridworld.py")

spec = importlib.util.spec_from_file_location("shared_gridworld", shared_path)
shared_gridworld = importlib.util.module_from_spec(spec)
spec.loader.exec_module(shared_gridworld)

GridWorld = shared_gridworld.GymGridWorld
CliffWalking = shared_gridworld.CliffWalking
