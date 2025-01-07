#pragma once
#include"GenerateSudoku.h"

//检查在给定的行、列和数字的情况下，数字是否可以安全地放置在数独棋盘上
bool isSafe(const vector<int>& board, int row, int col, int num);
//随机生成数独棋盘
bool solveSudoku(vector<int>& board);
//随机生成第一行
void GenerateFirstLine(vector<int>& a);
//生成对角线数独
void generateDiagonalSudoku(vector<int>& board);