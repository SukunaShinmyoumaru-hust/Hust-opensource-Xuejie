#pragma once
#include"clause.h"
class CNF
{
public:
	//子句集合
	vector<Clause> clauses;
	//构造函数
	CNF();
	//添加子句
	void addClause(Clause c);
	//移除所有包含单子句L的子句
	void removeClause(const Clause& clause);
	//移除剩下所有子句中的~L
	void removeLiteral(const Literal& L);
	bool isEmpty() const {
		return clauses.empty();
	}
	bool hasNext() const {
		// 如果列表中至少有一个子句，且不是空子句，则返回true
		return !clauses.empty() && !clauses[0].literals.empty();
	}
	void print();
};

