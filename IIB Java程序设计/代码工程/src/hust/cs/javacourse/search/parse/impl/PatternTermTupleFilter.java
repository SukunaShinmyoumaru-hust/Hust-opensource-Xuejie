package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.StopWords;

import java.util.Arrays;

public class PatternTermTupleFilter extends AbstractTermTupleFilter {
    /**
     * 构造函数
     * @param input：三元组流对象
     */
    public PatternTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
    }

    /**
     * 获得下一个三元组
     * 过滤掉非英文单词
     * @return :下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        AbstractTermTuple termTuple = input.next();
        //遇到非英文单词，继续调用input.next()获得下一个三元组，直至不满足上面的情况，退出循环
        while(termTuple!=null && !termTuple.term.getContent().matches(Config.TERM_FILTER_PATTERN)){
            termTuple = input.next();
        }
        return termTuple;
    }
}
