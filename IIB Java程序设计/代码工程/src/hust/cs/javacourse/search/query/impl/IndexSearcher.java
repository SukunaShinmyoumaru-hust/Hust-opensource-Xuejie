package hust.cs.javacourse.search.query.impl;


import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractPostingList;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.impl.Index;

import hust.cs.javacourse.search.index.impl.Posting;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.AbstractIndexSearcher;
import hust.cs.javacourse.search.query.Sort;
import hust.cs.javacourse.search.util.Config;

import java.io.File;
import java.util.ArrayList;

import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class IndexSearcher extends AbstractIndexSearcher {
    @Override
    public void open(String indexFile) {
        this.index = new Index();
        index.load(new File(indexFile));
    }
    /**
     * 根据单个检索词进行搜索
     * @param queryTerm ：检索词
     * @param sorter ：排序器
     * @return ：命中结果数组
     */
    @Override
    public AbstractHit[] search(AbstractTerm queryTerm, Sort sorter) {
        // 忽略大小写
        if (Config.IGNORE_CASE) {
            queryTerm.setContent(queryTerm.getContent().toLowerCase());
        }
        //首先先找到index里面这个元素对应的内容的postinglist在哪里
        AbstractPostingList indexSearchResult = index.search(queryTerm);
        if (indexSearchResult == null) {
            return new Hit[0];
        }
        //如果有的话就返回一个Hit的List
        List<AbstractHit> result = new ArrayList<>();
        //对于postinglist里面的元素进行处理
        for (int i = 0; i < indexSearchResult.size(); i++) {
            //每一个posting进行遍历
            AbstractPosting posting = indexSearchResult.get(i);
            //生成新的hit变量
            AbstractHit hit = new Hit(posting.getDocId(), index.getDocName(posting.getDocId()));
            //建立对于这个三元组和posting之间的关系
            hit.getTermPostingMapping().put(queryTerm, posting);
            //计算分数
            hit.setScore(sorter.score(hit));
            result.add(hit);
        }
        //排序
        sorter.sort(result);
        AbstractHit[] returnResult = new AbstractHit[result.size()];
        return result.toArray(returnResult);
    }

    @Override
    public AbstractHit[] search(AbstractTerm queryTerm1, AbstractTerm queryTerm2, Sort sorter, LogicalCombination combine) {
        AbstractPostingList indexSearchResult1 = index.search(queryTerm1);
        AbstractPostingList indexSearchResult2 = index.search(queryTerm2);
        // 如果两个都没找到直接就是空的数组
        if (indexSearchResult1 == null && indexSearchResult2 == null) {
            return new Hit[0];
        }
        List<AbstractHit> result = new ArrayList<>();
        if (combine == LogicalCombination.AND) {
            // 如果有一个词语根本就不存在，那就直接返回空的数组
            if (indexSearchResult1 == null || indexSearchResult2 == null) {
                return new Hit[0];
            }
            // 求交集
            for (int i = 0; i < indexSearchResult1.size(); i++) {
                // 获取docId
                int docId = indexSearchResult1.get(i).getDocId();
                int sub_index = indexSearchResult2.indexOf(docId);
                if (sub_index != -1) {
                    AbstractHit hit = new Hit(docId, index.getDocName(docId));
                    hit.getTermPostingMapping().put(queryTerm1, indexSearchResult1.get(i));
                    hit.getTermPostingMapping().put(queryTerm2, indexSearchResult2.get(sub_index));
                    hit.setScore(sorter.score(hit));
                    result.add(hit);
                }
            }
        } else if (combine == LogicalCombination.OR) {
            // 如果有一个词语不存在直接退化为对另外一个词语的搜索
            if (indexSearchResult1 == null) {
                return search(queryTerm2, sorter);
            }
            if (indexSearchResult2 == null) {
                return search(queryTerm1, sorter);
            }

            for (int i = 0; i < indexSearchResult1.size(); i++) {
                // 首先添加
                int docId = indexSearchResult1.get(i).getDocId();
                int sub_index = indexSearchResult2.indexOf(docId);
                if (sub_index == -1) {
                    // 如果在另外一个词语中没有,那就正常添加
                    AbstractHit hit = new Hit(docId, index.getDocName(docId));
                    hit.getTermPostingMapping().put(queryTerm1, indexSearchResult1.get(i));
                    hit.setScore(sorter.score(hit));
                    result.add(hit);
                } else {
                    // 如果在另外一个中有, 那就要做一些修改
                    AbstractHit hit = new Hit(docId, index.getDocName(docId));
                    hit.getTermPostingMapping().put(queryTerm1, indexSearchResult1.get(i));
                    hit.getTermPostingMapping().put(queryTerm2, indexSearchResult2.get(sub_index));
                    // 这个地方其实有点奇怪
                    hit.setScore(sorter.score(hit));
                    result.add(hit);
                }
            }
            for (int i = 0; i < indexSearchResult2.size(); i++) {
                int docId = indexSearchResult2.get(i).getDocId();
                int sub_index = indexSearchResult1.indexOf(docId);
                if (sub_index == -1) {
                    // 只有当1中不存在的时候才添加
                    AbstractHit hit = new Hit(docId, index.getDocName(docId));
                    hit.getTermPostingMapping().put(queryTerm2, indexSearchResult2.get(i));
                    hit.setScore(sorter.score(hit));
                    result.add(hit);
                }
            }
        }

        sorter.sort(result);
        AbstractHit[] returnResult = new AbstractHit[result.size()];
        return result.toArray(returnResult);
    }
    /**
     * 查询两个在文中相邻出现的单词（进阶功能）
     * @param queryTerm1 ：第一个单词
     * @param queryTerm2 ：第二个单词
     * @param sorter ：排序器
     * @return ：查询结果数组
     */
    public AbstractHit[] search(AbstractTerm queryTerm1, AbstractTerm queryTerm2, Sort sorter) {
        AbstractPostingList postList1 = index.search(queryTerm1);
        AbstractPostingList postList2 = index.search(queryTerm2);
        if(postList1 == null || postList2 == null) return null;
        List<AbstractHit> hitArray = new ArrayList<AbstractHit>();
        int i=0, j=0;
        while(i < postList1.size() && j < postList2.size()) {
            AbstractPosting post1 = postList1.get(i);
            AbstractPosting post2 = postList2.get(j);
            // 这里默认索引中的数据都是按文档ID从小到大排序了
            if (post1.getDocId() == post2.getDocId()) {
                List<Integer> pos1 = post1.getPositions();
                List<Integer> pos2 = post2.getPositions();
                int a = 0, b = 0;
                List<Integer> positions = new ArrayList<Integer>();     // 存放连续两个单词出现的位置
                while(a < pos1.size() && b < pos2.size()){
                    int p1 = pos1.get(a);
                    int p2 = pos2.get(b);
                    if(p1 == p2-1){
                        positions.add(p1);
                        a++;    b++;
                    } else if(p1 < p2-1){
                        a++;
                    } else {
                        b++;
                    }
                }
                if(positions.size() > 0) {      // 否则会出现score = 0.0的情况
                    String path = index.getDocName(post1.getDocId());
                    Map<AbstractTerm, AbstractPosting> mp =
                            new HashMap<AbstractTerm, AbstractPosting>();
                    // 把两个单词合并放入映射表中，中间用一个空格隔开，位置则使用两个单词中第一个单词所在的位置
                    // 这样在高亮显示的时候就可以了（要注意把空格替换成\\s，以避免原文中相邻单词直接有多个空格的情况）
                    mp.put(new Term(queryTerm1.getContent() + " " + queryTerm2.getContent()),
                            new Posting(post1.getDocId(), positions.size(), positions));
                    AbstractHit h = new Hit(post1.getDocId(), path, mp);
                    h.setScore(sorter.score(h));        // 先设置分数
                    hitArray.add(h);
                }
                i++;    j++;
            } else if (post1.getDocId() < post2.getDocId()) {
                i++;
            } else {        // post1 > post2
                j++;
            }
        }
        if(hitArray.size() < 1) return null;
        new SimpleSorter().sort(hitArray);
        return (AbstractHit[]) hitArray.toArray(new Hit[0]);
    }
}