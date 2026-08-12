import os
import numpy as np
import matplotlib.pyplot as plt

# Set random seed for reproducibility
np.random.seed(42)

def generate_samples(N=100):
    np.random.seed(42)
    return np.random.uniform(-10.0, 10.0, (N, 2))

def run_optimizer(samples, method, batch_size, lr_type, lr_0=0.1, epochs=30, w_0=np.array([15.0, 15.0])):
    """
    Runs the optimization algorithm.
    Returns:
      trajectories_by_step: w value at each update step
      trajectories_by_epoch: w value at the end of each epoch
    """
    N = len(samples)
    w = w_0.copy()
    
    traj_step = [w.copy()]
    traj_epoch = [w.copy()]
    
    update_counter = 0
    
    for epoch in range(epochs):
        # Shuffle the samples for stochastic and mini-batch methods
        indices = np.arange(N)
        if method in ['SGD', 'MBGD']:
            np.random.shuffle(indices)
        
        # Split into batches
        for start_idx in range(0, N, batch_size):
            end_idx = min(start_idx + batch_size, N)
            batch_indices = indices[start_idx:end_idx]
            batch_samples = samples[batch_indices]
            
            # Compute average gradient for this batch: \nabla_w f = w - x
            # Grad = w - mean(batch_samples)
            grad = w - np.mean(batch_samples, axis=0)
            
            # Get learning rate
            if lr_type == 'fixed':
                lr = lr_0
            elif lr_type == 'decaying':
                lr = lr_0 / (1.0 + 0.05 * update_counter)
            else:
                raise ValueError(f"Unknown lr_type: {lr_type}")
                
            # Update w
            w = w - lr * grad
            update_counter += 1
            
            traj_step.append(w.copy())
            
        traj_epoch.append(w.copy())
        
    return np.array(traj_step), np.array(traj_epoch)

def run_task3(results_dir="results"):
    os.makedirs(results_dir, exist_ok=True)
    
    samples = generate_samples(100)
    mu = np.array([0.0, 0.0]) # True mean / optimal solution
    w_0 = np.array([15.0, 15.0])
    epochs = 30
    
    configurations = [
        # (method_name, batch_size, label)
        ('BGD', 100, 'BGD (m=100)'),
        ('MBGD', 50, 'MBGD (m=50)'),
        ('MBGD', 5, 'MBGD (m=5)'),
        ('SGD', 1, 'SGD (m=1)')
    ]
    
    lr_types = ['fixed', 'decaying']
    
    results = {}
    
    for lr_t in lr_types:
        results[lr_t] = {}
        for method, m, label in configurations:
            traj_step, traj_epoch = run_optimizer(
                samples=samples, 
                method=method, 
                batch_size=m, 
                lr_type=lr_t, 
                lr_0=0.1, 
                epochs=epochs, 
                w_0=w_0
            )
            results[lr_t][label] = {
                'step': traj_step,
                'epoch': traj_epoch
            }
            
    # Print final error
    print("--- Task 3 Optimization Final Errors (True Optimum w*=[0,0]) ---")
    for lr_t in lr_types:
        print(f"Learning rate type: {lr_t}")
        for _, _, label in configurations:
            final_w = results[lr_t][label]['epoch'][-1]
            error = np.linalg.norm(final_w - mu)
            print(f"  {label:15s} -> Final w: [{final_w[0]:8.5f}, {final_w[1]:8.5f}], Error: {error:8.5f}")
            
    # ---------------------------------------------
    # Plotting 3.1: Parameter Trajectories (2D phase space)
    # ---------------------------------------------
    plt.figure(figsize=(14, 6))
    
    for idx, lr_t in enumerate(lr_types):
        plt.subplot(1, 2, idx + 1)
        plt.plot(mu[0], mu[1], 'r*', markersize=12, label='Optimum $w^*=(0,0)$')
        
        for method, m, label in configurations:
            # For 2D trajectories, let's use the epoch-wise trajectories or step-wise. 
            # Step-wise is much more detailed for SGD.
            traj = results[lr_t][label]['step']
            plt.plot(traj[:, 0], traj[:, 1], marker='o', markersize=2, label=label, alpha=0.7)
            
        plt.xlabel('$w^{(1)}$')
        plt.ylabel('$w^{(2)}$')
        plt.title(f'2D Parameter Trajectories ({lr_t.capitalize()} LR)')
        plt.legend()
        plt.grid(True, linestyle=':', alpha=0.6)
        plt.xlim(-2, 17)
        plt.ylim(-2, 17)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task3_trajectories_2d.png'), dpi=300)
    plt.close()
    
    # ---------------------------------------------
    # Plotting 3.2: Error curves vs Epochs
    # ---------------------------------------------
    plt.figure(figsize=(14, 6))
    
    for idx, lr_t in enumerate(lr_types):
        plt.subplot(1, 2, idx + 1)
        for method, m, label in configurations:
            traj = results[lr_t][label]['epoch']
            errors = np.linalg.norm(traj - mu, axis=1)
            plt.plot(range(epochs + 1), errors, marker='.', label=label)
            
        plt.xlabel('Epoch')
        plt.ylabel('$||w_t - w^*||$')
        plt.title(f'Error vs Epochs ({lr_t.capitalize()} LR)')
        plt.legend()
        plt.yscale('log')
        plt.grid(True, linestyle=':', alpha=0.6)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task3_error_vs_epochs.png'), dpi=300)
    plt.close()

    # ---------------------------------------------
    # Plotting 3.3: Error curves vs Update Steps
    # ---------------------------------------------
    plt.figure(figsize=(14, 6))
    
    for idx, lr_t in enumerate(lr_types):
        plt.subplot(1, 2, idx + 1)
        for method, m, label in configurations:
            traj = results[lr_t][label]['step']
            errors = np.linalg.norm(traj - mu, axis=1)
            plt.plot(range(len(traj)), errors, label=label, alpha=0.8)
            
        plt.xlabel('Update Step $k$')
        plt.ylabel('$||w_k - w^*||$')
        plt.title(f'Error vs Update Steps ({lr_t.capitalize()} LR)')
        plt.legend()
        plt.yscale('log')
        plt.grid(True, linestyle=':', alpha=0.6)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task3_error_vs_steps.png'), dpi=300)
    plt.close()
    
    # Return results for potential use
    return results

if __name__ == "__main__":
    run_task3()
