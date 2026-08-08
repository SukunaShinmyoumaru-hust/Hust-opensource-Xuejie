import numpy as np
from gridworld import GridWorld
from task3_q_learning import run_q_learning, evaluate_policy, get_q_values

env = GridWorld()
w, returns, success_rates = run_q_learning(env, state_dim=6, alpha=0.01, epsilon=0.1, episodes=1000, max_steps=100)
success_rate, avg_path = evaluate_policy(w, env, state_dim=6)
print("Q-learning Success Rate:", success_rate)
print("Q-learning Avg Path:", avg_path)

# Let's inspect the Q-values for all actions at all states
for s in range(env.num_states):
    if env.is_terminal(s):
        continue
    row, col = env.state_to_pos(s)
    q_vals = get_q_values(s, w, env, state_dim=6)
    print(f"State ({row}, {col}): {q_vals} -> Best action: {np.argmax(q_vals)}")
