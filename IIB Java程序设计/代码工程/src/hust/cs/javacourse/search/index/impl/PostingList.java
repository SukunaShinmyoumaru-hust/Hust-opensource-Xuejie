package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractPostingList;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.*;

/**
 * <pre>
 *    PostingList对象包含了一个单词的Posting列表.
 *    实现接口：
 *        FileSerializable：可序列化到文件或从文件反序列化.
 * </pre>
 */

public class PostingList extends AbstractPostingList {
    /**
     * 添加Posting,要求不能有内容重复的posting
     * @param posting：Posting对象
     */
    @Override
    public void add(AbstractPosting posting) {
        //判断posting是否为Posting类的实例对象
        if(posting instanceof Posting){
            //判断posting是否已经存在与list中
            if(!this.list.contains(posting)){
                list.add(posting);
            }
        }
    }

    /**
     * 添加Posting列表,,要求不能有内容重复的posting
     * @param postings：Posting列表
     */
    @Override
    public void add(List<AbstractPosting> postings) {
        for(AbstractPosting posting:postings){
            this.add(posting);
        }
    }

    /**
     * 返回指定文档id的Posting对象的下标
     * @param docId ：文档id
     * @return ：如果找到返回对应下标；否则返回-1
     */
    @Override
    public int indexOf(int docId) {
        //遍历list，找到对应的posting，返回其下标
        for(AbstractPosting posting:list){
            if(posting.getDocId() == docId){
                return list.indexOf(posting);
            }
        }
        return -1;
    }

    /**
     * 返回指定下标位置的Posting
     * @param index ：下标
     * @return ：指定下标位置的Posting
     */
    @Override
    public AbstractPosting get(int index) {
        return list.get(index);
    }

    /**
     * 返回指定Posting对象的下标
     * @param posting：指定的Posting对象
     * @return ：如果找到返回对应下标；否则返回-1
     */
    @Override
    public int indexOf(AbstractPosting posting) {
        return list.indexOf(posting);
    }

    /**
     * 是否包含指定Posting对象
     * @param posting： 指定的Posting对象
     * @return : 如果包含返回true，否则返回false
     */
    @Override
    public boolean contains(AbstractPosting posting) {
        return list.contains(posting);
    }

    /**
     * 删除指定下标的Posting对象
     * @param index：指定的下标
     */
    @Override
    public void remove(int index) {
        list.remove(index);
    }

    /**
     * 删除指定的Posting对象
     * @param posting ：定的Posting对象
     */
    @Override
    public void remove(AbstractPosting posting) {
        list.remove(posting);
    }

    /**
     * 返回PostingList的大小，即包含的Posting的个数
     * @return ：PostingList的大小
     */
    @Override
    public int size() {
        return list.size();
    }

    /**
     * 清除PostingList
     */
    @Override
    public void clear() {
        list.clear();
    }

    /**
     * PostingList是否为空
     * @return 为空返回true;否则返回false
     */
    @Override
    public boolean isEmpty() {
        return list.isEmpty();
    }

    /**
     * 根据文档id的大小对PostingList进行从小到大的排序
     */
    @Override
    public void sort() {
        //Comparable接口已经在Posting类中实现
        //直接调用Collections中的sort方法即可
        //因为sort方法会调用Posting中已经实现好的compareTo方法进行排序
        Collections.sort(list);
    }

    /**
     * 获得PosingList的字符串表示
     * @return ： PosingList的字符串表示
     */
    @Override
    public String toString() {
        StringBuffer buffer = new StringBuffer();
        for(AbstractPosting posting:list){
            buffer.append(posting.toString()).append("->");
        }
        if(buffer.length() > 2) {
            buffer.deleteCharAt(buffer.length() - 1);
            buffer.deleteCharAt(buffer.length() - 1);
        }
        buffer.append("\n");
        return buffer.toString();
    }

    /**
     * 写到二进制文件
     * @param out :输出流对象
     */
    @Override
    public void writeObject(ObjectOutputStream out) {
        try{
            //由于List没有实现序列化接口，数组是可以序列化的
            //故需要与数组进行相互转换，实现List的序列化和反序列化
            //将list转换成数组
            AbstractPosting[] postings = new AbstractPosting[list.size()];
            list.toArray(postings);
            //进行序列化存储
            out.writeObject(postings);

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
            //进行反序列化读取
            AbstractPosting[] postings = (AbstractPosting[])in.readObject();
            //将数组转换为List
            this.list = Arrays.asList(postings);
        }catch (IOException  | ClassNotFoundException e){  //推荐的捕获异常catch块的写法
            e.printStackTrace();
        }
    }
}
