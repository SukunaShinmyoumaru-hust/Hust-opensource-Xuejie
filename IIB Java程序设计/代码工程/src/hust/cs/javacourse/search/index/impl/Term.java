package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractTerm;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Locale;

/**
 * <pre>
 *   Term对象表示文本文档里的一个单词.
 *   实现接口：
 *       Comparable：可比较大小（字典序）,为了加速检索过程，字典需要将单词进行排序.
 *       FileSerializable：可序列化到文件或从文件反序列化.
 *   </pre>
 */

public class Term extends AbstractTerm {
    /**
     * 缺省构造函数
     */
    protected String content;

    public Term(){}

    /**
     * 构造函数
     * @param content ：Term内容
     */
    public Term(String content){
        this.content = content;
    }

    /**
     * 比较二个Term大小（按字典序）
     * @param o： 要比较的Term对象
     * @return ： 返回二个Term对象的字典序差值
     */
    //实现Comparable接口
    @Override
    public int compareTo(AbstractTerm o) {
        if(o instanceof Term) {
            //忽略大小写进行比较
            return this.content.toLowerCase().compareTo(o.getContent().toLowerCase());
        }
        return 0;   //暂时不知解决方法
    }

    /**
     * 因为要作为HashMap里面的key，因此必须要覆盖hashCode方法
     * 返回对象的HashCode
     * @return ：对象的HashCode
     */
    //覆盖HashCode方法
    @Override
    public int hashCode() {
        //返回类Term中属性字符串（忽略大小写）content的哈希码值
        return this.content.toLowerCase().hashCode();
    }

    /**
     * 返回Term内容
     * @return Term内容
     */
    @Override
    public String getContent() {
        return this.content;
    }

    /**
     * 设置Term内容
     * @param content：Term的内容
     */
    @Override
    public void setContent(String content) {
        this.content = content;
    }

    /**
     * 返回Term的字符串表示
     * @return 字符串
     */
    @Override
    public String toString() {
        return this.content ;
    }

    /**
     * 判断二个Term内容是否相同
     * @param obj ：要比较的另外一个Term
     * @return 如果内容相等返回true，否则返回false
     */
    @Override
    public boolean equals(Object obj) {
        //忽略大小写进行term单词的比较
        if (obj instanceof Term) {
            Term t = (Term) obj;
            return this.content.equalsIgnoreCase(t.content);
        }
        return false;
    }

    /**
     * 写到二进制文件
     * @param out :输出流对象
     */
    @Override
    public void writeObject(ObjectOutputStream out) {
        try{
            //将this对象成员序列化
            out.writeObject(this.content);
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    /**
     * 从二进制文件读
     * @param in ：输入流对象
     */
    @Override
    public void readObject(ObjectInputStream in) {
        try{
            //将this对象成员反序列化
            this.content = (String) (in.readObject());
        }catch (IOException | ClassNotFoundException e){
            e.printStackTrace();
        }
    }
}
