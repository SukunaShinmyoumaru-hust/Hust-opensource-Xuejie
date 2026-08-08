import os
import urllib.request
import re

def download_file(url, save_path):
    print(f"Downloading {url} to {save_path}...")
    try:
        # User agent to avoid blocking
        req = urllib.request.Request(
            url, 
            headers={'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36'}
        )
        with urllib.request.urlopen(req) as response:
            with open(save_path, 'wb') as f:
                f.write(response.read())
        print("Download successful.")
        return True
    except Exception as e:
        print(f"Failed to download: {e}")
        return False

def clean_and_normalize(line):
    # Strip whitespace at both ends
    line = line.strip()
    if not line:
        return ""
    # Standardize multiple spaces into two spaces (or a single space, we will use two spaces to match original train.txt)
    # The original train.txt uses two spaces or multiple spaces for word boundary.
    # Let's clean the line by splitting and joining with two spaces.
    words = line.split()
    if not words:
        return ""
    return "  ".join(words)

def merge_datasets():
    pku_url = "https://raw.githubusercontent.com/yuikns/icwb2-data/master/training/pku_training.utf8"
    
    # Define paths
    my_dir = os.path.dirname(os.path.abspath(__file__))
    data_dir = os.path.join(my_dir, "data")
    os.makedirs(data_dir, exist_ok=True)
    
    temp_pku_path = os.path.join(data_dir, "pku_training_raw.txt")
    original_train_path = os.path.abspath(os.path.join(my_dir, "..", "2 基于深度学习版 （Bi-LSTM+CRF）", "data", "train.txt"))
    output_path = os.path.join(data_dir, "train_expanded.txt")
    
    # Download SIGHAN PKU dataset if not exists
    if not os.path.exists(temp_pku_path):
        success = download_file(pku_url, temp_pku_path)
        if not success:
            # Try alternate mirror just in case github raw is blocked or slow
            mirror_url = "https://gitee.com/yuikns/icwb2-data/raw/master/training/pku_training.utf8"
            print("Trying alternate mirror Gitee...")
            success = download_file(mirror_url, temp_pku_path)
            if not success:
                print("Error: Could not retrieve SIGHAN PKU training dataset.")
                return
    
    print("Normalizing and merging datasets...")
    total_lines = 0
    original_lines_cnt = 0
    sighan_lines_cnt = 0
    
    with open(output_path, 'w', encoding='utf-8') as out_f:
        # 1. Read original train.txt
        if os.path.exists(original_train_path):
            print(f"Reading original train.txt from {original_train_path}...")
            with open(original_train_path, 'r', encoding='utf-8') as in_f:
                for line in in_f:
                    cleaned = clean_and_normalize(line)
                    if cleaned:
                        out_f.write(cleaned + "\n")
                        original_lines_cnt += 1
                        total_lines += 1
        else:
            print(f"Warning: Original train.txt not found at {original_train_path}")
            
        # 2. Read downloaded SIGHAN PKU corpus
        if os.path.exists(temp_pku_path):
            print(f"Reading SIGHAN PKU training set from {temp_pku_path}...")
            # SIGHAN PKU corpus is usually in UTF-8
            with open(temp_pku_path, 'r', encoding='utf-8', errors='ignore') as in_f:
                for line in in_f:
                    cleaned = clean_and_normalize(line)
                    if cleaned:
                        out_f.write(cleaned + "\n")
                        sighan_lines_cnt += 1
                        total_lines += 1
                        
    print(f"Integration complete!")
    print(f"Original train.txt lines written: {original_lines_cnt}")
    print(f"SIGHAN PKU lines written: {sighan_lines_cnt}")
    print(f"Total merged lines in train_expanded.txt: {total_lines}")

if __name__ == "__main__":
    merge_datasets()
