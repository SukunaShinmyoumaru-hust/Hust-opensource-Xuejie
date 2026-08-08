import os
import time
from task1_sample_mean import run_task1
from task2_robbins_monro import run_task2
from task3_stochastic_optimization import run_task3
from task4_convergence_analysis import run_task4

def main():
    print("=========================================")
    print("Starting Experiment 3: Stochastic Approximation")
    print("=========================================")
    
    start_time = time.time()
    results_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "results"))
    os.makedirs(results_dir, exist_ok=True)
    print(f"Results will be saved to: {results_dir}\n")
    
    print("--- Running Task 1: Sample Mean Direct vs Incremental ---")
    run_task1(results_dir)
    print("Task 1 completed successfully.\n")
    
    print("--- Running Task 2: Robbins-Monro Iteration ---")
    run_task2(results_dir)
    print("Task 2 completed successfully.\n")
    
    print("--- Running Task 3: Optimization with BGD, MBGD, SGD ---")
    run_task3(results_dir)
    print("Task 3 completed successfully.\n")
    
    print("--- Running Task 4: Multi-seed Statistics & Outlier Analysis ---")
    run_task4(results_dir)
    print("Task 4 completed successfully.\n")
    
    elapsed_time = time.time() - start_time
    print("=========================================")
    print(f"All tasks completed in {elapsed_time:.2f} seconds.")
    print("Plots generated in results directory:")
    for f in sorted(os.listdir(results_dir)):
        print(f"  - {f}")
    print("=========================================")

if __name__ == "__main__":
    main()
