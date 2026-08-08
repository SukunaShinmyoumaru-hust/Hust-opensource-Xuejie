import os
import numpy as np
import matplotlib.pyplot as plt

# Set random seed for reproducibility
np.random.seed(42)

def get_step_size(name, k, alpha_0=0.05):
    """
    Returns step size alpha_k for step k (1-indexed).
    """
    if name == '1/k':
        return 1.0 / k
    elif name == '1/k^0.6':
        return 1.0 / (k ** 0.6)
    elif name == '0.05':
        return 0.05
    elif name == '1/k^2':
        return 1.0 / (k ** 2)
    else:
        raise ValueError(f"Unknown step size name: {name}")

def run_task2(results_dir="results"):
    os.makedirs(results_dir, exist_ok=True)
    
    # -----------------------------
    # 1. 1D Robbins-Monro Comparison
    # -----------------------------
    mu_1d = 2.0
    sigma_1d = 1.0
    N_1d = 1000
    samples_1d = np.random.normal(mu_1d, sigma_1d, N_1d)
    
    step_sizes = ['1/k', '1/k^0.6', '0.05', '1/k^2']
    initial_values_1d = [-10.0, 10.0]
    
    results_1d = {}
    
    for w0 in initial_values_1d:
        results_1d[w0] = {}
        for ss in step_sizes:
            w = w0
            traj = [w]
            for k in range(1, N_1d + 1):
                x = samples_1d[k-1]
                alpha = get_step_size(ss, k)
                w = w - alpha * (w - x)
                traj.append(w)
            results_1d[w0][ss] = np.array(traj)
            
    # Plot 1D Results
    for w0 in initial_values_1d:
        plt.figure(figsize=(12, 5))
        
        # Subplot 1: w_k trajectories
        plt.subplot(1, 2, 1)
        for ss in step_sizes:
            traj = results_1d[w0][ss]
            plt.plot(range(0, N_1d + 1), traj, label=f'$\\alpha_k$={ss}')
        plt.axhline(mu_1d, color='red', linestyle='--', label='True Mean $\mu=2.0$')
        plt.xlabel('Step $k$')
        plt.ylabel('Estimate $w_k$')
        plt.title(f'1D Trajectories ($w_0 = {w0}$)')
        plt.legend()
        plt.grid(True, linestyle=':', alpha=0.6)
        
        # Subplot 2: |w_k - \mu| error curves
        plt.subplot(1, 2, 2)
        for ss in step_sizes:
            traj = results_1d[w0][ss]
            errors = np.abs(traj - mu_1d)
            plt.plot(range(0, N_1d + 1), errors, label=f'$\\alpha_k$={ss}')
        plt.xlabel('Step $k$')
        plt.ylabel('$|w_k - \\mu|$')
        plt.title(f'1D Error Curves ($w_0 = {w0}$)')
        plt.legend()
        plt.yscale('log')
        plt.grid(True, linestyle=':', alpha=0.6)
        
        plt.tight_layout()
        plt.savefig(os.path.join(results_dir, f'task2_1d_w0_{int(w0)}.png'), dpi=300)
        plt.close()
        
    # Print final errors for comparison
    print("--- 1D Robbins-Monro Final Absolute Errors ---")
    for w0 in initial_values_1d:
        print(f"Initial w0 = {w0}:")
        for ss in step_sizes:
            final_val = results_1d[w0][ss][-1]
            final_err = abs(final_val - mu_1d)
            print(f"  Step size {ss:10s} -> Final w: {final_val:8.5f}, Error: {final_err:8.5f}")

    # -----------------------------
    # 2. 2D Robbins-Monro Comparison
    # -----------------------------
    # Generate 100 2D samples Uniform[-10, 10]
    N_2d = 100
    samples_2d = np.random.uniform(-10.0, 10.0, (N_2d, 2))
    mu_2d = np.array([0.0, 0.0])
    
    # Let's use two initial values in 2D as well: [10, 10] and [-10, -10]
    initial_values_2d = [np.array([10.0, 10.0]), np.array([-10.0, -10.0])]
    
    results_2d = {}
    
    for i, w0 in enumerate(initial_values_2d):
        key = f"w0_{i}"
        results_2d[key] = {}
        for ss in step_sizes:
            w = w0.copy()
            traj = [w.copy()]
            for k in range(1, N_2d + 1):
                x = samples_2d[k-1]
                alpha = get_step_size(ss, k)
                w = w - alpha * (w - x)
                traj.append(w.copy())
            results_2d[key][ss] = np.array(traj)
            
    # Plot 2D Trajectories in Phase Space
    plt.figure(figsize=(12, 6))
    colors = {'1/k': 'blue', '1/k^0.6': 'orange', '0.05': 'green', '1/k^2': 'purple'}
    
    for i, w0 in enumerate(initial_values_2d):
        key = f"w0_{i}"
        plt.subplot(1, 2, i+1)
        plt.scatter(samples_2d[:, 0], samples_2d[:, 1], color='gray', alpha=0.3, label='Samples')
        plt.plot(mu_2d[0], mu_2d[1], 'r*', markersize=12, label='True Mean $(0,0)$')
        
        for ss in step_sizes:
            traj = results_2d[key][ss]
            plt.plot(traj[:, 0], traj[:, 1], color=colors[ss], marker='o', markersize=3, label=f'$\\alpha_k$={ss}')
            plt.scatter(traj[0, 0], traj[0, 1], color='black', marker='x', s=50) # Start point
            
        plt.xlabel('$w^{(1)}$')
        plt.ylabel('$w^{(2)}$')
        plt.title(f'2D RM Parameter Trajectories ($w_0 = {w0}$)')
        plt.legend()
        plt.grid(True, linestyle=':', alpha=0.6)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task2_2d_trajectories.png'), dpi=300)
    plt.close()
    
    # Plot 2D Error Curves (||w_k - \mu||)
    plt.figure(figsize=(12, 5))
    for i, w0 in enumerate(initial_values_2d):
        key = f"w0_{i}"
        plt.subplot(1, 2, i+1)
        for ss in step_sizes:
            traj = results_2d[key][ss]
            errors = np.linalg.norm(traj - mu_2d, axis=1)
            plt.plot(range(0, N_2d + 1), errors, label=f'$\\alpha_k$={ss}')
        plt.xlabel('Step $k$')
        plt.ylabel('$||w_k - \\mu||$')
        plt.title(f'2D Error Curves ($w_0 = {w0}$)')
        plt.legend()
        plt.yscale('log')
        plt.grid(True, linestyle=':', alpha=0.6)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task2_2d_errors.png'), dpi=300)
    plt.close()

if __name__ == "__main__":
    run_task2()
