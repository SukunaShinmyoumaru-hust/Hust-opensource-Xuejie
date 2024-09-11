#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

#define TRUE 1        //判断为真 
#define FALSE 0       //判断为假 
#define OK 1          //操作执行成功 
#define ERROR 0       //操作执行失败 
#define INFEASTABLE -1     //结构体未分配存储空间 
#define OVERFLOW -2       //发生溢出，分配储存空间失败 

typedef struct lNode {
	int l;          //存储文字 
	int exist;      //标记该文字是否被删除，初始值为1             
	struct lNode* next;
}lNode, * plNode;               //存储一个文字节点

typedef struct cNode {
	int l_num;			  //存储一个子句的文字个数
	int flag;             //记录该子句因为flag而被删除，初始值为0 
	int exist;             //标记该子句是否被删除 ，初始值为1 
	lNode* firstl;        //指向第一个文字节点 
	struct cNode* next;   //指向下一条子句 
}cNode, * pcNode;     //子句(头)节点 

typedef struct iNode {
	cNode* p_cnode;       //存储指向各文字头结点的指针 
	struct iNode* next;
}iNode, * piNode;       //索引表的邻接点 

typedef struct {
	iNode* firstpos;       //正文字的索引点 
	iNode* firstneg;       //负文字的索引点 
} index, * pindex;//对索引表结点的定义 

typedef struct cnf {
	int vari_num;        //变元个数 
	int clau_num;        //子句个数
	cNode* firstclau;       //指向第一个子句节点
	index* Index_List;   //各文字索引表 
}cnf, * pcnf;            //一个cnf公式

typedef struct AnsList {
	int* elem;		//数据域（线性选择文字）
	int length;		//表长
}AnsList;     //一个顺序表结构，存储答案 

int loadCNF(pcnf* L, char filename[80]);        //创建CNF式 
int printCNF(pcnf L);     //打印CNF公式 
int loadAnswer(pcnf L, AnsList& An);
int deleteclau(pcnf L, int flag);
int restoreclau(pcnf L, int flag);
int EmptyClause(pcnf L);
int Findl(pcnf L, AnsList& An);
int DPLL(pcnf L, AnsList& An, int now_l);
int verify(pcnf L, AnsList& An);
int rule_1(char filename[80]);
int rule_2(char filename[80]);
int rule_3(char filename[80]);
int write(char filename[80]);
int WriteCNF(char filename[80]);
int SaveSolution(int res, double time, AnsList& An, char filename[80]);
int ResWrite_2(int res, double time, AnsList& An, char filename[80]);
