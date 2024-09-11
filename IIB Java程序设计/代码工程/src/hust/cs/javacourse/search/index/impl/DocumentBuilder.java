package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractDocument;
import hust.cs.javacourse.search.index.AbstractDocumentBuilder;
import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.parse.impl.*;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * <pre>
 * Document构造器
 *      Document构造器的功能应该是由解析文本文档得到的TermTupleStream，产生Document对象.
 * </pre>
 */
public class DocumentBuilder extends AbstractDocumentBuilder {

    /**
     * <pre>
     * 由解析文本文档得到的TermTupleStream,构造Document对象.
     * @param docId             : 文档id
     * @param docPath           : 文档绝对路径
     * @param termTupleStream   : 文档对应的TermTupleStream
     * @return ：Document对象
     * </pre>
     */
    @Override
    public AbstractDocument build(int docId, String docPath, AbstractTermTupleStream termTupleStream) {
        //构建Document类型对象
        AbstractDocument document = new Document(docId,docPath);
        AbstractTermTuple tuple = termTupleStream.next();
        while(tuple != null){
            //通过调用document中的add方法添加元素
            document.addTuple(tuple);
            tuple = termTupleStream.next();
        }
        return document;
    }

    /**
     * <pre>
     * 由给定的File,构造Document对象.
     * 该方法利用输入参数file构造出AbstractTermTupleStream子类对象后,内部调用
     *      AbstractDocument build(int docId, String docPath, AbstractTermTupleStream termTupleStream)
     * @param docId     : 文档id
     * @param docPath   : 文档绝对路径
     * @param file      : 文档对应File对象
     * @return          : Document对象
     * </pre>
     */
    @Override
    public AbstractDocument build(int docId, String docPath, File file) {
        AbstractTermTupleStream termTupleStream = null;
        try{
            //装饰者模式，使用三层过滤器进行包装，实现三种过滤功能
            termTupleStream = new LengthTermTupleFilter(new PatternTermTupleFilter(
                    new StopWordTermTupleFilter(new TermTupleScanner(new BufferedReader(
                            new InputStreamReader(new FileInputStream(file)))))));
            //最后调用子方法来
            return this.build(docId,docPath,termTupleStream);
        }catch (FileNotFoundException e){
            e.printStackTrace();
        }
        return null;
    }
}
