#pragma once
#include"CNF.h"
class CNFParser
{
public:
	static bool parse(CNF& cnf,const string& filename, int& NumVars, int& NumClauses)
       //设置成静态函数，方便调用，不需要实例化对象
    {

        ifstream file(filename);
        std::string line;

		if (!file.is_open())//打开文件失败
        {
			cerr << "打开文件: " << filename <<"失败"<< endl;//输出错误信息
            return false;
        }
   //调试     else
   //     {
			//printf("File opened successfully\n");//打开文件成功
   //     }
        if (!cnf.isEmpty())//如果CNF不为空
        {
			cnf.clauses.clear();//清空CNF
        }
		while (std::getline(file, line)) // 逐行读取文件
        {
			//cout << line << endl;
            istringstream iss(line);
			
            if (line[0] == 'c' || line.empty()) {
                continue; // 忽略注释和空行
            }

            if (line[0] == 'p') {
				string format;//读取掉p cnf
				iss >> format >> format >> NumVars >> NumClauses;//读取变量数和子句
                //调试 cout << numVars << " " << numClauses << endl; 
            }
			
            else {
				Clause clause;//新建子句
				
				int var;//文字的值
                while (iss >> var && var != 0) {
                    Literal l(var);
					clause.addLiteral(l);//添加文字				
                }	
				
				cnf.addClause(clause);//添加子句
              
				
            }
        }

		file.close();//关闭文件
        return true;
    }
};