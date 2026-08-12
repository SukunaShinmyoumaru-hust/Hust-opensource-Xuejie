import os
import subprocess
import time

def run_script(script_name):
    print("=" * 60)
    print(f"Running {script_name}...")
    print("=" * 60)
    start_time = time.time()
    
    import sys
    result = subprocess.run([sys.executable, script_name], capture_output=True, text=True)
    
    end_time = time.time()
    print(result.stdout)
    if result.stderr:
        print("ERRORS/WARNINGS:")
        print(result.stderr)
        
    if result.returncode != 0:
        print(f"FAILED: {script_name} returned code {result.returncode}")
        exit(result.returncode)
    else:
        print(f"SUCCESS: {script_name} completed in {end_time - start_time:.2f} seconds.")
    print("\n")

def main():
    # Set workspace working directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    
    print("Starting all tasks for Experiment 4: Temporal Difference Learning...")
    
    # Run DP benchmark first
    run_script("dp_benchmark.py")
    
    # Run Task 2: TD(0) and First-Visit MC evaluation
    run_script("task2_td0_eval.py")
    
    # Run Task 3: Sarsa and n-step Sarsa control
    run_script("task3_sarsa.py")
    
    # Run Task 4: Q-learning, offline learning, Cliff Walking
    run_script("task4_qlearning.py")
    
    print("All tasks completed successfully! Plots can be found in 'results/'.")

if __name__ == '__main__':
    main()
