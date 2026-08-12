import os
import numpy as np
import matplotlib.pyplot as plt

def generate_normal_samples(seed, N=100):
    np.random.seed(seed)
    return np.random.uniform(-10.0, 10.0, (N, 2))

def generate_outlier_samples(seed, N=100, num_outliers=5, outlier_val=50.0):
    np.random.seed(seed)
    samples = np.random.uniform(-10.0, 10.0, (N, 2))
    # Replace last num_outliers samples with outliers
    samples[-num_outliers:] = np.array([outlier_val, outlier_val])
    return samples

def run_optimizer(samples, method, batch_size, lr_type, lr_0=0.1, epochs=30, w_0=np.array([15.0, 15.0])):
    N = len(samples)
    w = w_0.copy()
    traj = [w.copy()]
    update_counter = 0
    
    for epoch in range(epochs):
        indices = np.arange(N)
        if method in ['SGD', 'MBGD']:
            np.random.shuffle(indices)
            
        for start_idx in range(0, N, batch_size):
            end_idx = min(start_idx + batch_size, N)
            batch_indices = indices[start_idx:end_idx]
            batch_samples = samples[batch_indices]
            
            grad = w - np.mean(batch_samples, axis=0)
            
            if lr_type == 'fixed':
                lr = lr_0
            elif lr_type == 'decaying':
                lr = lr_0 / (1.0 + 0.05 * update_counter)
                
            w = w - lr * grad
            update_counter += 1
            traj.append(w.copy())
            
    return np.array(traj)

