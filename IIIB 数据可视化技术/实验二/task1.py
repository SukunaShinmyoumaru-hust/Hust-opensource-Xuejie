import json
from pyecharts import options as opts
from pyecharts.charts import Graph

with open(r"weibo.json", "r", encoding="utf-8") as f:
          j = json.load(f)
          nodes, links, categories, cont, mid, userl = j

graph= (Graph()
         .add("", nodes, links, categories, repulsion=50,
             linestyle_opts=opts.LineStyleOpts(curve=0.2),
             label_opts=opts.LabelOpts(is_show=False) )
         .set_global_opts(legend_opts=opts.LegendOpts(is_show=False),
             title_opts=opts.TitleOpts(title="Graph-微博转发关系图")))
graph.render('微博转发关系图.html')
