from pyecharts import options as opts
from pyecharts.charts import Page, WordCloud
from pyecharts.globals import SymbolType

words = [
     ("原铁路总公司总经理盛光祖获刑15年", 40),
     ("年轻人报复性挤爆“3.5分饭店”", 38),
     ("“新三样”展现新优势", 36),
     ("周海媚小区保安证实救护车曾来救人", 34),
     ("女孩回应被男子问5000元玩不玩", 32),
     ("一碗30元的网红面馆卖不动了", 30),
     ("巴西球员比赛中被闪电击中身亡", 28),
     ("降雪能冻死病毒？", 26),
     ("大批空姐转行卖车？多方回应", 24),
     ("万达380亿元对赌危机解除", 22),
     ("男子杀害“高原神鸟”烹食被刑拘", 20),
     ("业主欠800万水费 倾家荡产不够交", 18),
     ("加拿大鹅9500元羽绒服被疑瑕疵品", 16),
     ("薛之谦上上谦火锅仅剩两家", 14),
     ("周杰伦成为Dior全球品牌大使", 12),
     ("河南市民骑电动车在雪地里艰难前行", 10),
     ("考生科目二考前跳起了“科目三”", 8),
     ("18岁中国留学生闯库里豪宅要签名", 6),
     ("破产姐妹Max演员结婚", 4),
     ("小米：余承东发布言论与事实不符", 2),
        ]
wordcloud = (WordCloud()
       .add("", words, word_size_range=[0, 20])# word_size_range为字体大小范围
       .set_global_opts(title_opts=opts.TitleOpts(title="WordCloud-基本示例")) )
wordcloud.render('wordcloud.html')
