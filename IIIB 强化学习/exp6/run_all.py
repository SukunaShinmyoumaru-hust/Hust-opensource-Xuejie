import subprocess
import sys
import os

def run_script(script_name):
    print(f"==================== Running {script_name} ====================")
    cmd = [sys.executable, script_name]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    print(result.stdout)
    if result.returncode != 0:
        print(f"Error running {script_name}:", file=sys.stderr)
        print(result.stderr, file=sys.stderr)
        sys.exit(result.returncode)
    print(f"==================== {script_name} completed ====================\n")

if __name__ == "__main__":
    # Ensure working directory is the experiment directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    
    # Run all tasks
    run_script("task1.py")
    run_script("task2.py")
    run_script("task3.py")
    
    print("All tasks ran successfully. All plots saved to exp6/results/.")