def run_task4(results_dir="results"):
    os.makedirs(results_dir, exist_ok=True)
    
    seeds = range(42, 42 + 20)
    methods = [
        ('BGD', 100, 'BGD (m=100)'),
        ('MBGD', 50, 'MBGD (m=50)'),
        ('MBGD', 5, 'MBGD (m=5)'),
        ('SGD', 1, 'SGD (m=1)')
    ]
    lr_types = ['fixed', 'decaying']
    w_0 = np.array([15.0, 15.0])
    mu = np.array([0.0, 0.0])
    epochs = 30
    
    # Store final errors
    # structure: errors[lr_type][method_label] = list of 20 final errors
    errors_dict = {lr_t: {m[2]: [] for m in methods} for lr_t in lr_types}
    
    for seed in seeds:
        samples = generate_normal_samples(seed, N=100)
        for lr_t in lr_types:
            for method, m_size, label in methods:
                traj = run_optimizer(samples, method, m_size, lr_t, lr_0=0.1, epochs=epochs, w_0=w_0)
                final_w = traj[-1]
                err = np.linalg.norm(final_w - mu)
                errors_dict[lr_t][label].append(err)
                
    # Calculate Mean and Std
    print("--- Task 4: Final Error Statistical Summary (20 Seeds) ---")
    summary_lines = []
    summary_lines.append("| Learning Rate | Method | Mean Error | Std Error |")
    summary_lines.append("|---|---|---|---|")
    
    for lr_t in lr_types:
        for method, m_size, label in methods:
            errs = errors_dict[lr_t][label]
            mean_err = np.mean(errs)
            std_err = np.std(errs)
            print(f"LR: {lr_t:8s} | {label:15s} | Mean: {mean_err:8.5f} | Std: {std_err:8.5f}")
            summary_lines.append(f"| {lr_t.capitalize()} | {label} | {mean_err:.5f} | {std_err:.5f} |")
            
    # Write summary table to file
    with open(os.path.join(results_dir, "task4_statistical_summary.txt"), "w") as f:
        f.write("\n".join(summary_lines))
        
    # Plot Statistical Comparison Bar Chart
    fig, ax = plt.subplots(figsize=(10, 6))
    x = np.arange(len(methods))
    width = 0.35
    
    fixed_means = [np.mean(errors_dict['fixed'][m[2]]) for m in methods]
    fixed_stds = [np.std(errors_dict['fixed'][m[2]]) for m in methods]
    
    decay_means = [np.mean(errors_dict['decaying'][m[2]]) for m in methods]
    decay_stds = [np.std(errors_dict['decaying'][m[2]]) for m in methods]
    
    rects1 = ax.bar(x - width/2, fixed_means, width, yerr=fixed_stds, label='Fixed LR (0.1)', capsize=5, color='skyblue', edgecolor='black')
    rects2 = ax.bar(x + width/2, decay_means, width, yerr=decay_stds, label='Decaying LR', capsize=5, color='orange', edgecolor='black')
    
    ax.set_ylabel('Final Error $||w_{final} - w^*||$')
    ax.set_title('Final Error Statistics over 20 Seeds')
    ax.set_xticks(x)
    ax.set_xticklabels([m[2] for m in methods])
    ax.legend()
    ax.set_yscale('log')
    ax.grid(True, linestyle=':', alpha=0.6)
    
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task4_statistical_comparison.png'), dpi=300)
    plt.close()
    
    # -----------------------------
    # Outlier Experiment
    # -----------------------------
    # Generate samples with outliers
    outlier_seed = 42
    samples_outlier = generate_outlier_samples(outlier_seed, N=100, num_outliers=5, outlier_val=50.0)
    
    # The true mean of the base distribution is (0,0)
    # The actual sample mean of this dataset is:
    outlier_sample_mean = np.mean(samples_outlier, axis=0)
    print(f"Sample mean with outliers: [{outlier_sample_mean[0]:.4f}, {outlier_sample_mean[1]:.4f}]")
    
    outlier_results = {}
    for lr_t in lr_types:
        outlier_results[lr_t] = {}
        for method, m_size, label in methods:
            traj = run_optimizer(samples_outlier, method, m_size, lr_t, lr_0=0.1, epochs=epochs, w_0=w_0)
            outlier_results[lr_t][label] = traj
            
    # Plot outlier trajectories
    plt.figure(figsize=(14, 6))
    
    for idx, lr_t in enumerate(lr_types):
        plt.subplot(1, 2, idx + 1)
        plt.scatter(samples_outlier[:, 0], samples_outlier[:, 1], color='red', alpha=0.6, marker='x', label='Samples (incl. Outliers)')
        plt.plot(mu[0], mu[1], 'g*', markersize=14, label='True Mean $(0,0)$')
        plt.plot(outlier_sample_mean[0], outlier_sample_mean[1], 'b^', markersize=10, label=f'Sample Mean ({outlier_sample_mean[0]:.1f}, {outlier_sample_mean[1]:.1f})')
        
        for method, m_size, label in methods:
            traj = outlier_results[lr_t][label]
            plt.plot(traj[:, 0], traj[:, 1], marker='o', markersize=2, label=label, alpha=0.8)
            
        plt.xlabel('$w^{(1)}$')
        plt.ylabel('$w^{(2)}$')
        plt.title(f'Trajectories with Outliers ({lr_t.capitalize()} LR)')
        plt.legend()
        plt.grid(True, linestyle=':', alpha=0.6)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task4_outlier_trajectories.png'), dpi=300)
    plt.close()
    
    # Plot outlier error curves vs update step
    # Note that the "error" can be measured w.r.t the true distribution mean (0,0)
    plt.figure(figsize=(14, 6))
    for idx, lr_t in enumerate(lr_types):
        plt.subplot(1, 2, idx + 1)
        for method, m_size, label in methods:
            traj = outlier_results[lr_t][label]
            errors = np.linalg.norm(traj - mu, axis=1) # W.r.t true mean (0,0)
            plt.plot(range(len(traj)), errors, label=label, alpha=0.8)
            
        plt.xlabel('Update Step $k$')
        plt.ylabel('$||w_k - \mu_{true}||$')
        plt.title(f'Error w.r.t True Mean ({lr_t.capitalize()} LR)')
        plt.legend()
        plt.yscale('log')
        plt.grid(True, linestyle=':', alpha=0.6)
        
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'task4_outlier_errors.png'), dpi=300)
    plt.close()
    
    # Print final values with outliers
    print("--- Task 4 Outlier Experiment Final Positions ---")
    for lr_t in lr_types:
        print(f"Learning rate type: {lr_t}")
        for _, _, label in methods:
            final_w = outlier_results[lr_t][label][-1]
            error_to_true = np.linalg.norm(final_w - mu)
            error_to_sample = np.linalg.norm(final_w - outlier_sample_mean)
            print(f"  {label:15s} -> Final w: [{final_w[0]:8.5f}, {final_w[1]:8.5f}], Error to True: {error_to_true:8.5f}, Error to Sample Mean: {error_to_sample:8.5f}")

if __name__ == "__main__":
    run_task4()
