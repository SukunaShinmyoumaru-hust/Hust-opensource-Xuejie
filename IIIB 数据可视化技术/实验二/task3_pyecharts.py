import pandas as pd
from pyecharts import options as opts
from pyecharts.charts import Bar, Pie, Line, Grid, Page

data = pd.read_excel('student.xls')
data = data.replace(["作弊", "缺考"], 0)
data = data.fillna(0)

data['总分'] = data[['英语', '体育', '军训', '数分', '高代', '解几']].sum(axis=1)
subject_scores = data[['英语', '体育', '军训', '数分', '高代', '解几']]
students = data['姓名']
subjects = ['英语', '体育', '军训', '数分', '高代', '解几']
colors = ['skyblue', 'lightgreen', 'orange', 'lightpink', 'lightblue', 'gray']

# (1) 利用堆叠柱状图显示所有学生的总分
bar = (
    Bar(init_opts=opts.InitOpts(width='1000px', height='600px'))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="每个学生每门课的分数和总分"),
        xaxis_opts=opts.AxisOpts(axislabel_opts=opts.LabelOpts(rotate=90)),
        legend_opts=opts.LegendOpts(type_='scroll', pos_top='10%', pos_bottom='10%'),
    )
)

for i, subject in enumerate(subjects):
    bar.add_xaxis(students.tolist())
    bar.add_yaxis(subject, subject_scores[subject].tolist(), stack='stack1', color=colors[i])

bar.render('bar.html')

# (2) 利用饼图展示总分前3名同学的分数构成
top_students = data.nlargest(3, '总分')
# 使用page，把三个饼图画在一个page上
page = Page()
for _, student in top_students.iterrows():
    pie = Pie()
    scores = [student['英语'], student['体育'], student['军训'], student['数分'], student['高代'], student['解几']]
    pie.add(student['姓名'], list(zip(['英语', '体育', '军训', '数分', '高代', '解几'], scores)))
    pie.set_global_opts(title_opts=opts.TitleOpts(title=f"总分前3名的分数构成: {student['姓名']}"))
    page.add(pie)

page.render('pie.html')

# (3) 利用折线图显示英语、数分、高代、解几四门课程的成绩分布图（统计分数段内的人数，分数段按照每10分进行统计，然后用折线图展示）
# 这里引入了全部六门课程的成绩分布图，如果只需要保留其中一部分，则点击图例去掉无关项即可
score_ranges = ['0-59', '60-69', '70-79', '80-89', '90-100']
course_scores = {}
for course in subjects:
    score_counts = pd.cut(data[course], bins=[0, 60, 70, 80, 90, 100], labels=score_ranges, right=False).value_counts().sort_index()
    course_scores[course] = score_counts.tolist()

line = (
    Line(init_opts=opts.InitOpts(width='1000px', height='600px'))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="六门课程的成绩分布"),
        xaxis_opts=opts.AxisOpts(name="分数段范围"),
        yaxis_opts=opts.AxisOpts(name="人数"),
        legend_opts=opts.LegendOpts(type_='scroll', pos_top='10%', pos_bottom='10%'),
    )
)

for course in subjects:
    line.add_xaxis(score_ranges)
    line.add_yaxis(course, course_scores[course], symbol='circle')

line.render('score.html')

# (4) 利用合适的可视化图形（分组柱状图）展示男生和女生各科平均成绩的对比
male_scores = data[data['性别'] == '男'][['英语', '体育', '军训', '数分', '高代', '解几']].mean().round(2)
female_scores = data[data['性别'] == '女'][['英语', '体育', '军训', '数分', '高代', '解几']].mean().round(2)

bar = (
    Bar(init_opts=opts.InitOpts(width='1000px', height='600px'))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="男生和女生各科平均成绩对比"),
        xaxis_opts=opts.AxisOpts(name="科目"),
        yaxis_opts=opts.AxisOpts(name="平均成绩"),
        legend_opts=opts.LegendOpts(type_='scroll', pos_top='10%', pos_bottom='10%'),
    )
)

bar.add_xaxis(male_scores.index.tolist())
bar.add_yaxis("男生", male_scores.tolist())
bar.add_yaxis("女生", female_scores.tolist())

bar.render('average.html')