import os
import random
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import matplotlib.pyplot as plt
from collections import deque
from gridworld import GridWorld
from visualization import plot_policy_arrows, plot_value_heatmap

def set_seed(seed=42):
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    if torch.cuda.is_available():
        torch.cuda.manual_seed(seed)

# ---- DQN 网络结构 ----
class QNetwork(nn.Module):
    def __init__(self, input_dim=2, hidden_dim=100, output_dim=4):
        super(QNetwork, self).__init__()
        self.fc = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, output_dim)
        )
        
    def forward(self, x):
        return self.fc(x)

# ---- 经验回放缓冲区 ----
class ReplayBuffer:
    def __init__(self, capacity=500):
        self.buffer = deque(maxlen=capacity)
        
    def push(self, state, action, reward, next_state, done):
        self.buffer.append((state, action, reward, next_state, done))
        
    def sample(self, batch_size):
        state, action, reward, next_state, done = zip(*random.sample(self.buffer, batch_size))
        return (np.array(state, dtype=np.float32),
                np.array(action, dtype=np.int64),
                np.array(reward, dtype=np.float32),
                np.array(next_state, dtype=np.float32),
                np.array(done, dtype=np.float32))
                
    def __len__(self):
        return len(self.buffer)

def state_to_input(s, env):
    """将一维状态转换为归一化坐标特征 (y, x)"""
    row, col = env.state_to_pos(s)
    return np.array([row / (env.size - 1), col / (env.size - 1)], dtype=np.float32)

# ---- 评估策略 ----
def evaluate_dqn_policy(q_net, env, max_eval_steps=20):
    success_count = 0
    total_steps = 0
    non_terminal_states = env.get_non_terminal_states()
    q_net.eval()
    
    with torch.no_grad():
        for s_start in non_terminal_states:
            s = s_start
            steps = 0
            success = False
            while not env.is_terminal(s) and steps < max_eval_steps:
                state_feat = torch.tensor(state_to_input(s, env)).unsqueeze(0)
                q_values = q_net(state_feat)
                a = torch.argmax(q_values).item()
                s_next, _ = env.step(s, a)
                s = s_next
                steps += 1
                if env.is_terminal(s):
                    success = True
                    break
            
            if success:
                success_count += 1
                total_steps += steps
                
    success_rate = success_count / len(non_terminal_states)
    avg_path_len = total_steps / success_count if success_count > 0 else float('nan')
    return success_rate, avg_path_len

def evaluate_tabular_policy(Q_table, env, max_eval_steps=20):
    success_count = 0
    total_steps = 0
    non_terminal_states = env.get_non_terminal_states()
    
    for s_start in non_terminal_states:
        s = s_start
        steps = 0
        success = False
        while not env.is_terminal(s) and steps < max_eval_steps:
            q_vals = Q_table[s]
            a = np.argmax(q_vals)
            s_next, _ = env.step(s, a)
            s = s_next
            steps += 1
            if env.is_terminal(s):
                success = True
                break
        
        if success:
            success_count += 1
            total_steps += steps
            
    success_rate = success_count / len(non_terminal_states)
    avg_path_len = total_steps / success_count if success_count > 0 else float('nan')
    return success_rate, avg_path_len

