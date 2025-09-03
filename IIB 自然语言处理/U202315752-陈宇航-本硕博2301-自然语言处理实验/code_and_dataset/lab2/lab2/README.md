实验中代码仅提供一种简单的 Bi-LSTM+CRF PyTorch 实现方案。

更优实现可参见：https://github.com/bamtercelboo/pytorch_NER_BiLSTM_CNN_CRF/

## 环境搭建

1. 安装 Anaconda

   <a href="https://zhuanlan.zhihu.com/p/75717350">windows 下安装教程</a>

   官方文档：<a href="https://docs.continuum.io/anaconda/install/">anaconda install</a>

2. 搭建虚拟环境并安装 PyTorch
    ```shell
    # 创建虚拟环境
    conda create -n nlplab python=3.7	# 创建名为 nlplab 的虚拟环境
    
    # 虚拟环境相关命令
    conda activate nlplab  # 激活虚拟环境nlplab，成功执行后应看到命令行首部由 (base) 变为 (nlplab)
    conda deactivate       # 退出当前虚拟环境
    conda info -e          # 查看所有虚拟环境，*指示当前所处环境
    
    # 安装 Pytorch 1.6.0 CPU 版本
    # 注意：先激活 nlplab 虚拟环境，再进行安装
    conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch/
    conda install pytorch==1.6.0 cpuonly
    ```

## 运行方式

1. 配置 PyCharm

   安装 PyCharm，并在 PyCharm 中使用 Anaconda 虚拟环境 (<a href="https://jingyan.baidu.com/article/f3e34a12e7b015f5eb653523.html">参考</a>)

2. 安装其他依赖

   ```sh
   # 在 nlplab 虚拟环境中安装
   pip install -r requirements.txt
   ```

3. 训练

   ```shell
   # save 目录下存放了一个粗略训练过的模型，可先跳过训练过程直接进行推断
   
   # 模型训练，项目根目录下运行
   # 若安装并配置了 GPU 相关运行环境可添加命令行参数 --cuda 来使用 GPU 训练
   python run.py
   ```
   
4. 推断

   ```shell
   python infer.py
   
   # 去除多余的空白行
   python format.py
   ```
   
   