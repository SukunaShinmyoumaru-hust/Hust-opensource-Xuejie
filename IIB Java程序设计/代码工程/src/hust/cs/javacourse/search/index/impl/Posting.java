package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractPosting;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Posting extends AbstractPosting {
    /**
     * 缺省构造函数
     */
    public Posting(){}  //缺省构造函数
    /**
     * 构造函数
     * @param docId ：包含单词的文档id
     * @param freq  ：单词在文档里出现的次数
     * @param positions   ：单词在文档里出现的位置
     */
    public Posting(int docId, int freq, List<Integer> positions){
        super(docId,freq,positions);
    }

    /**
     * 返回包含单词的文档id
     * @return ：文档id
     */
    @Override
    public int getDocId() {
        return docId;
    }

    /**
     * 设置包含单词的文档id
     * @param docId：包含单词的文档id
     */
    @Override
    public void setDocId(int docId) {
        this.docId = docId;
    }

    /**
     * 返回单词在文档里出现的次数
     * @return ：出现次数
     */
    @Override
    public int getFreq() {
        return freq;
    }

    /**
     * 设置单词在文档里出现的次数
     * @param freq:单词在文档里出现的次数
     */
    @Override
    public void setFreq(int freq) {
        this.freq = freq;
    }

    /**
     * 返回单词在文档里出现的位置列表
     * @return ：位置列表
     */
    @Override
    public List<Integer> getPositions() {
        return positions;
    }

    /**
     * 设置单词在文档里出现的位置列表
     * @param positions：单词在文档里出现的位置列表
     */
    @Override
    public void setPositions(List<Integer> positions) {
        this.positions = positions;
    }


    /**
     * 比较二个Posting对象的大小（根据docId）
     * @param o： 另一个Posting对象
     * @return ：二个Posting对象的docId的差值
     */
    @Override
    public int compareTo(AbstractPosting o) {
        if(o instanceof Posting){
            return this.docId-o.getDocId();
        }
        return 0;
    }

    /**
     * 判断二个Posting内容是否相同
     * @param obj ：要比较的另外一个Posting
     * @return 如果内容相等返回true，否则返回false
     */
    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Posting){
            Posting p = (Posting) obj;
            //分别对两个Posting对象中的positions进行排序,防止因序列顺序不同导致两个线性表不相等
            this.sort();
            p.sort();
            return this.docId == p.docId && this.freq == p.freq && this.positions.equals(p.positions);
        }
        return false;
    }

    /**
     * 对内部positions从小到大排序
     */
    @Override
    public void sort() {
        //调用Collections类中的sort方法排序
        Collections.sort(positions);
    }

    /**
     * 返回Posting的字符串表示
     * @return 字符串
     */
    @Override
    public String toString() {
        return "{docId: " + this.docId + "  freq: " + this.freq + "   positions: " + this.positions + "}";
    }

    @Override
    public void writeObject(ObjectOutputStream out) {
        try{
            //将this对象的成员依次序列化
            out.writeObject(this.docId);
            out.writeObject(this.freq);
            //将list转换成数组
            Integer[] integers = new Integer[positions.size()];
            positions.toArray(integers);
            //进行序列化存储
            out.writeObject(integers);
        }catch(IOException e){
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
            //将this对象的成员依次反序列化
            this.docId = (int)(in.readObject());
            this.freq = (int)(in.readObject());
            //进行反序列化读取
            Integer[] integers = (Integer[]) in.readObject();
            //将数组转化为List
            this.positions = new ArrayList<>(Arrays.asList(integers));

        }catch(IOException | ClassNotFoundException e){  //推荐的catch语句块的写法
            e.printStackTrace();
        }
    }
}
