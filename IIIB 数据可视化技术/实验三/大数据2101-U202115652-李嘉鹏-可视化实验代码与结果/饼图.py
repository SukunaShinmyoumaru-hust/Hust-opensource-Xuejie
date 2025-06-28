from pyecharts import options as opts
from pyecharts.charts import Grid, Pie
import pandas as pd

world_cup_data = pd.read_csv('WorldCupsSummary.csv')

champions = world_cup_data['Winner'].value_counts()
runners_up = world_cup_data['Second'].value_counts()
third_places = world_cup_data['Third'].value_counts()
fourth_places = world_cup_data['Fourth'].value_counts()

data_champions = list(zip(champions.index.tolist(), champions.values.tolist()))
data_runners_up = list(zip(runners_up.index.tolist(), runners_up.values.tolist()))
data_third_places = list(zip(third_places.index.tolist(), third_places.values.tolist()))
data_fourth_places = list(zip(fourth_places.index.tolist(), fourth_places.values.tolist()))

pie1 = (
    Pie()
    .add(
        "冠军",
        data_champions,
        radius=["50%", "70%"],
        rosetype="radius"
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    # .set_global_opts(title_opts=opts.TitleOpts(title="世界杯冠军、亚军、季军、第4名国家分布图"))
)

pie2 = (
    Pie()
    .add(
        "亚军",
        data_runners_up,
        radius=["40%", "60%"],
        rosetype="radius"
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(legend_opts=opts.LegendOpts(is_show=False))
)

pie3 = (
    Pie()
    .add(
        "季军",
        data_third_places,
        radius=["30%", "50%"],
        rosetype="radius"
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(legend_opts=opts.LegendOpts(is_show=False))
)

pie4 = (
    Pie()
    .add(
        "第4名",
        data_fourth_places,
        radius=["20%", "40%"],
        rosetype="radius"
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(legend_opts=opts.LegendOpts(is_show=False))
)

grid = (
    Grid()
    .add(pie1, grid_opts=opts.GridOpts(pos_left="10%", pos_right="35%"), is_control_axis_index=True)
    .add(pie2, grid_opts=opts.GridOpts(pos_left="30%", pos_right="15%"), is_control_axis_index=True)
    .add(pie3, grid_opts=opts.GridOpts(pos_left="50%", pos_right="15%"), is_control_axis_index=True)
    .add(pie4, grid_opts=opts.GridOpts(pos_left="70%", pos_right="0%"), is_control_axis_index=True)
)
grid.render("世界杯结果分布-多层级饼图.html")