#include "clause.h"

//构造函数
Clause::Clause()
{
}
//析构函数
Clause::~Clause()
{
}
//添加文字
 void Clause::addLiteral(Literal l)
{
	literals.push_back(l);
}
 /***********************************************
*函数名称：removeLiteral
* 函数功能：删除子句中的特定文字
* 注释：新建一个数组，将不是特定文字的文字复制到新数组中，然后指向新数组。
* 返回值：void
************************************************/
 void Clause::removeLiteral(const Literal& l)
{
	vector<Literal> temp;//新建一个数组
	for (int i = 0; i < literals.size(); i++)//复制数据
	{
		if (literals[i].var != l.var)
		{
			temp.push_back(literals[i]);
		}
	}
	literals = temp;//指向新数组
	
}
 /***********************************************
 *函数名称：print
 * 函数功能：打印子句
 * 注释：依次打印子句中的文字。
 * 返回值：void
 ************************************************/
 void Clause::print()
{
	for (int i = 0; i < literals.size(); i++)
	{
		literals[i].print();
	}
	cout << endl;
}
 /***********************************************
 *函数名称：isUnit
 * 函数功能：判断是否是单子句
 * 注释：判断子句中文字的个数是否为1。
 * 返回值：bool类型，如果是单子句，则返回true，否则返回false。
 ************************************************/
 bool Clause::isUnit()const
{
	return literals.size() == 1;
}
