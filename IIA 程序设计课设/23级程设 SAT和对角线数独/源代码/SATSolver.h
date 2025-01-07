#pragma once
#include"CNF.h"
#include"clause.h"
/*
在CNF（Conjunctive Normal Form，合取范式）求解器中，assignment 是一个关键的数据结构，
用于记录变量的赋值状态。具体来说，assignment 是一个布尔类型的向量（std::vector<bool>），
其中每个元素对应一个变量的赋值状态。
*/
/*
结构和含义
索引：assignment 中的每个索引对应一个变量。例如，assignment[1] 表示变量 1 的赋值状态。
值：assignment 中的每个值（true 或 false）表示变量的赋值状态。通常，true 表示变量被赋值
为 true，false 表示变量未被赋值或被赋值为 false。
*/
class SATSolver
{
public:
    bool solve(CNF& cnf, vector<bool>& assignment) {
        // 如果CNF为空，返回True
        if (cnf.clauses.empty()) return true;

        // 如果有空子句，返回False
        for (int i = 0; i < cnf.clauses.size(); ++i) {
            const Clause& clause = cnf.clauses[i];
            if (clause.literals.empty()) return false;
        }

        // 选择一个未赋值的文字
        int literal = selectLiteral(cnf, assignment);
        if (literal == 0) return false; // 如果没有找到未赋值的文字，返回false

        // 递归求解true
        CNF cnfTrue = propagate(cnf, literal, true, assignment); // 传递真值
        vector<bool> assignmentTrue = assignment; // 复制assignment
        assignmentTrue[abs(literal)] = (literal > 0); // 记录正文字的赋值状态

        if (solve(cnfTrue, assignmentTrue)) { // 递归求解
            assignment = assignmentTrue;
            return true;
        }

        // 递归求解false
        CNF cnfFalse = propagate(cnf, literal, false, assignment);
        vector<bool> assignmentFalse = assignment;
        assignmentFalse[abs(literal)] = (literal < 0); // 记录正文字的赋值状态

        if (solve(cnfFalse, assignmentFalse)) { // 递归求解
            assignment = assignmentFalse;
            return true; // 如果有解，返回true
        }

        // 如果没有解，返回false
        return false;
    }

private:
    // 选择策略1：选取第一个未赋值的文字
    int selectLiteral(const CNF& cnf, const vector<bool>& assignment) {
        for (int i = 0; i < cnf.clauses.size(); ++i) {
            const Clause& clause = cnf.clauses[i];
            for (int j = 0; j < clause.literals.size(); ++j) {
                const Literal& literal = clause.literals[j];
                int varIndex = abs(literal.var);
                if (varIndex >= assignment.size() || !assignment[varIndex]) {
                    return literal.var;
                }
            }
        }
        return 0;
    }

    // 传递分裂结果
    CNF propagate(const CNF& cnf, int literal, bool value, const vector<bool>& assignment) {
        CNF newCNF;

        for (int i = 0; i < cnf.clauses.size(); ++i) {
            const Clause& clause = cnf.clauses[i];
            Clause newClause;
            bool satisfied = false; // 是否满足

            for (int j = 0; j < clause.literals.size(); ++j) {
                const Literal& lit = clause.literals[j];
                if ((lit.var == literal && value) || (lit.var == -literal && !value)) {
                    // 如果文字已经满足，根据单子句规则，整个子句满足
                    satisfied = true;
                    break;
                }
                int varIndex = abs(lit.var);
                if (varIndex >= assignment.size() || !assignment[varIndex]) {
                    // 与文字不相等且未赋值
                    newClause.addLiteral(lit);
                }
            }

            if (!satisfied) { // 如果子句没有满足，添加到新CNF中
                newCNF.addClause(newClause);
            }
        }

        return newCNF;
    }
};
