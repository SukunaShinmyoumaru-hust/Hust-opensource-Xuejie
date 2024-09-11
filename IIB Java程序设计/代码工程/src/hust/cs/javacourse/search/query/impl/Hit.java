package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;

import java.util.Iterator;
import java.util.Map;

/**
 * <pre>
 *  一个Hit表示一个命中文档.
 *  实现接口Comparable: 实现该接口是因为需要必须比较大小，用于命中结果的排序.
 * </pre>
 */

public class Hit extends AbstractHit {
    /**
     * 缺省构造函数
     */
    public Hit(){}
    /**
     * 构造函数
     * @param docId ：文档Id
     * @param docPath ：文档绝对路径
     */
    public Hit(int docId, String docPath) {
        super(docId, docPath);
    }

    /**
     * 构造函数
     * @param docId              ：文档id
     * @param docPath            ：文档绝对路径
     * @param termPostingMapping ：命中的三元组列表
     */
    public Hit(int docId, String docPath, Map<AbstractTerm, AbstractPosting> termPostingMapping) {
        super(docId, docPath, termPostingMapping);
    }

    /**
     * 获得文档id
     * @return ： 文档id
     */
    @Override
    public int getDocId() {
        return this.docId;
    }

    /**
     * 获得文档绝对路径
     * @return ：文档绝对路径
     */
    @Override
    public String getDocPath() {
        return this.docPath;
    }

    /**
     * 获得文档得分
     * @return ： 文档得分
     */
    @Override
    public double getScore() {
        return this.score;
    }

    /**
     * 获得文档内容
     * @return ： 文档内容
     */
    @Override
    public String getContent() {
        return  this.content;
    }

    /**
     * 获得命中的单词和对应的Posting键值对
     * @return ：命中的单词和对应的Posting键值对
     */
    @Override
    public Map<AbstractTerm, AbstractPosting> getTermPostingMapping() {
        return this.termPostingMapping;
    }

    /**
     * 设置文档得分
     * @param score ：文档得分
     */
    @Override
    public void setScore(double score) {
        this.score = score;
    }

    /**
     * 设置文档内容
     * @param content ：文档内容
     */
    @Override
    public void setContent(String content) {
        this.content = content;
    }

    /**
     * 获得命中结果的字符串表示, 用于显示搜索结果.
     * @return : 命中结果的字符串表示
     */
    @Override
    public String toString() {
        StringBuffer buffer = new StringBuffer("\n-----------------------------------------------------------------------\n");
        buffer.append("\ndocId: " + docId + "\ndocPath: " + docPath + "\ndocScore: " + score +"\ntermPostingMapping:\n");
        for(Map.Entry<AbstractTerm,AbstractPosting>entry:termPostingMapping.entrySet()){
            buffer.append(entry.getKey() + " ------> " + entry.getValue() + '\n');
        }
        buffer.append("content: \n" + content + '\n');
        return buffer.toString();
    }

    /**
     * 比较二个命中结果的大小，根据score比较
     * @param o     ：要比较的命中结果
     * @return      ：二个命中结果得分的差值
     */
    @Override
    public int compareTo(AbstractHit o) {
        //得分从大到小排序
       return (int)(this.score - o.getScore());
    }

    /**
     * 根据文档Id判断两个命中结果是否位同一文档
     * @param obj    ：要判断是否相等的命中结果
     * @return      ：文档Id相同返回true，否则返回false
     */
    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Hit){
            return this.docId == ((Hit)obj).docId;
        }
        return false;
    }
}