def train_dqn(env, total_steps=1000, lr=0.005, batch_size=32, target_update_freq=50, epsilon=0.1):
    q_net = QNetwork()
    target_net = QNetwork()
    target_net.load_state_dict(q_net.state_dict())
    target_net.eval()
    
    optimizer = optim.Adam(q_net.parameters(), lr=lr)
    loss_fn = nn.MSELoss()
    replay_buffer = ReplayBuffer(capacity=500)
    
    eval_steps = []
    eval_success_rates = []
    eval_path_lens = []
    td_errors = []
    
    s = np.random.choice(env.get_non_terminal_states())
    
    for step in range(total_steps):
        # 1. 选择动作
        q_net.eval()
        if np.random.rand() < epsilon:
            a = np.random.choice(env.num_actions)
        else:
            with torch.no_grad():
                state_feat = torch.tensor(state_to_input(s, env)).unsqueeze(0)
                q_values = q_net(state_feat)
                a = torch.argmax(q_values).item()
                
        # 2. 执行动作
        s_next, reward = env.step(s, a)
        done = env.is_terminal(s_next)
        
        # 3. 存入经验回放
        state_feat = state_to_input(s, env)
        next_state_feat = state_to_input(s_next, env)
        replay_buffer.push(state_feat, a, reward, next_state_feat, done)
        
        # 4. 更新网络
        if len(replay_buffer) >= batch_size:
            q_net.train()
            states_b, actions_b, rewards_b, next_states_b, dones_b = replay_buffer.sample(batch_size)
            
            states_t = torch.tensor(states_b)
            actions_t = torch.tensor(actions_b).unsqueeze(1)
            rewards_t = torch.tensor(rewards_b)
            next_states_t = torch.tensor(next_states_b)
            dones_t = torch.tensor(dones_b)
            
            current_q = q_net(states_t).gather(1, actions_t).squeeze(1)
            
            with torch.no_grad():
                max_next_q = target_net(next_states_t).max(1)[0]
                target_q = rewards_t + env.gamma * max_next_q * (1.0 - dones_t)
                
            loss = loss_fn(current_q, target_q)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            
            td_errors.append(loss.item())
            
        # 5. 更新目标网络
        if (step + 1) % target_update_freq == 0:
            target_net.load_state_dict(q_net.state_dict())
            
        # 6. 定期评估
        if (step + 1) % 50 == 0:
            succ_rate, avg_len = evaluate_dqn_policy(q_net, env)
            eval_steps.append(step + 1)
            eval_success_rates.append(succ_rate)
            eval_path_lens.append(avg_len)
            
        # 7. 状态转移
        if done:
            s = np.random.choice(env.get_non_terminal_states())
        else:
            s = s_next
            
    return q_net, eval_steps, eval_success_rates, eval_path_lens, td_errors

def train_tabular_q(env, total_steps=1000, alpha=0.1, epsilon=0.1):
    Q_table = np.zeros((env.num_states, env.num_actions))
    
    eval_steps = []
    eval_success_rates = []
    eval_path_lens = []
    
    s = np.random.choice(env.get_non_terminal_states())
    
    for step in range(total_steps):
        # 1. 选择动作
        if np.random.rand() < epsilon:
            a = np.random.choice(env.num_actions)
        else:
            q_vals = Q_table[s]
            max_q = np.max(q_vals)
            best_actions = np.where(q_vals == max_q)[0]
            a = np.random.choice(best_actions)
            
        # 2. 执行动作
        s_next, reward = env.step(s, a)
        done = env.is_terminal(s_next)
        
        # 3. 更新 Q 表
        if done:
            max_next_q = 0.0
        else:
            max_next_q = np.max(Q_table[s_next])
            
        td_target = reward + env.gamma * max_next_q
        Q_table[s, a] += alpha * (td_target - Q_table[s, a])
        
        # 4. 定期评估
        if (step + 1) % 50 == 0:
            succ_rate, avg_len = evaluate_tabular_policy(Q_table, env)
            eval_steps.append(step + 1)
            eval_success_rates.append(succ_rate)
            eval_path_lens.append(avg_len)
            
        # 5. 状态转移
        if done:
            s = np.random.choice(env.get_non_terminal_states())
        else:
            s = s_next
            
    return Q_table, eval_steps, eval_success_rates, eval_path_lens

