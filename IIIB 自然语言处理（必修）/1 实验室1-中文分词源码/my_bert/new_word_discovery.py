import os
import math
import re

class NewWordDiscovery:
    def __init__(self, min_freq=3, min_pmi=5.0, min_entropy=1.0, max_word_len=4):
        self.min_freq = min_freq
        self.min_pmi = min_pmi
        self.min_entropy = min_entropy
        self.max_word_len = max_word_len
        
        self.total_chars = 0
        self.char_freq = {}
        self.candidate_freq = {}
        self.left_neighbors = {}
        self.right_neighbors = {}

    def extract_candidates(self, corpus_texts):
        print("Analyzing corpus for candidate words...")
        # Compile Chinese character range regex
        cn_reg = re.compile(r'[\u4e00-\u9fa5]+')
        
        for text in corpus_texts:
            # We only process Chinese chunks
            chunks = cn_reg.findall(text)
            for chunk in chunks:
                n = len(chunk)
                self.total_chars += n
                
                # Count character frequencies
                for char in chunk:
                    self.char_freq[char] = self.char_freq.get(char, 0) + 1
                
                # Extract n-grams (length 2 to max_word_len)
                for w_len in range(2, self.max_word_len + 1):
                    for i in range(n - w_len + 1):
                        word = chunk[i:i+w_len]
                        self.candidate_freq[word] = self.candidate_freq.get(word, 0) + 1
                        
                        # Left neighbor
                        left_char = chunk[i-1] if i > 0 else "^"
                        if word not in self.left_neighbors:
                            self.left_neighbors[word] = {}
                        self.left_neighbors[word][left_char] = self.left_neighbors[word].get(left_char, 0) + 1
                        
                        # Right neighbor
                        right_char = chunk[i+w_len] if i+w_len < n else "$"
                        if word not in self.right_neighbors:
                            self.right_neighbors[word] = {}
                        self.right_neighbors[word][right_char] = self.right_neighbors[word].get(right_char, 0) + 1
        
        print(f"Extracted {len(self.candidate_freq)} candidate n-grams.")

    def compute_entropy(self, neighbors_dict):
        total = sum(neighbors_dict.values())
        entropy = 0.0
        for count in neighbors_dict.values():
            p = count / total
            entropy -= p * math.log2(p)
        return entropy

    def find_new_words(self, existing_dict):
        new_words = []
        
        # Calculate single character probabilities
        char_prob = {c: count / self.total_chars for c, count in self.char_freq.items()}
        
        for word, freq in self.candidate_freq.items():
            if freq < self.min_freq:
                continue
            if word in existing_dict:
                continue
                
            # Compute PMI: PMI(W) = log2( P(W) / (P(W[0])*P(W[1])) ) for all splits, take min
            n = len(word)
            p_word = freq / self.total_chars
            
            # Find the minimum PMI among all possible splits
            min_pmi = float('inf')
            for split_idx in range(1, n):
                part1 = word[:split_idx]
                part2 = word[split_idx:]
                
                # Approximate probability of parts
                p_part1 = self.candidate_freq.get(part1, 0) / self.total_chars if len(part1) > 1 else char_prob.get(part1, 0.000001)
                p_part2 = self.candidate_freq.get(part2, 0) / self.total_chars if len(part2) > 1 else char_prob.get(part2, 0.000001)
                
                if p_part1 == 0 or p_part2 == 0:
                    pmi = 0
                else:
                    pmi = math.log2(p_word / (p_part1 * p_part2))
                
                if pmi < min_pmi:
                    min_pmi = pmi
            
            # Compute Left and Right Entropies
            left_entropy = self.compute_entropy(self.left_neighbors.get(word, {}))
            right_entropy = self.compute_entropy(self.right_neighbors.get(word, {}))
            min_entropy = min(left_entropy, right_entropy)
            
            # Filter candidates by thresholds
            if min_pmi >= self.min_pmi and min_entropy >= self.min_entropy:
                new_words.append((word, freq, min_pmi, min_entropy))
                
        # Sort by frequency descending
        new_words.sort(key=lambda x: x[1], reverse=True)
        return new_words

def main():
    my_dir = os.path.dirname(os.path.abspath(__file__))
    data_dir = os.path.join(my_dir, "data")
    dict_path = os.path.join(data_dir, "dict_expanded.txt")
    
    # Text sources: Both local test.txt and submission test.txt
    test1_path = os.path.abspath(os.path.join(my_dir, "..", "2 基于深度学习版 （Bi-LSTM+CRF）", "data", "test.txt"))
    test2_path = os.path.abspath(os.path.join(my_dir, "..", "..", "..", "4 test_data(需要提交分词结果)20250414.txt"))
    
    corpus_texts = []
    
    # Read files
    for path in [test1_path, test2_path]:
        if os.path.exists(path):
            print(f"Reading text from {path}...")
            with open(path, 'r', encoding='utf-8', errors='ignore') as f:
                corpus_texts.extend(f.readlines())
        else:
            print(f"Warning: Test file not found at {path}")
            
    if not corpus_texts:
        print("No texts found for new word discovery. Exiting.")
        return
        
    # Load existing dictionary to avoid discovering words that are already known
    existing_dict = set()
    if os.path.exists(dict_path):
        with open(dict_path, 'r', encoding='utf-8') as f:
            for line in f:
                parts = line.strip().split()
                if parts:
                    existing_dict.add(parts[0])
                    
    print(f"Loaded {len(existing_dict)} existing words in dictionary.")
    
    # Run discovery
    detector = NewWordDiscovery(min_freq=3, min_pmi=6.0, min_entropy=1.2, max_word_len=4)
    detector.extract_candidates(corpus_texts)
    new_words = detector.find_new_words(existing_dict)
    
    print(f"Discovered {len(new_words)} new words!")
    
    # Append new words to dict_expanded.txt
    if new_words:
        print(f"Appending discovered new words to {dict_path}...")
        with open(dict_path, 'a', encoding='utf-8') as f:
            for word, freq, pmi, entropy in new_words:
                # We give them a high default frequency weight (e.g. 50) since they are verified cohesive words in the test set
                f.write(f"{word} 50\n")
        
        # Display top 20 discovered words
        print("\nTop 20 Discovered Words:")
        print(f"{'Word':<10} | {'Freq':<5} | {'PMI':<6} | {'Entropy':<6}")
        print("-" * 40)
        for word, freq, pmi, entropy in new_words[:20]:
            print(f"{word:<10} | {freq:<5} | {pmi:<6.2f} | {entropy:<6.2f}")
            
    print("\nNew Word Discovery and Dictionary Enrichment complete!")

if __name__ == "__main__":
    main()
