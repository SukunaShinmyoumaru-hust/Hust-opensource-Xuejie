import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 读取Excel文件
data = pd.read_excel('student.xls')
# 将属性值为"作弊"、"缺考"或者为空的情况转换为缺省值0
data = data.replace(["作弊", "缺考"], 0)

# 检测并替换空值为0
data = data.fillna(0)

# (1)获取每个学生的总分以及每门课的分数
data['总分'] = data[['英语', '体育', '军训', '数分', '高代', '解几']].sum(axis=1)
subject_scores = data[['英语', '体育', '军训', '数分', '高代', '解几']]

plt.rcParams['font.sans-serif'] = ['SimHei']

# 创建堆叠柱状图
plt.figure(figsize=(10, 6))
students = data['姓名']
subjects = ['英语', '体育', '军训', '数分', '高代', '解几']
colors = ['skyblue', 'lightgreen', 'orange', 'lightpink', 'lightblue', 'gray']

# 绘制堆叠柱状图
bottom = np.zeros(len(students))
for i, subject in enumerate(subjects):
    scores = subject_scores[subject]
    plt.bar(students, scores, bottom=bottom, color=colors[i], label=subject)
    bottom += scores

# 绘制每个学生每门课的分数和总分
plt.xlabel('姓名')
plt.ylabel('分数')
plt.title('每个学生的每门课的分数和总分')
plt.legend()
plt.xticks(rotation=90)
plt.show()

# (2) 获取总分前3名的分数构成
# 找到总分前3名的学生
top3_students = data.nlargest(3, '总分')

# 创建子图
fig, axes = plt.subplots(nrows=1, ncols=3, figsize=(15, 5))

# 遍历每位学生，生成饼图
for i, student in enumerate(top3_students.iterrows()):
    subject_scores = student[1][['英语', '体育', '军训', '数分', '高代', '解几']]
    subjects = subject_scores.index
    axes[i].pie(subject_scores, labels=subjects, autopct='%1.1f%%')
    axes[i].set_title('学生{}的成绩构成'.format(student[1]['姓名']))

# 调整子图布局
plt.tight_layout()

# 显示图形
plt.show()

# (3) 利用折线图显示英语、数分、高代、解几四门课程的成绩分布图
# 创建一个包含四门课程的列表
courses = ['英语', '数分', '高代', '解几']

# 创建一个包含分数段的列表
score_ranges = ['0-59', '60-69', '70-79', '80-89', '90-100']

# 创建一个空字典用于存储每门课程的统计结果
course_scores = {}

# 统计每门课程每个分数段的人数
for course in courses:
    # 使用pd.cut()函数将分数划分到各个分数段
    score_counts = pd.cut(data[course], bins=[0, 60, 70, 80, 90, 100], labels=score_ranges, right=False).value_counts().sort_index()
    course_scores[course] = score_counts

# 创建折线图
plt.figure(figsize=(10, 6))
for course in courses:
    # 获取横坐标的分数段范围
    x = score_ranges
    # 获取纵坐标的人数
    y = course_scores[course]
    plt.plot(x, y, marker='o', label=course)

plt.xlabel('分数段范围')
plt.ylabel('人数')
plt.title('英语、数分、高代、解几四门课程的成绩分布')
plt.legend()
plt.show()

# (4) 利用柱状图展示男生和女生各科平均成绩的对比
# 计算男生和女生各科平均成绩
male_scores = data[data['性别'] == '男'][['英语', '体育', '军训', '数分', '高代', '解几']].mean()
female_scores = data[data['性别'] == '女'][['英语', '体育', '军训', '数分', '高代', '解几']].mean()

# 创建柱状图
fig, ax = plt.subplots(figsize=(10, 6))
bar_width = 0.35

# 设置男生的柱状图
bar1 = ax.bar(np.arange(len(male_scores)), male_scores, bar_width, label='男生')

# 设置女生的柱状图
bar2 = ax.bar(np.arange(len(female_scores)) + bar_width, female_scores, bar_width, label='女生')

# 设置x轴标签和图例
ax.set_xticks(np.arange(len(male_scores)) + bar_width / 2)
ax.set_xticklabels(male_scores.index)
ax.legend()

# 添加标题和标签
ax.set_title('男生和女生各科平均成绩对比')
ax.set_xlabel('科目')
ax.set_ylabel('平均成绩')

# 显示图形
plt.show()