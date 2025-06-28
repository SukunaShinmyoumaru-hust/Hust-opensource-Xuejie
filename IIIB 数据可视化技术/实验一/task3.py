import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 读取excel文件
file_path = 'covid19_data.xls'
current_prov = pd.read_excel(file_path, sheet_name='current_prov')

# 获取需要可视化的数据，例如各省的确诊人数、治愈人数和疑似人数
provinces = current_prov['province']
confirm_data = current_prov['confirm']
heal_data = current_prov['heal']
dead_data = current_prov['dead']

plt.rcParams['font.sans-serif'] = ['SimHei']

# 创建画布和子图
fig, axs = plt.subplots(4, 1, figsize=(10, 10))

# 条形图
index = np.arange(len(provinces))  # 省份的索引
bar_width = 0.2  # 条形宽度

# 条形图：各省确诊人数
rects1 = axs[0].bar(index - bar_width, confirm_data, bar_width, color='skyblue', label='确诊人数')

# 条形图：各省治愈人数
rects2 = axs[0].bar(index, heal_data, bar_width, color='lightcoral', label='治愈人数')

# 条形图：各省死亡人数
rects3 = axs[0].bar(index + bar_width, dead_data, bar_width, color='green', label='死亡人数')

axs[0].set_xlabel('省份')
axs[0].set_ylabel('人数')
axs[0].set_title('各省确诊、治愈和死亡人数对比')
axs[0].set_xticks(index)
axs[0].set_xticklabels(provinces)
axs[0].legend()

# 直方图：各省确诊人数
axs[1].bar(provinces, confirm_data, color='skyblue')
axs[1].set_xlabel('省份')
axs[1].set_ylabel('确诊人数')
axs[1].set_title('各省确诊人数')

axs[2].bar(provinces, heal_data, color='lightcoral')
axs[2].set_xlabel('省份')
axs[2].set_ylabel('治愈人数')
axs[2].set_title('各省治愈人数')

axs[3].bar(provinces, dead_data, color='green')
axs[3].set_xlabel('省份')
axs[3].set_ylabel('死亡人数')
axs[3].set_title('各省死亡人数')

# 调整布局
plt.tight_layout()

# 显示图形
plt.show()