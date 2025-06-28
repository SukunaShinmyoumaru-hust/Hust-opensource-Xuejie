from pyecharts import options as opts
from pyecharts.charts import  Page, Pie
import pandas as pd

world_cup_data = pd.read_csv('WorldCupsSummary.csv')

champions = world_cup_data['Winner'].value_counts()
runners_up = world_cup_data['Second'].value_counts()
third_places = world_cup_data['Third'].value_counts()
fourth_places = world_cup_data['Fourth'].value_counts()

pie1 = (
    Pie()
    .add(
        "冠军",
        [list(z) for z in zip(champions.index.tolist(), champions.values.tolist())],
        radius=["50%", "70%"]  # 设置圆环图的内外半径
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="冠军分布"),
        legend_opts=opts.LegendOpts(pos_left="right", orient="vertical")
    )
)

pie2 = (
    Pie()
    .add(
        "亚军",
        [list(z) for z in zip(runners_up.index.tolist(), runners_up.values.tolist())],
        radius=["50%", "70%"]
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="亚军分布"),
        legend_opts=opts.LegendOpts(pos_left="right", orient="vertical")
    )
)

pie3 = (
    Pie()
    .add(
        "季军",
        [list(z) for z in zip(third_places.index.tolist(), third_places.values.tolist())],
        radius=["50%", "70%"]
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="季军分布"),
        legend_opts=opts.LegendOpts(pos_left="right", orient="vertical")
    )
)

pie4 = (
    Pie()
    .add(
        "第4名",
        [list(z) for z in zip(fourth_places.index.tolist(), fourth_places.values.tolist())],
        radius=["50%", "70%"]
    )
    .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c}"))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="第4名分布"),
        legend_opts=opts.LegendOpts(pos_left="right", orient="vertical")
    )
)

page = Page(page_title='世界杯结果分布', layout=Page.SimplePageLayout)
page.add(pie1, pie2, pie3, pie4)

page.render('世界杯结果分布-环形图.html')