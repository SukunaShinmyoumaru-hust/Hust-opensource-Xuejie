import re
import csv
import chardet

input_file = "output.log"
output_file = "Loss.csv"
loss_values = []

with open(input_file, 'rb') as file:
    result = chardet.detect(file.read())
encoding = result['encoding']

with open(input_file, 'r', encoding=encoding) as file:
    for line in file:
        match = re.search(r'Train Loss: (\d+\.\d+)  - Test Loss: (\d+\.\d+)', line)
        if match:
            train_loss = float(match.group(1))
            test_loss = float(match.group(2))
            loss_values.append([train_loss, test_loss])

with open(output_file, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Train Loss', 'Test Loss'])
    for values in loss_values:
        writer.writerow(values)