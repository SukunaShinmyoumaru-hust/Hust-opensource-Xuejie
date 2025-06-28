from pyecharts.charts import Line
import pandas as pd
from pyecharts import options as opts

world_cup_data = pd.read_csv('WorldCupsSummary.csv')

line = (
    Line()
    .add_xaxis(world_cup_data['Year'].astype("str").tolist())
    .add_yaxis("观众人数", world_cup_data['Attendance'].tolist(), areastyle_opts=opts.AreaStyleOpts(opacity=0.5))
    .extend_axis(
        yaxis=opts.AxisOpts(
            name="比赛场次",
            type_="value"
        )
    )
    .add_yaxis("比赛场次", world_cup_data['MatchesPlayed'].tolist(), yaxis_index=1, areastyle_opts=opts.AreaStyleOpts(opacity=0.5))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="观众人数和比赛场次随时间变化趋势面积图"),
        yaxis_opts=opts.AxisOpts(
            type_="value",
            name="观众人数"
        ),
        tooltip_opts=opts.TooltipOpts(trigger="axis", axis_pointer_type="cross"),
        datazoom_opts=[opts.DataZoomOpts(type_="slider", range_start=0, range_end=100)]
    )
)

line.render('观众人数和比赛场次随时间变化趋势面积图.html')