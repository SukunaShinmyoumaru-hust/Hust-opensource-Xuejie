from pyecharts.globals import ThemeType
import pandas as pd
from pyecharts.charts import Map
from pyecharts import options as opts

world_cup_data = pd.read_csv('WorldCupsSummary.csv')

host_count = world_cup_data['HostCountry'].value_counts().reset_index()
host_count.columns = ['HostCountry', 'Count']

map_ = (
    Map(init_opts=opts.InitOpts(theme=ThemeType.LIGHT))
    .add("世界杯举办次数", [list(z) for z in zip(host_count['HostCountry'], host_count['Count'])], "world")
    .set_global_opts(title_opts=opts.TitleOpts(title="世界杯举办次数Map"),
                     visualmap_opts=opts.VisualMapOpts(is_show=True,
                                                       min_=0, max_=3,
                                                       pieces=[{"min": 0, "max": 0, "label": "0次"},
                                                               {"min": 1, "max": 1, "label": "1次"},
                                                               {"min": 2, "max": 2, "label": "2次"},
                                                               {"min": 3, "max": 3, "label": "3次"},
                                                               ],
                                                       pos_left="5%", pos_bottom="5%"))
    .set_series_opts(label_opts=opts.LabelOpts(is_show=False))
)

map_.render('世界杯举办国家分布图-地图.html')