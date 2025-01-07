#pragma once
#include"Literal.h"
#include"vector.hpp"
class Clause
{
public:
	//文字集合
	vector<Literal> literals;
	//构造函数
	Clause();
	//析构函数
	~Clause();
	//添加文字
	void addLiteral(Literal l);
	//删除所有特定文字
	void removeLiteral(const Literal& l);
	//打印子句
	void print();
	//判断是否是单子句
	bool isUnit()const;

};

