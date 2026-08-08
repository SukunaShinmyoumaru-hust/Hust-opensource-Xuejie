"""
共享 GridWorld 与 CliffWalking 环境模块
========================================
项目所有强化学习实验的基础环境定义，提供统一的动力学接口。
"""

import numpy as np
from typing import List, Tuple, Optional

# 动作定义：上、下、左、右
ACTIONS = {
    0: (-1, 0),  # 上
    1: (1, 0),   # 下
    2: (0, -1),  # 左
    3: (0, 1),   # 右
}
ACTION_NAMES = {0: '↑', 1: '↓', 2: '←', 3: '→'}
NUM_ACTIONS = len(ACTIONS)


class GridWorld:
    """确定性网格世界环境（状态传递接口，适用于状态空间规划和动态规划算法）。"""

    def __init__(
        self,
        size: int = 5,
        goal: Optional[Tuple[int, int]] = None,
        forbidden_states: Optional[List[Tuple[int, int]]] = None,
        r_boundary: float = -1.0,
        r_forbidden: float = -1.0,
        r_target: float = 1.0,
        r_step: float = 0.0,
        gamma: float = 0.9,
    ):
        self.size = size
        self.goal = goal if goal is not None else (size - 1, size - 1)
        self.forbidden_states = set(forbidden_states) if forbidden_states else set()
        self.r_boundary = r_boundary
        self.r_forbidden = r_forbidden
        self.r_target = r_target
        self.r_step = r_step
        self.gamma = gamma
        self.num_states = size * size
        self.num_actions = NUM_ACTIONS

    # ---- 坐标转换 ----
    def state_to_pos(self, s: int) -> Tuple[int, int]:
        """将一维状态索引转换为 (row, col) 坐标。"""
        return divmod(s, self.size)

    def pos_to_state(self, row: int, col: int) -> int:
        """将 (row, col) 坐标转换为一维状态索引。"""
        return row * self.size + col

    # ---- 环境动力学 ----
    def is_terminal(self, s: int) -> bool:
        """判断状态 s 是否为终止状态（目标）。"""
        return self.state_to_pos(s) == self.goal

    def step(self, s: int, a: int) -> Tuple[int, float]:
        """执行动作 a，返回 (next_state, reward)。

        如果 s 是终止状态，直接返回 (s, 0)。
        """
        if self.is_terminal(s):
            return s, 0.0

        row, col = self.state_to_pos(s)
        dr, dc = ACTIONS[a]
        new_row, new_col = row + dr, col + dc

        # 撞墙检测
        if new_row < 0 or new_row >= self.size or new_col < 0 or new_col >= self.size:
            return s, self.r_boundary

        next_pos = (new_row, new_col)

        # 到达目标
        if next_pos == self.goal:
            return self.pos_to_state(new_row, new_col), self.r_target

        # 进入禁止区域
        if next_pos in self.forbidden_states:
            return self.pos_to_state(new_row, new_col), self.r_forbidden

        # 普通移动
        return self.pos_to_state(new_row, new_col), self.r_step

    def get_transitions(self, s: int, a: int) -> List[Tuple[float, int, float]]:
        """返回转移列表 [(prob, next_state, reward), ...]。

        确定性环境中只有一个转移，概率为 1.0。
        """
        next_s, reward = self.step(s, a)
        return [(1.0, next_s, reward)]

    def get_all_states(self) -> List[int]:
        """返回所有状态索引列表。"""
        return list(range(self.num_states))

    def get_non_terminal_states(self) -> List[int]:
        """返回所有非终止状态索引列表。"""
        return [s for s in range(self.num_states) if not self.is_terminal(s)]

    def __repr__(self) -> str:
        return (
            f"GridWorld(size={self.size}, goal={self.goal}, "
            f"forbidden={self.forbidden_states}, γ={self.gamma})"
        )


class GymGridWorld:
    """确定性网格世界环境（带有内部状态，符合 Gym 接口标准，适用于时序差分类算法）。"""

    def __init__(self, size=5, start=(0, 0), goal=(4, 4), gamma=0.9):
        self.size = size
        self.start = start
        self.goal = goal
        self.gamma = gamma
        self.num_actions = NUM_ACTIONS
        self.num_states = size * size
        self.reset()

    def reset(self, state=None):
        if state is not None:
            self.state = state
        else:
            self.state = self.start
        return self.state

    def step(self, action):
        if self.state == self.goal:
            return self.state, 0.0, True, {}

        r, c = self.state
        if action == 0:    # Up
            nr, nc = r - 1, c
        elif action == 1:  # Down
            nr, nc = r + 1, c
        elif action == 2:  # Left
            nr, nc = r, c - 1
        elif action == 3:  # Right
            nr, nc = r, c + 1
        else:
            raise ValueError("Invalid action")

        bump = False
        if nr < 0 or nr >= self.size or nc < 0 or nc >= self.size:
            nr, nc = r, c
            bump = True

        self.state = (nr, nc)
        done = (self.state == self.goal)

        if done:
            reward = 1.0
        else:
            reward = -1.0  # 普通移动和撞墙均返回 -1.0

        return self.state, reward, done, {}


class CliffWalking:
    """悬崖寻路环境（符合 Gym 接口标准，用于时序差分中的 Cliff Walking 选做实验）。"""

    def __init__(self, gamma=0.9):
        self.height = 4
        self.width = 12
        self.start = (3, 0)
        self.goal = (3, 11)
        self.gamma = gamma
        self.reset()

    def reset(self):
        self.state = self.start
        return self.state

    def step(self, action):
        if self.state == self.goal:
            return self.state, 0.0, True, {}

        r, c = self.state
        if action == 0:    # Up
            nr, nc = r - 1, c
        elif action == 1:  # Down
            nr, nc = r + 1, c
        elif action == 2:  # Left
            nr, nc = r, c - 1
        elif action == 3:  # Right
            nr, nc = r, c + 1
        else:
            raise ValueError("Invalid action")

        # 越界撞墙检测
        if nr < 0 or nr >= self.height or nc < 0 or nc >= self.width:
            nr, nc = r, c

        next_state = (nr, nc)

        # 悬崖检测
        if nr == 3 and 1 <= nc <= 10:
            reward = -100.0
            self.state = self.start
            done = False
        else:
            reward = -1.0
            self.state = next_state
            done = (self.state == self.goal)

        return self.state, reward, done, {}
