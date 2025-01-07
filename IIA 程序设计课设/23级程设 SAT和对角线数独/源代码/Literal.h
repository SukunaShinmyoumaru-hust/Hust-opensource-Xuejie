#pragma once
#include"definition.h"
class Literal
{
public:
	//布尔变量
	int var;
	Literal();
	//构造函数
	Literal(int var);
	//析构函数
	~Literal();
	//是否是负文字
	bool isNegative();
	//返回绝对值
	int getAbs();
	//打印文字
	void print();
	//不等号重载
	bool operator!=(const Literal& l)const;
	//等号重载
	bool operator==(const Literal& l)const;
};

