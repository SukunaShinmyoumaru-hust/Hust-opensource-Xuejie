package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.index.impl.TermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleScanner;
import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.StringSplitter;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.List;
/**
 * <pre>
 *     TermTupleScanner是一个AbstractTermTupleStream流对象，
 *     它利用java.io.BufferedReader去读取文本文件得到一个个三元组TermTuple.
 *     其具体子类需要重新实现next方法获得文本文件里的三元组
 * </pre>
 */

public class TermTupleScanner extends AbstractTermTupleScanner {

    /**
    * count用于标记单词出现的位置
     */
    protected int count=0; //标记单词出现的位置

    /**
     * stringList用于存储一个文档中的所有单词，构成一个列表
     */
    protected List<String> stringList;//文档单词列表

    public TermTupleScanner(){} //缺省构造函数
    public TermTupleScanner(BufferedReader input) {
        super(input);
        //调用getStringList方法,得到输入流的单词列表
        this.getStringList();
    }
    /**
     * <pre>
     * 由输入流对象input得到相关文档的所有单词形成的列表
     * </pre>
     */
    protected void getStringList(){
        try{
            String string = null ;
            //读入文档
            StringBuffer buffer = new StringBuffer();
            //readLine方法读取文件末尾返回null，读取的每一行字符串会自动去除'\n'
            while((string = input.readLine())!=null){
                buffer.append(string).append('\n');
            }
            //分割文档单词，保存到stringList中
            StringSplitter stringSplitter = new StringSplitter();
            stringSplitter.setSplitRegex(Config.STRING_SPLITTER_REGEX);
            stringList = stringSplitter.splitByRegex(buffer.toString());
        }catch (IOException e){
            e.printStackTrace();
        }
    }
    /**
     * 获得下一个三元组
     * @return :下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        if(stringList != null) {
            if (count < stringList.size()) {
                AbstractTerm term = new Term(stringList.get(count));
                AbstractTermTuple tuple = new TermTuple(term, count);
                count++;
                return tuple;
            }else{
                return null;
            }
        }
        return null;
    }

}
