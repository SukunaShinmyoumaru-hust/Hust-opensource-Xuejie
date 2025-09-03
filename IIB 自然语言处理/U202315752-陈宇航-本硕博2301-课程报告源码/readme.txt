nlp_project/
└── segmentation/
    ├── demo.py               # 交互式演示：一次性加载 3 种分词器并在终端逐句对比
    ├── scorer.py             # 评测脚本：计算 Precision／Recall／F1
    ├── seg.py                # 批处理入口：按参数调用分词器、计时并写结果
    │
    ├── model/                # 分词算法主体
    │   ├── __init__.py       # 空文件，声明 Python 包
    │   ├── mm_seg.py         # 最大匹配（FMM／BMM／BiMM）
    │   ├── unigram_seg.py    # Uni-gram + 动态规划
    │   ├── hmm_seg.py        # HMM + BMES + 维特比
    │   ├── dict_generator.py # 把训练语料转 JSON 词典 / 读取搜狗词典
    │   │
    │   ├── dicts/
    │   │   └── pku_dict.json # PKU 词典（UTF-16，内含词频与总词数 _t_）
    │   │
    │   ├── datasets/
    │   │   └── pku/
    │   │       ├── pku_training_words.utf8 # 官方训练分词语料
    │   │       ├── test.txt                # 未分词测试集
    │   │       ├── gold.txt                # 测试集金标准
    │   │       ├── mm_seg.txt              # 历史 MM 分词结果
    │   │       ├── score                   # PKU 官方评分脚本副本
    │   │       └── ...                     # 其他中间产物
    │   │
    │   └── hmm_para/         # 训练好的 HMM 参数
    │       ├── start.npy     # 初始概率 π
    │       ├── trans.npy     # 转移矩阵 A
    │       └── emit.npy      # 发射矩阵 B
    │
    ├── shared/               # 其他算法通用组件
    │   ├── hmm.py            # 通用 HMM 封装 + Builder + I/O
    │   └── smooths.py        # Simple Good–Turing / 加一平滑实现
    │
    └── test_data/
        └── pku/              # 批处理输出与对照集
            ├── test.txt              # 与 model/datasets/pku/test.txt 相同
            ├── gold.txt              # 对照（复制方便评测）
            ├── mm_seg.txt            # 由 seg.py 生成的 MM 结果
            ├── bmm_seg.txt           # 反向最大匹配结果
            ├── bimm_seg.txt          # 双向最大匹配结果
            ├── unigram_seg.txt       # Uni-gram 结果
            └── hmm_seg.txt           # HMM 结果



    