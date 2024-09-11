package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.*;
import hust.cs.javacourse.search.query.AbstractHit;

import java.io.*;
import java.util.*;

/**
 * AbstractIndex的具体实现类
 * 一个倒排索引对象包含了一个文档集合的倒排索引.
 *      内存中的倒排索引结构为TreeMap，key为Term对象，value为对应的PostingList对象.
 *      另外在AbstractIndex里还定义了从docId和docPath之间的映射关系.
 *      实现接口：
 *         FileSerializable：可序列化到文件或从文件反序列化.
 */
public class Index extends AbstractIndex {


    /**
     * 添加文档到索引，更新索引内部的映射表
     *
     * @param document ：文档的AbstractDocument子类型表示
     */
    @Override
    public void addDocument(AbstractDocument document) {
        //如果文档键值对中没有此文档，则加入文档键值对,更新索引
        if(!docIdToDocPathMapping.containsKey(document.getDocId())){
            docIdToDocPathMapping.put(document.getDocId(),document.getDocPath());
            //遍历文档的三元组列表，构建索引表
            for(AbstractTermTuple termTuple:document.getTuples()){
                //获取单词
                AbstractTerm term = termTuple.term;
                //查找是否单词已经存在
                if(termToPostingListMapping.containsKey(term)){
                    //获得该term的PostingList
                    AbstractPostingList postingList = termToPostingListMapping.get(term);
                    //获得此文档的Posting的位置
                    int indexOfPosting = postingList.indexOf(document.getDocId());
                    //如果有该文档的Posting
                    if(indexOfPosting!=-1){
                        //如果存在，则更新此Posting中的数据域
                        AbstractPosting posting = postingList.get(indexOfPosting);
                        int freq = posting.getFreq() + 1 ; //获得之前的freq并加1
                        posting.setFreq(freq);  //重置频率
                        posting.getPositions().add(termTuple.curPos);//加入当前位置
                    }else{
                        //如果不存在，创建新的Posting对象
                        AbstractPosting posting = new Posting();
                        posting.setDocId(document.getDocId());
                        posting.setFreq(1);
                        posting.getPositions().add(termTuple.curPos);
                        //将新的Posting对象加入到PostingList末尾
                        postingList.add(posting);
                    }
                }else{
                    //如果单词不存在，构建新的PostingList
                    AbstractPostingList postingList = new PostingList();
                    AbstractPosting posting = new Posting();
                    posting.setDocId(document.getDocId());
                    posting.setFreq(1);
                    posting.getPositions().add(termTuple.curPos);
                    postingList.add(posting);
                    //将term和postingList加入到Map中
                    termToPostingListMapping.put(term,postingList);
                }
            }
        }

    }

    /**
     * <pre>
     * 从索引文件里加载已经构建好的索引.内部调用FileSerializable接口方法readObject即可
     * @param file ：索引文件
     * </pre>
     */
    @Override
    public void load(File file) {
        try{
            ObjectInputStream in = new ObjectInputStream(new FileInputStream(file));
            readObject(in);
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    /**
     * <pre>
     * 将在内存里构建好的索引写入到文件. 内部调用FileSerializable接口方法writeObject即可
     * @param file ：写入的目标索引文件
     * </pre>
     */
    @Override
    public void save(File file) {
        try{
            ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(file));
            writeObject(out);
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    /**
     * 返回指定单词的PostingList
     *
     * @param term : 指定的单词
     * @return ：指定单词的PostingList;如果索引字典没有该单词，则返回null
     */
    @Override
    public AbstractPostingList search(AbstractTerm term) {
            return termToPostingListMapping.get(term);
    }

    /**
     * 返回索引的字典.字典为索引里所有单词的并集
     *
     * @return ：索引中Term列表
     */
    @Override
    public Set<AbstractTerm> getDictionary() {
        //利用TreeSet的排序功能
        return new TreeSet<>(termToPostingListMapping.keySet());
    }

    /**
     * <pre>
     * 对索引进行优化，包括：
     *      对索引里每个单词的PostingList按docId从小到大排序
     *      同时对每个Posting里的positions从小到大排序
     * 在内存中把索引构建完后执行该方法
     * </pre>
     */
    @Override
    public void optimize() {
        for(AbstractPostingList postingList:termToPostingListMapping.values()){
            //对postingList进行排序
            postingList.sort();
            //遍历postingList，对其中的每一个posting的positions进行排序
            for(int i = 0; i<postingList.size();i++ )
                postingList.get(i).sort();
        }
    }

    /**
     * 根据docId获得对应文档的完全路径名
     *
     * @param docId ：文档id
     * @return : 对应文档的完全路径名
     */
    @Override
    public String getDocName(int docId) {
        return docIdToDocPathMapping.get(docId);
    }

    /**
     * 写到二进制文件
     *
     * @param out :输出流对象
     */
    @Override
    public void writeObject(ObjectOutputStream out) {
        try{
            //TreeMap以及实现了序列化接口，可以直接序列化
            out.writeObject(this.termToPostingListMapping);
            out.writeObject(this.docIdToDocPathMapping);
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    /**
     * 从二进制文件读
     *
     * @param in ：输入流对象
     */
    @Override
    public void readObject(ObjectInputStream in) {
        try{
            this.termToPostingListMapping = (Map<AbstractTerm, AbstractPostingList>) in.readObject();
            this.docIdToDocPathMapping = (Map<Integer, String>) in.readObject();
        }catch (IOException | ClassNotFoundException e){
            e.printStackTrace();
        }
    }
    /**
     * 返回索引的字符串表示
     *
     * @return 索引的字符串表示
     */
    @Override
    public String toString() {
        StringBuffer buffer = new StringBuffer();
        buffer.append("-----------------------------------------------------------------------\ndictionary:\n");
        Set<AbstractTerm> terms = new TreeSet<>(getDictionary());
        for(AbstractTerm term:terms){
            buffer.append(term.toString()+' ');
        }
        buffer.append("\n-----------------------------------------------------------------------\ndocId----->docPath Mapping\n");
        for(Map.Entry<Integer,String> entry:docIdToDocPathMapping.entrySet()){
            buffer.append(entry.getKey()).append("\t----->\t");
            buffer.append(entry.getValue()).append('\n');
        }
        buffer.append("\n-----------------------------------------------------------------------\nterm---->postingList Mapping\n");
        for(Map.Entry<AbstractTerm,AbstractPostingList> entry:termToPostingListMapping.entrySet()){
            buffer.append(entry.getKey()).append("\t----->\t");
            buffer.append(entry.getValue());
        }
        return buffer.toString();
    }

}
