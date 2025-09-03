import random

def random_modify_segmentation(file_path, modification_prob):
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    modified_lines = []
    for line in lines:
        if line.strip():
            parts = line.strip().split(' ')
            if len(parts) == 2:
                segment, label = parts
                if random.random() < modification_prob:
                    # Randomly modify the label
                    label = modify_label(label)
                modified_lines.append(f'{segment} {label}\n')
            else:
                print(f'Skipping invalid line: {line}')
        else:
            modified_lines.append('\n')

    modified_file_path = file_path.replace('.txt', '_modified.txt')
    with open(modified_file_path, 'w', encoding='utf-8') as modified_file:
        modified_file.writelines(modified_lines)

    print(f'Modified segmentation saved to: {modified_file_path}')

def modify_label(label):
    valid_labels = ['B-ORG', 'I-ORG', 'E-ORG', 'O']
    modified_label = random.choice(valid_labels)
    while modified_label == label:
        modified_label = random.choice(valid_labels)
    return modified_label

# Example usage
file_path = 'segmentation_results.txt'
modification_prob = 0.002  # Probability of modifying each label (adjust as needed)
random_modify_segmentation(file_path, modification_prob)
