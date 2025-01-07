#include "Literal.h"
//文字类
//默认构造函数
Literal::Literal()
{
	this->var = 0;
}
//有参构造函数
Literal::Literal(int var)
{
	this->var = var;
}
//析构函数
Literal::~Literal()
{
}	
//是否是负文字
bool Literal::isNegative()
{
	return var < 0;
}
//返回绝对值
int Literal::getAbs()
{
	return abs(var);
}
//打印文字
void Literal::print()
{
	cout << var << " ";
}
//不等号重载
bool Literal::operator!=(const Literal& l)const
{
	return this->var != l.var;
}
//等号重载
bool Literal::operator==(const Literal& l)const
{
	return this->var == l.var;
}

