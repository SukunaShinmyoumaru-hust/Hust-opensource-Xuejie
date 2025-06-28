import pandas as pd
import matplotlib.pyplot as plt

# 读取Excel文件
data = pd.read_excel('covid19_data.xls', sheet_name='data_world')

# 按确诊人数降序排序，取前4个国家
top_countries = data.nlargest(4, 'confirm')

# 设置中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']

# 创建一个包含4个子图的画布
fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(12, 8))

# 绘制每个国家的饼图
for i, country in enumerate(top_countries['country']):
    # 获取该国家的confirm、dead、heal和suspect数据
    confirm = top_countries.loc[top_countries['country'] == country, 'confirm'].values[0]
    dead = top_countries.loc[top_countries['country'] == country, 'dead'].values[0]
    heal = top_countries.loc[top_countries['country'] == country, 'heal'].values[0]
    suspect = top_countries.loc[top_countries['country'] == country, 'suspect'].values[0]

    # 绘制饼图
    ax = axes[i // 2, i % 2]
    ax.pie([confirm, dead, heal, suspect], labels=['confirm', 'dead', 'heal', 'suspect'], autopct='%1.1f%%')
    ax.set_title(country)

# 调整子图之间的间距
plt.tight_layout()

# 显示图形
plt.show()