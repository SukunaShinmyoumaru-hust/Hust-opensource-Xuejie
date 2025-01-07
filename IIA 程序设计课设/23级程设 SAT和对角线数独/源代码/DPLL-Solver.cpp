#include "DPLL-Solver.h"
#include <chrono>

extern int  numVars;//变元个数，在main.cpp中定义
extern int Count;//调试用，检测递归次数

/***********************************************
*函数名称：propagate
* 函数功能：根据给定的文字（literal）对 CNF 公式进行传播，更新 CNF 公式和赋值数组。
* 注释：- 遍历 CNF 公式中的所有子句。
        - 检查子句是否已被给定文字（literal）满足。
        - 如果子句已被满足，则忽略该子句。
        - 如果子句未被满足，删除子句中与给定文字相反的文字。
        - 如果删除文字后子句为空，则返回 False，表示冲突。
        - 如果子句非空，将其添加到新的 CNF 公式中。
        - 更新赋值数组以记录文字的赋值。
* 返回值：bool 类型，如果传播成功，则返回 True，若出现空子句，则返回 False。
************************************************/
bool propagate(CNF& cnf, const Literal& literal,int *assignments) {
    CNF newCNF;
    bool hasChanged = false;

    for (int i = 0; i < cnf.clauses.size(); ++i) {
        const Clause& clause = cnf.clauses[i];//取出一个子句

        //检查字句是否包含文字
        bool satisfied = false;
        for (int j = 0; j < clause.literals.size(); ++j) {
            if (clause.literals[j] == literal) //文字相等
            {
                satisfied = true;
                break;
            }
        }

        if (satisfied) {
			//删除已满足的字句，即不添加到新CNF
            continue;
        }

        //删除子句中的~L文字
        Clause newClause = clause;
        Literal negLiteral(-literal.var);
        newClause.removeLiteral(negLiteral);

        //如果删除~L后子句不为空，添加到新CNF
        if (!newClause.literals.empty()) {
            newCNF.addClause(newClause);
        }
        else {
            // 如果出现空子句，返回false
            return false;
        }
        hasChanged = true;//标记已经改变
    }
	assignments[abs(literal.var)] = (literal.var > 0) ? 1 : -1; //记录赋值情况
	cnf = newCNF;//更新CNF
    return true;
}

/***********************************************
*函数名称：selectLiteral
* 函数功能：根据指定的策略选择一个文字。
* 注释：- 策略1：选择第一个子句的第一个文字。
        - 策略2：随机选择一个非空子句中的一个文字。
        - 策略3：选择出现次数最多的文字。
        - 策略4：选择权重最大的文字（不考虑正负）。
        - 策略5：选择权重最大的文字（考虑正负）。
* 返回值：Literal 类型，返回选择的文字。
************************************************/
 Literal selectLiteral(const CNF& cnf,int way) {
    // 策略1：选择第一个子句的第一个文字
	 
     Count++;
     if (way == 2) 
     {
         if (!cnf.clauses.empty() && !cnf.clauses[0].literals.empty()) //如果子句不为空
         {
             return cnf.clauses[0].literals[0];
         }
     }
	 // 策略2：随机选取一个文字
     else if (way == 1)
     {
            
         // 找到一个非空子句
         int clauseIndex = rand() % cnf.clauses.size();
         while (cnf.clauses[clauseIndex].literals.empty()) {
             clauseIndex = rand() % cnf.clauses.size();
         }

         // 选择子句中的一个文字
         int literalIndex = rand() % cnf.clauses[clauseIndex].literals.size();
         return cnf.clauses[clauseIndex].literals[literalIndex];
     }
	 // 策略3：选择出现次数最多的文字
     else if (way == 3)
	 {
         // 初始化计数数组
         vector<int> cnt(numVars * 2 + 1, 0);

         // 计算每个变元的出现次数
         for (const Clause& clause : cnf.clauses) {
             for (const Literal& literal : clause.literals) {
                 if (literal.var > 0) {
                     cnt[literal.var]++;
                 }
                 else {
                     cnt[numVars - literal.var]++;
                 }
             }
         }

         // 选择出现次数最多的变元（正面）
         int maxBool = 0;
         int maxTimes = 0;
         for (int i = 1; i <= numVars; ++i) {
             if (cnt[i] > maxTimes) {
                 maxTimes = cnt[i];
                 maxBool = i;
             }
         }

         // 如果正面变元的出现次数都是 1，则选择反面变元
         if (maxTimes == 1) {
             for (int i = numVars + 1; i <= numVars * 2; ++i) {
                 if (cnt[i] > maxTimes) {
                     maxTimes = cnt[i];
                     maxBool = numVars - i;
                 }
             }
         }

         return Literal(maxBool);
	 }
	 //策略4：选择权重最大的文字（不考虑正负）
     else if (way == 4)
     {

         // 使用 vector 替代裸指针
         vector<double> weight(numVars + 1, 0.0);

         // 计算每个变元的权重
         for (const Clause& clause : cnf.clauses) {
             double clauseWeight = pow(2.0, -static_cast<double>(clause.literals.size()));
             for (const Literal& literal : clause.literals) {
                 if (literal.var != 0) {
                     int varIndex = abs(literal.var);
                     if (varIndex <= numVars) {
                         weight[varIndex] += clauseWeight;
                     }
                 }
             }
         }

         // 找到最大权重的变元
         double maxWeight = 0.0;
         int maxBool = 0;
         for (int i = 1; i <= numVars; ++i) {
             if (weight[i] > maxWeight) {
                 maxWeight = weight[i];
                 maxBool = i;
             }
         }

         return Literal(maxBool);
     }
	 //策略5：选择权重最大的文字（考虑正负）
     else
     {
         // 使用 std::vector 替代裸指针
         vector<double> weight(numVars * 2 + 1, 0.0);

         // 计算每个变元的权重
         for (const Clause& clause : cnf.clauses) {
             double clauseWeight = pow(2.0, -static_cast<double>(clause.literals.size()));
             for (const Literal& literal : clause.literals) {
                 int varIndex = std::abs(literal.var);
                 if (literal.var > 0) {
                     weight[varIndex] += clauseWeight;
                 }
                 else {
                     weight[numVars - literal.var] += clauseWeight;
                 }
             }
         }

         // 找到最大权重的变元
         double maxWeight = 0.0;
         int maxBool = 0;
         for (int i = 1; i <= numVars; ++i) {
             double totalWeight = weight[i] + weight[i + numVars];
             if (totalWeight > maxWeight) {
                 maxWeight = totalWeight;
                 maxBool = i;
             }
         }

         // 确定最终变元是正面还是反面
         if (weight[maxBool] < weight[maxBool + numVars]) {
             maxBool = -maxBool;
         }

         return Literal(maxBool);
     }
         return Literal(); // 返回空文字
     
}

