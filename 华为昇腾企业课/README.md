# 华为《昇腾目标检测实战》代码平台练习完成记录

### 计算机科学与技术学院  大数据2101班  李嘉鹏  U202115652

2023.10.29

 

*实验一：基于MindSpore的手写体识别*

结果：

![image-20231029132149556](/华为昇腾企业课/image-20231029132149556.png)

代码：

```python
import os
import argparse
from mindspore import context
from mindspore.train.callback import Callback, LossMonitor

parser = argparse.ArgumentParser(description='MindSpore LeNet Example')
parser.add_argument('--device_target', type=str, default="CPU", choices=['Ascend', 'GPU', 'CPU'])

args = parser.parse_known_args()[0]
# context.set_context(mode=context.GRAPH_MODE, device_target=args.device_target)

import os
import requests

requests.packages.urllib3.disable_warnings()

def download_dataset(dataset_url, path):
    filename = dataset_url.split("/")[-1]
    save_path = os.path.join(path, filename)
    if os.path.exists(save_path):
        return
    if not os.path.exists(path):
        os.makedirs(path)
    res = requests.get(dataset_url, stream=True, verify=False)
    with open(save_path, "wb") as f:
        for chunk in res.iter_content(chunk_size=512):
            if chunk:
                f.write(chunk)
    print("The {} file is downloaded and saved in the path {} after processing".format(os.path.basename(dataset_url), path))

train_path = "datasets/MNIST_Data/train"
test_path = "datasets/MNIST_Data/test"

download_dataset("https://mindspore-website.obs.myhuaweicloud.com/notebook/datasets/mnist/train-labels-idx1-ubyte", train_path)
download_dataset("https://mindspore-website.obs.myhuaweicloud.com/notebook/datasets/mnist/train-images-idx3-ubyte", train_path)
download_dataset("https://mindspore-website.obs.myhuaweicloud.com/notebook/datasets/mnist/t10k-labels-idx1-ubyte", test_path)
download_dataset("https://mindspore-website.obs.myhuaweicloud.com/notebook/datasets/mnist/t10k-images-idx3-ubyte", test_path)

import mindspore.dataset as ds
import mindspore.dataset.transforms.c_transforms as C
import mindspore.dataset.vision.c_transforms as CV
from mindspore.dataset.vision import Inter
from mindspore import dtype as mstype

def create_dataset(data_path, batch_size=32, repeat_size=1,num_parallel_workers=1):
    # 定义数据集
    mnist_ds = ds.MnistDataset(data_path)
    # Todo 设置放缩的大小
    resize_height = 32
    resize_width = 32
    # Todo 归一化
    rescale = 1/255.
    shift = 0.0
    rescale_nml = 1 / 0.3081
    shift_nml = -1 * 0.1307 / 0.3081

    # 定义所需要操作的map映射
    resize_op = CV.Resize((resize_height, resize_width), interpolation=Inter.LINEAR)
    rescale_nml_op = CV.Rescale(rescale_nml, shift_nml)
    rescale_op = CV.Rescale(rescale, shift)
    hwc2chw_op = CV.HWC2CHW()
    type_cast_op = C.TypeCast(mstype.int32)

    # 使用map映射函数，将数据操作应用到数据集
    mnist_ds = mnist_ds.map(operations=type_cast_op, input_columns="label", num_parallel_workers=num_parallel_workers)
    mnist_ds = mnist_ds.map(operations=[resize_op, rescale_op, rescale_nml_op, hwc2chw_op], input_columns="image", num_parallel_workers=num_parallel_workers)

    # Todo 进行shuffle、batch、repeat操作
    buffer_size = 10000
    mnist_ds = mnist_ds.shuffle(buffer_size=buffer_size)
    mnist_ds = mnist_ds.batch(batch_size, drop_remainder=True)
    mnist_ds = mnist_ds.repeat(repeat_size)

    return mnist_ds

import mindspore.nn as nn
from mindspore.common.initializer import Normal


# Todo 根据LeNet5网络结构神经网络
class LeNet5(nn.Cell):
    """
    Lenet网络结构
    """
    def __init__(self, num_class=10, num_channel=1):
        super(LeNet5, self).__init__()
        self.conv1 = nn.Conv2d(num_channel, 6, 5, pad_mode='valid')
        self.conv2 = nn.Conv2d(6, 16, 5, pad_mode='valid')
        self.fc1 = nn.Dense(16 * 5 * 5, 120, weight_init=Normal(0.02))
        self.fc2 = nn.Dense(120, 84, weight_init=Normal(0.02))
        self.fc3 = nn.Dense(84, num_class, weight_init=Normal(0.02))
        self.relu = nn.ReLU()
        self.max_pool2d = nn.MaxPool2d(kernel_size=2, stride=2)
        self.flatten = nn.Flatten()

    def construct(self, x):
        # 使用定义好的运算构建前向网络
        x = self.max_pool2d(self.relu(self.conv1(x)))
        x = self.max_pool2d(self.relu(self.conv2(x)))
        x = self.flatten(x)
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.fc3(x)
        return x

class StepLossAccInfo(Callback):
    def __init__(self, model, eval_dataset, steps_loss, steps_eval):
        self.model = model
        self.eval_dataset = eval_dataset
        self.steps_loss = steps_loss
        self.steps_eval = steps_eval

    def step_end(self, run_context):
        cb_params = run_context.original_args()
        cur_epoch = cb_params.cur_epoch_num
        cur_step = (cur_epoch-1)*1875 + cb_params.cur_step_num
        self.steps_loss["loss_value"].append(str(cb_params.net_outputs))
        self.steps_loss["step"].append(str(cur_step))
        if cur_step % 125 == 0:
            acc = self.model.eval(self.eval_dataset, dataset_sink_mode=False)
            self.steps_eval["step"].append(cur_step)
            self.steps_eval["acc"].append(acc["Accuracy"])

# 实例化网络
net = LeNet5()

# MindSpore支持的损失函数有SoftmaxCrossEntropyWithLogits、L1Loss、MSELoss等。
net_loss = nn.SoftmaxCrossEntropyWithLogits (sparse=True, reduction='mean')

# MindSpore支持的优化器有Adam、AdamWeightDecay、Momentum等。这里使用Momentum优化器为例。
net_opt = nn.Momentum (net.trainable_params(), learning_rate=0.01, momentum=0.9)

from mindspore.train.callback import ModelCheckpoint, CheckpointConfig

config_ck = CheckpointConfig(save_checkpoint_steps=1875, keep_checkpoint_max=10)

ckpoint = ModelCheckpoint(prefix="checkpoint_lenet", config=config_ck)

from mindspore.nn import Accuracy
from mindspore.train.callback import LossMonitor
from mindspore import Model

def train_net(model, epoch_size, data_path, repeat_size, ckpoint_cb, sink_mode):
    """定义训练的方法"""
    # 加载训练数据集
    ds_train = create_dataset(os.path.join(data_path, "train"), 32, repeat_size)
    eval_dataset = create_dataset(os.path.join(mnist_path, "test"), 32)
    steps_loss = {"step": [], "loss_value": []}
    steps_eval = {"step": [], "acc": []}
    step_loss_acc_info = StepLossAccInfo(model, eval_dataset, steps_loss, steps_eval)
    model.train(epoch_size, ds_train, callbacks=[ckpoint_cb, LossMonitor(125), step_loss_acc_info],
                dataset_sink_mode=sink_mode)

from mindspore import load_checkpoint, load_param_into_net
# 加载已经保存的用于测试的模型

def test_net(model, data_path):
    """定义验证的方法"""
    param_dict = load_checkpoint("checkpoint_lenet-1_1875.ckpt")
    load_param_into_net(net, param_dict)
    ds_eval = create_dataset(os.path.join(mnist_path, "test"))
    acc = model.eval(ds_eval, dataset_sink_mode=False)
    print("Acc:{}".format(acc))

train_epoch = 1
mnist_path = "./datasets/MNIST_Data"
dataset_size = 1
model = Model(net, net_loss, net_opt, metrics={"Accuracy": Accuracy()})
train_net(model, train_epoch, mnist_path, dataset_size, ckpoint, False)
test_net(model, mnist_path)

param_dict = load_checkpoint("checkpoint_lenet-1_1875.ckpt")
# 加载参数到网络中
load_param_into_net(net, param_dict)

import numpy as np
from mindspore import Tensor

# 定义测试数据集，batch_size设置为1，则取出一张图片
ds_test = create_dataset(os.path.join(mnist_path, "test"), batch_size=1).create_dict_iterator()
data = next(ds_test)

# images为测试图片，labels为测试图片的实际分类
images = data["image"].asnumpy()
labels = data["label"].asnumpy()

# 使用函数model.predict预测image对应分类
output = model.predict(Tensor(data['image']))
predicted = np.argmax(output.asnumpy(), axis=1)

# 输出预测分类与实际分类
print(f'Predicted: "{predicted[0]}", Actual: "{labels[0]}"')


import matplotlib.pyplot as plt
import numpy as np

ds_test = create_dataset("datasets/MNIST_Data/test", 9)
ds_test = ds_test.create_dict_iterator()
data = next(ds_test)
images = data["image"].asnumpy()
labels = data["label"].asnumpy()

plt.figure()
for i in range(1, 10):
    plt.subplot(3, 3, i)
    plt.imshow(images[i-1][0], interpolation="None", cmap="gray")
plt.show()

import mindspore

# 使用函数model.predict预测image对应分类
output = model.predict(mindspore.Tensor(data['image']))
predicted = np.argmax(output.asnumpy(), axis=1)
# 输出预测分类与实际分类
print(f'Predicted: "{predicted}", Labels: "{labels}"')

```



