from pyecharts.charts import Bar
import pandas as pd
from pyecharts import options as opts

world_cup_data = pd.read_csv('WorldCupsSummary.csv')

bar = (
    Bar()
    .add_xaxis(world_cup_data['Year'].tolist())
    .add_yaxis("进球数", world_cup_data['GoalsScored'].tolist())
    .add_yaxis("参赛队伍数", world_cup_data['QualifiedTeams'].tolist())
    .set_global_opts(title_opts=opts.TitleOpts(title="不同年份进球数与参赛队伍数的柱状图"),
                     datazoom_opts=[opts.DataZoomOpts(type_="slider", range_start=0, range_end=100)])
)

bar.render('世界杯进球数和参赛队伍数柱状图.html')