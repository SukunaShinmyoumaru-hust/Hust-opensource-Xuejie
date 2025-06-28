import pandas as pd
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from tensorflow.keras.callbacks import Callback

class LossHistory(Callback):
    def __init__(self, train_data, test_data):
        self.train_data = train_data
        self.test_data = test_data

    def on_batch_end(self, batch, logs=None):
        train_loss = logs.get('loss')
        test_loss = self.model.evaluate(self.test_data[0], self.test_data[1], verbose=0)
        print("      Mini-Batch:", batch, "- Train Loss:", train_loss, " - Test Loss:", test_loss[0])

    def on_epoch_end(self, epoch, logs=None):
        pass

data = pd.read_csv('dataset.csv')

# 随机打乱数据集
data = data.sample(frac=1).reset_index(drop=True)

train_size = int(0.9 * len(data))
train_data = data[:train_size]
test_data = data[train_size:]

train_features = train_data[['data1', 'data2']].to_numpy()
train_labels = train_data['label'].to_numpy() - 1
test_features = test_data[['data1', 'data2']].to_numpy()
test_labels = test_data['label'].to_numpy() - 1

# 定义神经网络架构，网络层数、激活函数、神经元个数可修改
model = tf.keras.Sequential([
    tf.keras.layers.Dense(8, activation='sigmoid', input_shape=(2,)),
    tf.keras.layers.Dense(8, activation='sigmoid'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(4, activation='softmax')
])

model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

history = model.fit(train_features, train_labels, epochs=40, batch_size=32, validation_data=(test_features, test_labels), callbacks=[LossHistory((train_features, train_labels), (test_features, test_labels))])

train_loss, train_accuracy = model.evaluate(train_features, train_labels, verbose=0)
test_loss, test_accuracy = model.evaluate(test_features, test_labels, verbose=0)

print("训练集损失train_loss：", train_loss)
print("训练集准确率train_acc：", train_accuracy)
print("测试集损失test_loss：", test_loss)
print("测试集准确率test_acc：", test_accuracy)