import pandas as pd
from pyecharts import options as opts
from pyecharts.charts import WordCloud

data = pd.read_csv('WorldCupPlayers.csv')

player_counts = data['Player Name'].value_counts()

wordcloud = (
    WordCloud()
    .add(series_name="", data_pair=player_counts.items())
    .set_global_opts(
        title_opts=opts.TitleOpts(title="Player Name词云图"),
        tooltip_opts=opts.TooltipOpts(trigger="item"),
    )
)

wordcloud.render("Player Name词云图.html")