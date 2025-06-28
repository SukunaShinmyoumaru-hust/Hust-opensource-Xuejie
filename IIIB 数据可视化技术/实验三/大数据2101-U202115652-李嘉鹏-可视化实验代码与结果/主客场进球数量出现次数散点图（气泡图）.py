import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('WorldCupMatches.csv')

df_clean = df[['Home Team Goals', 'Away Team Goals']].dropna()
df_clean['Home Team Goals'] = df_clean['Home Team Goals'].astype(float)
df_clean['Away Team Goals'] = df_clean['Away Team Goals'].astype(float)

counts = df_clean.groupby(['Home Team Goals', 'Away Team Goals']).size().reset_index(name='Counts')

x = counts['Home Team Goals']
y = counts['Away Team Goals']
sizes = counts['Counts']

# 设置散点图颜色范围
cmin, cmax = 0, 10

# 设置颜色数组
colors = counts['Counts']

plt.scatter(x, y, s=15*sizes, c=colors, cmap='rainbow', vmin=cmin, vmax=cmax)
plt.colorbar(label='Counts')
plt.xlabel('Home Team Goals')
plt.ylabel('Away Team Goals')
plt.title('主客场进球数量出现次数散点图（气泡图）')

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.show()