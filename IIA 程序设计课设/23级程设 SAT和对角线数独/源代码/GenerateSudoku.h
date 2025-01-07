#pragma once
#include"vector.hpp"
#define SIZE 9
//产生数独
typedef struct
{
    int base_col; // 当前行的起始搜索列
    int curr_col; // 实际存储列
    unsigned int col_mask;//记录前面行的数据，推算出当前行实际可行的位置
} Col_Index;//某一行的数据结构，根据前面所有行的数据，推算出当前行实际可行的位置，方便对当前行的搜索

typedef struct {
    int long_row; // 当前搜索到第几行（哪个数字的哪一行），81为搜索完成
    Col_Index col_index[81]; //每一行的实际搜索状态
} Sodoku;//数独局面

void InitSodoku(Sodoku* sodoku);//初始化数独
bool isSodokuInProgress(const Sodoku* sodoku);//判断数独是否完成
void moveNextRow(Sodoku* sodoku);//跳到下一行
void rollback_row(Sodoku* sodoku);//回滚到上一行
int calcRandCol(void);//计算随机列
int calNextCol(int col);//计算下一列
 void calculateColumnMaskForRow(Sodoku* sodoku, int long_row);//计算列掩码
bool moveNextCol(Sodoku* sodoku);//跳到下一列
void SaveSodokutoBoard(Sodoku* sodoku, vector<int>& Board);//保存数独到数组
void PrintBoard(const vector<int>& Board);//打印数独
void StartSodokuGenerate(Sodoku* sodoku, vector<int>& Board);//开始数独生成
void shuffleArray(vector<int>& array);//打乱数组
int generateGameBoard(const vector<int>& normalBoard, vector<int>& gameBoard);//挖洞


