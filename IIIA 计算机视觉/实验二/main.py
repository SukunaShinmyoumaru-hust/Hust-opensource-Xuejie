import tensorflow as tf
from tensorflow.keras import layers
from tensorflow.keras.callbacks import Callback

# 定义ResNet模块
class ResNetBlock(layers.Layer):
    def __init__(self, filters):
        super(ResNetBlock, self).__init__()
        self.conv1 = layers.Conv2D(filters, (1, 1), activation='relu')
        self.bn1 = layers.BatchNormalization()
        self.conv2 = layers.Conv2D(filters, (3, 3), activation='relu', padding='same')
        self.bn2 = layers.BatchNormalization()
        self.conv3 = layers.Conv2D(filters, (1, 1))
        self.bn3 = layers.BatchNormalization()

    def call(self, inputs):
        x = self.conv1(inputs)
        x = self.bn1(x)
        x = self.conv2(x)
        x = self.bn2(x)
        x = self.conv3(x)
        x = self.bn3(x)
        x = layers.Add()([inputs, x])
        x = layers.Activation('relu')(x)
        return x

# 定义卷积神经网络结构
class CNNModel(tf.keras.Model):
    def __init__(self):
        super(CNNModel, self).__init__()
        self.conv1 = layers.Conv2D(32, (3, 3), activation='relu', input_shape=(28, 28, 1))  # 卷积层
        self.bn1 = layers.BatchNormalization()  # 批归一化层
        self.maxpool = layers.MaxPooling2D((2, 2))  # 最大池化层
        self.resnet_block1 = ResNetBlock(32)  # ResNet模块1
        self.resnet_block2 = ResNetBlock(32)  # ResNet模块2
        self.flatten = layers.Flatten()  # 扁平化层
        self.fc1 = layers.Dense(64, activation='relu')  # 全连接层
        self.dropout = layers.Dropout(0.0001) # Dropout层
        self.fc2 = layers.Dense(10, activation='softmax') # 全连接层（也就是输出层）

    def call(self, inputs):
        x = self.conv1(inputs)
        x = self.bn1(x)
        x = self.maxpool(x)
        x = self.resnet_block1(x)
        x = self.resnet_block2(x)
        x = self.flatten(x)
        x = self.fc1(x)
        x = self.dropout(x)
        output = self.fc2(x)
        return output

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
x_train = x_train / 255.0
x_test = x_test / 255.0

x_train = tf.expand_dims(x_train, -1)
x_test = tf.expand_dims(x_test, -1)

model = CNNModel()
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

history = model.fit(x_train, y_train, epochs=10, batch_size=512, validation_data=(x_test, y_test), callbacks=[LossHistory((x_train, y_train), (x_test, y_test))])

train_loss, train_acc = model.evaluate(x_train, y_train, verbose=2)
test_loss, test_acc = model.evaluate(x_test, y_test, verbose=2)
print('Train loss:', train_loss)
print('Train accuracy:', train_acc)
print('Test loss:', test_loss)
print('Test accuracy:', test_acc)

import numpy as np
from sklearn.metrics import classification_report

y_pred_probabilities = model.predict(x_test)
y_pred = np.argmax(y_pred_probabilities, axis=-1)

# 使用 classification_report 函数计算每个类别的评估指标
report = classification_report(y_test, y_pred, digits=10, output_dict=False)
print(report)