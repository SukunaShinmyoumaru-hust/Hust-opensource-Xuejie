import os
import subprocess
import sys

def run_script(script_name):
    print("=" * 60)
    print(f"Running {script_name}...")
    print("=" * 60)
    try:
        # Run with the current python interpreter
        result = subprocess.run([sys.executable, script_name], check=True, text=True, capture_output=True)
        print(result.stdout)
        if result.stderr:
            print("Errors/Warnings:")
            print(result.stderr)
    except subprocess.CalledProcessError as e:
        print(f"Error executing {script_name}:")
        print(e.stdout)
        print(e.stderr)
        sys.exit(1)

def main():
    # Make sure we are in the correct directory
    current_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(current_dir)
    
    os.makedirs("results", exist_ok=True)
    
    # Run all tasks
    run_script("task1_td_linear.py")
    run_script("task2_sarsa.py")
    run_script("task3_q_learning.py")
    run_script("task4_dqn.py")
    
    print("\n" + "=" * 60)
    print("All tasks executed successfully! Plots are saved in results/.")
    print("=" * 60)

if __name__ == "__main__":
    main()