/***********************************************
*函数名称：DPLL
* 函数功能：使用 DPLL 算法对给定的 CNF 公式进行求解。
* 注释：- 不断查找单子句，并进行传播。
        - 如果找到单子句且传播成功，则继续处理。
        - 如果 CNF 公式为空，说明所有子句都满足，返回 True。
        - 否则，选择一个文字进行分支搜索。
        - 尝试给文字赋值为真或假，并递归地调用 DPLL 函数。
        - 如果任一分支成功，则返回 True；否则，返回 False。
* 返回值：bool 类型，如果 CNF 公式可满足，则返回 True，否则返回 False。
************************************************/
 bool DPLL(CNF& cnf,int way, int* assignments) {
    while (true) {
        bool unitClauseFound = false;
        for (int i = 0; i < cnf.clauses.size(); ++i) {
            const Clause& clause = cnf.clauses[i];
            if (clause.isUnit()) {
                //如果是单子句，传播
                Literal unitLiteral = clause.literals[0];//取出单子句的文字
                if (!propagate(cnf, unitLiteral, assignments)) {
                    return false;//如果传播失败，返回false
                }
              
                unitClauseFound = true;

                break;
            }
        }

		if (!unitClauseFound) {//没有单子句，跳出循环
            break;
        }
    }

    if (cnf.isEmpty()) {
        return true; // 所有子句都满足,返回true
    }

    //如果cnf中还有子句，选择文字
    if (cnf.hasNext())
    {
        // 选择文字来搜索
        Literal literal = selectLiteral(cnf,way);
        if (literal.var == 0) {
            return false; //没有可供选择的文字
        }

        // 尝试文字的两种赋值
        CNF cnfTrue = cnf;
        Literal posLiteral(literal.var);
        if (propagate(cnfTrue, posLiteral, assignments)) {
            if (DPLL(cnfTrue,way, assignments)) {

                return true;
            }
        }


        CNF cnfFalse = cnf;
        Literal negLiteral(-literal.var);
        if (propagate(cnfFalse, negLiteral, assignments)) {
            if (DPLL(cnfFalse,way, assignments)) {

                return true;
            }
        }
    }

    return false; // 两种分支都失败
}
 
 /***********************************************
*函数名称：printAssignments
* 函数功能：打印给定赋值数组中的变量赋值。
* 注释：-遍历赋值数组，打印赋值为 1 的变量。
        - 如果 choice 为 True，且赋值为 -1，则也打印负变量。
* 返回值：void
************************************************/
 void printAssignments(const int* assignments,int choice) {
     for (int i = 1; i <= numVars; ++i) {
         if (assignments[i] == 1) {
             std::cout << i << " ";
         }
         else if (choice&&assignments[i] == -1) {
             std::cout << -i << " ";
         }
     }
     std::cout << std::endl;
 }
 // 将结果保存到文件中
 void saveResultToFile(const std::string& baseFilename, bool result, const int* assignments,double duration) {
     string filename = baseFilename;
     filename.erase(filename.end() - 4,filename.end());
     filename += ".res";
     ofstream outFile(filename);

     if (!outFile) {
         cerr << "打开文件: " << filename << "失败" << endl;
         return;
     }

     // 写入求解结果
     outFile << "s " << (result ? "1" : "0") << std::endl;

     // 写入变量赋值
     outFile << "v ";
     if (result) {
     for (int i = 1; i <= numVars; ++i) {
         if (assignments[i] == 1) {
             outFile << i << " ";
         }
         else if (assignments[i] == -1) {
             outFile << -i << " ";
         }
     }
     outFile << std::endl;
 }
     // 写入执行时间
     outFile << "t " << duration << std::endl;

     outFile.close();
 }