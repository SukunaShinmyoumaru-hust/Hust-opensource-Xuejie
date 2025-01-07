#include "CNF.h"
//构造函数
CNF::CNF()
{
}
//添加子句
 void CNF::addClause(Clause c)
{
	clauses.push_back(c);
	/*调试//cout << "addClause" << endl;
	this->print();*/
}
 /***********************************************
 *函数名称：removeClause
 * 函数功能：移除所有包含单子句L的子句
 * 注释：传入单子句c，新建一个数组，将不包含c的子句复制到新数组中，然后指向新数组。
 * 返回值：void
 ************************************************/
 void CNF::removeClause(const Clause& c)
{
	vector<Clause> temp;//新建一个数组
	for (int i = 0; i < clauses.size(); i++)//复制数据
	{
		int j;
		for ( j = 0; j < clauses[i].literals.size(); j++)
		{
			if (clauses[i].literals[j].var == c.literals[0].var)//如果包含
			{
				break;
			}
		}
		if (j == clauses[i].literals.size())//没有包含
		{
			temp.push_back(clauses[i]);//添加到temp
		}
	}
	clauses = temp;//指向新数组
}

 /***********************************************
*函数名称：removeLiteral
* 函数功能：移除剩下所有子句中的~L文字
* 注释：传入文字L，遍历所有子句，删除子句中的L文字。
* 返回值：void
************************************************/
void CNF::removeLiteral(const Literal& L)
{
	for (int i = 0; i < clauses.size(); i++)
	{
		clauses[i].removeLiteral(L);
	}
}

/***********************************************
*函数名称：print
* 函数功能：打印CNF
* 注释：依次打印CNF中的子句。
* 返回值：void
************************************************/
void CNF::print()
{
	for (int i = 0; i < clauses.size(); i++)
	{
		clauses[i].print();
	}
}