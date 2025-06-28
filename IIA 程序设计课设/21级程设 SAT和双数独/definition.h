#ifndef data_structure_project_2022
#define data_structure_project_2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0





int result[3500] = {0}; //各文字取值的结果数组，1代表真，0代表假
int found[3500] = {0}; //单子句的查找记录数组
int temp[50] = {0}; //创建双数独cnf时暂存读入的数
int board[300] = {0}; //存储棋盘上数字的解，board[abc]表示第a个大数独中b行c列的值
int fullboard[300] = {0}; //存储初始时的合法棋盘解（用于挖洞），全过程保持不变
FILE *fp;

typedef struct literal { //以链表形式存储文字
	int word;//正值代表正文字，反之为负文字
	struct literal *next;//下一个文字
} literal, *L;

typedef struct clause { //以链表形式存储子句
	int num; //某子句内的文字总数
	struct literal *head; //第一个文字
	struct clause *next; //下一个子句
} clause, *C;

typedef struct formula { //整个公式
	int clausenum;//文字总数
	int literalnum;//子句总数
	int emptyclausenum;//空子句总数
	int singleclausenum;//单子句总数（随DPLL进行而减少）
	int currentclausenum;//当前子句总数（随DPLL进行而减少）
	struct clause *head;//第一个子句
} formula, *F;
#endif