#pragma once
#include"CNFParser.h"


bool propagate(CNF& cnf, const Literal& literal,int* assignments);
Literal selectLiteral(const CNF& cnf,int way);
 bool DPLL(CNF& cnf,int way, int* assignments);
 void printAssignments(const int* assignments, int choice);
 void saveResultToFile(const std::string& baseFilename, bool result, const int* assignments, double duration);
