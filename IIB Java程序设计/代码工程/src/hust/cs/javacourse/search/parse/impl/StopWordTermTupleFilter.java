package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.StopWords;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class StopWordTermTupleFilter extends AbstractTermTupleFilter {
    /**
     * stopWords用于存储所有的停用词
     */
    List<String> stopWords;

    /**
     * 构造函数
     * @param input：三元组流对象
     */
    public StopWordTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
        stopWords = new ArrayList<>(Arrays.asList(StopWords.STOP_WORDS));
    }

    /**
     * 获得下一个三元组
     * 过滤掉停用词
     * @return :下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next(){
        AbstractTermTuple termTuple = input.next();
        //遇到停用词，继续调用input.next()获得下一个三元组，直至不满足上面的情况，退出循环
        while(termTuple!=null && stopWords.contains(termTuple.term.getContent().toLowerCase())){
            termTuple = input.next();
        }
        return termTuple;
    }
}
