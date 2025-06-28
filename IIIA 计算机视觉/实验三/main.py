import tensorflow as tf
import numpy as np
from tensorflow.keras.callbacks import Callback

class LossHistory(Callback):
    def __init__(self, train_data, test_data):
        self.train_data = train_data
        self.test_data = test_data

    def on_batch_end(self, batch, logs=None):
        train_loss = logs.get('loss')
        test_loss = self.model.evaluate(self.test_data[0], self.test_data[1], verbose=0)
        print("    Mini-Batch:", batch, "- Train Loss:", train_loss, " - Test Loss:", test_loss[0])

    def on_epoch_end(self, epoch, logs=None):
        pass

mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()

train_samples = int(len(x_train) * 0.1)
test_samples = int(len(x_test) * 0.1)

train_indices = np.random.choice(len(x_train), train_samples, replace=False)
test_indices = np.random.choice(len(x_test), test_samples, replace=False)

x_train = x_train[train_indices] / 255.0
y_train = y_train[train_indices]
x_test = x_test[test_indices] / 255.0
y_test = y_test[test_indices]

# 处理训练集(1)
label0 = 0
label1 = 0
flag = 0 # 当前寻找的标签
new_x_train = []
new_y_train = []
for i in range(0, len(x_train)-1):
    for j in range(i, len(x_train)-1):
        if flag == 1 and y_train[i] == y_train[j] and label1 < 50000:
            new_x_train.append([x_train[i], x_train[j]])
            new_y_train.append(1)
            label1 += 1
            flag = 0
        if flag == 0 and y_train[i] != y_train[j] and label0 < 50000:
            new_x_train.append([x_train[i], x_train[j]])
            new_y_train.append(0)
            label0 += 1
            flag = 1
        if label1 == 50000 and label0 == 50000:
            break

new_x_train = np.array(new_x_train)
new_y_train = np.array(new_y_train)

np.random.seed(0)
random_index = np.random.permutation(len(new_x_train))
new_x_train = new_x_train[random_index]
new_y_train = new_y_train[random_index]

# # 处理训练集(2)
# label0 = 0
# label1 = 0
# new_x_train = []
# new_y_train = []
#
# for i in range(0, len(x_train)-1):
#     for j in range(i, len(x_train)-1):
#         if label0 < 50000 and i < 50000:
#             new_x_train.append([x_train[i], x_train[j]])
#             new_y_train.append(0)
#             label0 += 1
#         elif label1 < 50000 and j >= len(x_train)-50000:
#             new_x_train.append([x_train[i], x_train[j]])
#             new_y_train.append(1)
#             label1 += 1
#         if label0 == 50000 and label1 == 50000:
#             break
#
# new_x_train = np.array(new_x_train)
# new_y_train = np.array(new_y_train)
#
# np.random.seed(0)
# random_index = np.random.permutation(len(new_x_train))
# new_x_train = new_x_train[random_index]
# new_y_train = new_y_train[random_index]

# # 处理训练集(3)
# label0 = 0
# label1 = 0
# flag = 0 # 当前寻找的标签
# new_x_train = []
# new_y_train = []
# for i in range(0, len(x_train)-1):
#     for j in range(i, len(x_train)-1):
#         if flag == 1 and y_train[i] == y_train[j] and label1 < 90000:
#             new_x_train.append([x_train[i], x_train[j]])
#             new_y_train.append(1)
#             label1 += 1
#             flag = 0
#             if label0 == 10000:
#                 flag = 1
#         if flag == 0 and y_train[i] != y_train[j] and label0 < 10000:
#             new_x_train.append([x_train[i], x_train[j]])
#             new_y_train.append(0)
#             label0 += 1
#             flag = 1
#         if label1 == 90000 and label0 == 10000:
#             break
#
# new_x_train = np.array(new_x_train)
# new_y_train = np.array(new_y_train)
#
# np.random.seed(0)
# random_index = np.random.permutation(len(new_x_train))
# new_x_train = new_x_train[random_index]
# new_y_train = new_y_train[random_index]

# 处理测试集
label0 = 0
label1 = 0
flag = 0 # 当前寻找的标签
new_x_test = []
new_y_test = []
for i in range(0, len(x_test)-1):
    for j in range(i, len(x_test)-1):
        if flag == 1 and y_test[i] == y_test[j] and label1 < 2000:
            new_x_test.append([x_test[i], x_test[j]])
            new_y_test.append(1)
            label1 += 1
            flag = 0
        if flag == 0 and y_test[i] != y_test[j] and label0 < 2000:
            new_x_test.append([x_test[i], x_test[j]])
            new_y_test.append(0)
            label0 += 1
            flag = 1
        if label1 == 2000 and label0 == 2000:
            break

new_x_test = np.array(new_x_test)
new_y_test = np.array(new_y_test)

np.random.seed(0)
random_index = np.random.permutation(len(new_x_test))
new_x_test = new_x_test[random_index]
new_y_test = new_y_test[random_index]

count_0 = np.count_nonzero(new_y_train == 0)
count_1 = np.count_nonzero(new_y_train == 1)

print("新的训练集中标签为0的数量：", count_0)
print("新的训练集中标签为1的数量：", count_1)
print("新的训练集数据形状：", new_x_train.shape)
print("新的训练集标签形状：", new_y_train.shape)
print("新的测试集数据形状：", new_x_test.shape)
print("新的测试集标签形状：", new_y_test.shape)

# 调整输入形状
new_x_train = new_x_train.reshape(-1, 56, 28, 1)
new_x_test = new_x_test.reshape(-1, 56, 28, 1)

# 创建卷积神经网络模型
model = tf.keras.Sequential([
    tf.keras.layers.Conv2D(64, 3, activation='relu', input_shape=(56, 28, 1)),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Conv2D(128, 3, activation='relu'),
    tf.keras.layers.MaxPooling2D(),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(256, activation='relu'),
    tf.keras.layers.Dropout(0.5),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dense(2, activation='sigmoid')
])

# 计算类别权重
total_samples = len(new_y_train)
class_weight = {0: total_samples / np.sum(new_y_train == 0), 1: total_samples / np.sum(new_y_train == 1)}

model.compile(optimizer='adam',
              loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
              metrics=['accuracy'])
history = model.fit(new_x_train, new_y_train, epochs=8, batch_size=8000, class_weight=class_weight, callbacks=[LossHistory((new_x_train, new_y_train), (new_x_test, new_y_test))])

train_loss, train_acc = model.evaluate(new_x_train, new_y_train)
test_loss, test_acc = model.evaluate(new_x_test, new_y_test)
print('\nTrain accuracy:', train_acc)
print('\nTrain loss:', train_loss)
print('\nTest accuracy:', test_acc)
print('\nTest loss:', test_loss)

predictions = model.predict(new_x_test)
predictions = np.argmax(predictions, axis=1)
print("\nPredictions:", predictions)

import numpy as np
from sklearn.metrics import classification_report

y_pred_probabilities = model.predict(new_x_test)
y_pred = np.argmax(y_pred_probabilities, axis=-1)

report = classification_report(new_y_test, y_pred, digits=2, output_dict=False)
print(report)