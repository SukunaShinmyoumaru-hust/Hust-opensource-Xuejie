package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.StopWords;

import java.util.Arrays;



public class LengthTermTupleFilter extends AbstractTermTupleFilter {
    /**
     * 构造函数
     * @param input：三元组流对象
     */
    public LengthTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
    }

    /**
     * 获得下一个三元组
     * 过滤掉长度大于20或者长度小于3的单词
     * @return :下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        AbstractTermTuple termTuple = input.next();
        //term单词长度小于3或者大于20
        //继续调用input.next()获得下一个三元组，直至不满足上面的情况，退出循环
        while(termTuple != null && (termTuple.term.getContent().length() < Config.TERM_FILTER_MINLENGTH
                || termTuple.term.getContent().length() > Config.TERM_FILTER_MAXLENGTH)){
            termTuple = input.next();
        }
        return termTuple;
    }
}
