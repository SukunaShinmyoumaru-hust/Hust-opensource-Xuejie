import csv
import matplotlib.pyplot as plt

input_file = 'Loss.csv'
train_loss = []
test_loss = []

with open(input_file, 'r') as file:
    reader = csv.reader(file)
    next(reader)
    for row in reader:
        train_loss.append(float(row[0]))
        test_loss.append(float(row[1]))

plt.plot(train_loss, label='Train Loss')
plt.plot(test_loss, label='Test Loss')
plt.xlabel('Mini-Batch')
plt.ylabel('Loss')
plt.title('Train Loss and Test Loss')
plt.legend()

plt.show()