def main():
    set_seed(42)
    os.makedirs("results", exist_ok=True)
    
    env = GridWorld()
    
    print("Training DQN...")
    q_net, dqn_steps, dqn_succ, dqn_lens, dqn_td_err = train_dqn(env, total_steps=1000, lr=0.005)
    print(f"DQN Final Evaluation: Success Rate = {dqn_succ[-1]:.2%}, Avg Path Length = {dqn_lens[-1]:.2f}")
    
    print("\nTraining Tabular Q-learning...")
    Q_table, tab_steps, tab_succ, tab_lens = train_tabular_q(env, total_steps=1000, alpha=0.1)
    print(f"Tabular Q-learning Final Evaluation: Success Rate = {tab_succ[-1]:.2%}, Avg Path Length = {tab_lens[-1]:.2f}")
    
    # ---- 绘制对比曲线 ----
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
    
    # 成功率对比
    ax1.plot(dqn_steps, dqn_succ, 'o-', label="DQN (Neural Net)", color='#e74c3c', linewidth=2)
    ax1.plot(tab_steps, tab_succ, 's-', label="Tabular Q-learning", color='#3498db', linewidth=2)
    ax1.set_xlabel("Steps")
    ax1.set_ylabel("Evaluation Success Rate")
    ax1.set_title("DQN vs Tabular Q-learning Sample Efficiency (Success Rate)")
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # 平均路径长度对比
    ax2.plot(dqn_steps, dqn_lens, 'o-', label="DQN (Neural Net)", color='#e74c3c', linewidth=2)
    ax2.plot(tab_steps, tab_lens, 's-', label="Tabular Q-learning", color='#3498db', linewidth=2)
    ax2.set_xlabel("Steps")
    ax2.set_ylabel("Evaluation Avg Path Length")
    ax2.set_title("DQN vs Tabular Q-learning Sample Efficiency (Path Length)")
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig("results/task4_dqn_vs_tabular.png", dpi=150, bbox_inches='tight')
    plt.close()
    
    # ---- 绘制 DQN TD Error ----
    plt.figure(figsize=(8, 4))
    plt.plot(dqn_td_err, color='#8e44ad', alpha=0.6, label="DQN MSE Loss (TD Error)")
    window = 20
    if len(dqn_td_err) >= window:
        loss_smooth = np.convolve(dqn_td_err, np.ones(window)/window, mode='valid')
        plt.plot(np.arange(window-1, len(dqn_td_err)), loss_smooth, color='#2c3e50', linewidth=2, label="Smoothed Loss")
    plt.xlabel("Training Steps (Mini-batch Updates)")
    plt.ylabel("Loss")
    plt.title("DQN Loss (TD Error) Convergence")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig("results/task4_dqn_loss.png", dpi=150, bbox_inches='tight')
    plt.close()
    
    # ---- 提取策略并绘制策略图 ----
    policy_dqn = np.zeros(env.num_states)
    V_dqn = np.zeros(env.num_states)
    policy_tab = np.zeros(env.num_states)
    V_tab = np.zeros(env.num_states)
    
    q_net.eval()
    with torch.no_grad():
        for s in range(env.num_states):
            if env.is_terminal(s):
                policy_dqn[s] = 0
                V_dqn[s] = 0.0
                policy_tab[s] = 0
                V_tab[s] = 0.0
            else:
                # DQN
                state_feat = torch.tensor(state_to_input(s, env)).unsqueeze(0)
                q_vals_dqn = q_net(state_feat).numpy().flatten()
                policy_dqn[s] = np.argmax(q_vals_dqn)
                V_dqn[s] = np.max(q_vals_dqn)
                
                # Tabular
                q_vals_tab = Q_table[s]
                policy_tab[s] = np.argmax(q_vals_tab)
                V_tab[s] = np.max(q_vals_tab)
                
    plot_policy_arrows(policy_dqn, env, "DQN Final Policy", V=V_dqn, save_path="results/task4_dqn_policy.png")
    plot_policy_arrows(policy_tab, env, "Tabular Q-learning Final Policy", V=V_tab, save_path="results/task4_tabular_policy.png")
    
    print("DQN Task complete. Results saved to results/.")

if __name__ == "__main__":
    main()
