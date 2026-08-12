import os
import numpy as np
import matplotlib.pyplot as plt

# Set random seed for reproducibility
np.random.seed(42)

def run_task1(results_dir="results"):
    os.makedirs(results_dir, exist_ok=True)
    
    # -----------------------------
    # 1. 1D Warm-up Experiment
    # -----------------------------
    # X ~ N(2, 1), generate 1000 samples
    mu_1d = 2.0
    sigma_1d = 1.0
    N_1d = 1000
    samples_1d = np.random.normal(mu_1d, sigma_1d, N_1d)
    
    # Direct mean estimation
    direct_means_1d = np.zeros(N_1d)
    for k in range(1, N_1d + 1):
        direct_means_1d[k-1] = np.mean(samples_1d[:k])
        
    # Incremental mean estimation
    incremental_means_1d = np.zeros(N_1d)
    w = 0.0  # Initial value w_0 = 0
    for k in range(1, N_1d + 1):
        x = samples_1d[k-1]
        w = w - (1.0 / k) * (w - x)
        incremental_means_1d[k-1] = w
        
    # Verify that they are identical
    difference_1d = np.max(np.abs(direct_means_1d - incremental_means_1d))
    print(f"1D Max difference between direct and incremental: {difference_1d:.2e}")
    
    # Plotting 1D Results
    plt.figure(figsize=(12, 5))
    
    # Plot w_k trajectory
    plt.subplot(1, 2, 1)
    plt.plot(range(1, N_1d + 1), incremental_means_1d, label='Incremental Estimate $w_k$', color='blue', alpha=0.8)
    plt.axhline(mu_1d, color='red', linestyle='--', label='True Mean $\mu=2.0$')
    plt.xlabel('Sample Number $k$')
    plt.ylabel('Estimate value')
    plt.title('1D Sample Mean Estimate Trajectory')
    plt.legend()
    plt.grid(True, linestyle=':', alpha=0.6)
    
    # Plot |w_k - \mu| error curve
    plt.subplot(1, 2, 2)
    errors_1d = np.abs(incremental_means_1d - mu_1d)
    plt.plot(range(1, N_1d + 1), errors_1d, label='$|w_k - \mu|$', color='purple')
    plt.xlabel('Sample Number $k$')
    plt.ylabel('Absolute Error')
    plt.title('1D Estimation Error Curve')
    plt.legend()
    plt.yscale('log')
    plt.grid(True, linestyle=':', alpha=0.6)
    
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task1_1d_sample_mean.png'), dpi=300)
    plt.close()
    
    # -----------------------------
    # 2. 2D Required Experiment
    # -----------------------------
    # X ~ U([-10, 10]^2), generate 100 samples
    # Center (0,0), side length 20.
    N_2d = 100
    samples_2d = np.random.uniform(-10.0, 10.0, (N_2d, 2))
    mu_2d = np.array([0.0, 0.0])
    
    # Direct mean estimation
    direct_means_2d = np.zeros((N_2d, 2))
    for k in range(1, N_2d + 1):
        direct_means_2d[k-1] = np.mean(samples_2d[:k], axis=0)
        
    # Incremental mean estimation
    incremental_means_2d = np.zeros((N_2d, 2))
    w_2d = np.zeros(2)  # Initial value w_0 = [0, 0]
    for k in range(1, N_2d + 1):
        x = samples_2d[k-1]
        w_2d = w_2d - (1.0 / k) * (w_2d - x)
        incremental_means_2d[k-1] = w_2d.copy()
        
    # Verify that they are identical
    difference_2d = np.max(np.abs(direct_means_2d - incremental_means_2d))
    print(f"2D Max difference between direct and incremental: {difference_2d:.2e}")
    
    # Plotting 2D Results
    plt.figure(figsize=(12, 5))
    
    # Plot 2D scatter of samples, true mean, and trajectory
    plt.subplot(1, 2, 1)
    plt.scatter(samples_2d[:, 0], samples_2d[:, 1], color='gray', alpha=0.5, label='Samples')
    plt.plot(incremental_means_2d[:, 0], incremental_means_2d[:, 1], color='blue', marker='o', markersize=3, label='Estimate Trajectory')
    plt.plot(mu_2d[0], mu_2d[1], 'r*', markersize=12, label='True Mean $(0,0)$')
    # Mark starting and ending points of the trajectory
    plt.scatter(incremental_means_2d[0, 0], incremental_means_2d[0, 1], color='green', marker='s', s=80, label='Start ($w_1$)')
    plt.scatter(incremental_means_2d[-1, 0], incremental_means_2d[-1, 1], color='orange', marker='D', s=80, label='End ($w_{100}$)')
    plt.xlabel('$x_1$')
    plt.ylabel('$x_2$')
    plt.title('2D Sample Scatter & Estimate Trajectory')
    plt.legend()
    plt.grid(True, linestyle=':', alpha=0.6)
    
    # Plot ||w_k - \mu|| error curve
    plt.subplot(1, 2, 2)
    errors_direct_2d = np.linalg.norm(direct_means_2d - mu_2d, axis=1)
    errors_inc_2d = np.linalg.norm(incremental_means_2d - mu_2d, axis=1)
    
    plt.plot(range(1, N_2d + 1), errors_direct_2d, label='Direct Average Error', color='green', linestyle='-', alpha=0.7)
    plt.plot(range(1, N_2d + 1), errors_inc_2d, label='Incremental Average Error', color='blue', linestyle='--', alpha=0.7)
    plt.xlabel('Sample Number $k$')
    plt.ylabel('$||w_k - \mu||$')
    plt.title('2D Estimation Error Curve')
    plt.legend()
    plt.yscale('log')
    plt.grid(True, linestyle=':', alpha=0.6)
    
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task1_2d_sample_mean.png'), dpi=300)
    plt.close()
    
    # Return samples for reuse in task 3
    return samples_2d

if __name__ == "__main__":
    run_task1()