*实验二：基于MindSpore的YOLOv3目标检测*

结果：（1）启动模型训练

![image-20231029132749720](/华为昇腾企业课/image-20231029132749720.png)

（2）测试结果输出

![image-20231029132954367](/华为昇腾企业课/image-20231029132954367.png)

![image-20231029133019709](/华为昇腾企业课/image-20231029133019709.png)



*实验三：基于MindSpore的手写体识别*

结果：

![image-20231029132328114](/华为昇腾企业课/image-20231029132328114.png)



*实验四：基于MindX SDK开发目标检测应用*

编译完成：

![image-20231030114349950](/华为昇腾企业课/image-20231030114349950.png)

运行：

![image-20231030114434440](/华为昇腾企业课/image-20231030114434440.png)

在原始代码上做修改并编译，达到数据可视化的要求：

![image-20231030115741709](/华为昇腾企业课/image-20231030115741709.png)

![image-20231030115853910](/华为昇腾企业课/image-20231030115853910.png)

![image-20231030115925048](/华为昇腾企业课/image-20231030115925048.png)



附：实验环境搭建

![image-20231029133614382](/华为昇腾企业课/image-20231029133614382.png)

![image-20231030120330387](/华为昇腾企业课/image-20231030120330387.png)



微认证考试通过：

![image-20231030110853198](C:\Users\xiaoy\AppData\Roaming\Typora\typora-user-images\image-20231030110853198.png)
