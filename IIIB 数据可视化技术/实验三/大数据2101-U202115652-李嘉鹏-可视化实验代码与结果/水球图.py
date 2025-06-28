import pandas as pd
import numpy as np
from pyecharts import options as opts
from pyecharts.charts import Grid, Liquid, Page

df = pd.read_csv('WorldCupMatches.csv')

team_goals = pd.concat([df.groupby('Home Team Name')['Home Team Goals'].sum(), df.groupby('Away Team Name')['Away Team Goals'].sum()])
total_goals = team_goals.groupby(team_goals.index).sum()

top_9_teams = total_goals.sort_values(ascending=False)[:9]
total = total_goals.sum()
percentages = top_9_teams / total

liquids = []
for team, percentage in zip(top_9_teams.index, percentages):
    liquid = (
        Liquid()
        .add(
            "",
            [np.round(float(percentage) * 100, 2)],
            label_opts=opts.LabelOpts(formatter="{c}%"),
        )
        .set_global_opts(
            title_opts=opts.TitleOpts(
                title="{} 进球数在世界杯进球总数的占比".format(team)
            )
        )
    )
    liquids.append(liquid)

page = Page(page_title="进球数在世界杯进球总数的占比", layout=Page.SimplePageLayout)
for liquid in liquids:
    page.add(liquid)

page.render("liquid_chart.html")