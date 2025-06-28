import pandas as pd
import matplotlib.pyplot as plt

# 读取excel文件
file_path = 'covid19_data.xls'
data = pd.read_excel(file_path, sheet_name='data_history')

# 设置中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']

# 绘制折线图
plt.figure(figsize=(10, 5))
plt.plot(data['date'], data['confirm'], label='确诊', color='r', linestyle='-.', marker='o')
plt.plot(data['date'], data['dead'], label='死亡', color='g', linestyle='--', marker='s')
plt.plot(data['date'], data['heal'], label='治愈', color='b', linestyle=':', marker='^')

# 设置坐标轴标签
plt.xlabel('日期')
plt.ylabel('数量')

# 设置坐标轴刻度
plt.xticks(rotation=45)
plt.yticks(range(0, max(data['confirm'])+1000, 10000))

# 显示图例
plt.legend()

# 显示折线图
plt.show()

# 绘制散点图
plt.figure(figsize=(10, 5))
plt.scatter(data['date'], data['confirm'], label='确诊', color='r', marker='o')
plt.scatter(data['date'], data['dead'], label='死亡', color='g', marker='s')
plt.scatter(data['date'], data['heal'], label='治愈', color='b', marker='^')

# 设置坐标轴标签
plt.xlabel('日期')
plt.ylabel('数量')

# 设置坐标轴刻度
plt.xticks(rotation=45)
plt.yticks(range(0, max(data['confirm'])+1000, 10000))

# 显示图例
plt.legend()

# 显示散点图
plt.show()