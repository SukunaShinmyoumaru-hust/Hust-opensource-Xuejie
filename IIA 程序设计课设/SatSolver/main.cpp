#include "Sat_Puzzle.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	pcnf L = NULL;			//存cnf文件
	AnsList Ans;				//存储答案
	clock_t start, end;              //记录时间
	double duration;
	char filename[80];
	//三个功能选择 
	int op = 1;
	int op_1 = 1;
	int op_2 = 1;
	int tag = -1;
	while (op) {//两大模块sat和puzzle 
		system("cls");	printf("\n\n");
		//printf("      Menu for Linear Table On Sequence Structure \n");
		printf("		欢迎使用DPLL-SAT求解器 \n");
		printf("------------------------------------------------------\n");
		printf("    	  1. SAT算例求解       2. 二进制数独\n");
		printf("          0. 退出                   \n");
		printf("------------------------------------------------------\n");
		printf("    请选择你的操作[0~2]:");
		scanf("%d", &op);
		switch (op) {  //演示系统的功能选择 
		case 1:
			op_1 = 1;
			while (op_1) {//sat部分系统 
				system("cls");	printf("\n\n");
				//printf("     Menu for Linear Table On Sequence Structure    \n");
				printf("					请首先读入CNF文件    \n");
				printf("----------------------------------------------------\n");
				printf("       1. 读入CNF文件         2. 打印CNF范式               \n");
				printf("       3. DPLL算法求解        4. 验证结果                  \n");
				printf("       5. 打印解	             6. 保存解到文件            \n");
				printf("       0. 退出                                      \n");
				printf("----------------------------------------------------\n");
				printf("     请选择你的操作[0~6]:\n");
				scanf("%d", &op_1);
				switch (op_1) {
				case 1:
					//printf("\n--------creatCNF功能待实现！\n");
					printf("请输入选择的文件名\n");
					scanf("%s", filename);
					if (loadCNF(&L, filename) == OK) printf("cnf公式创建成功\n");
					else {
						printf("cnf公式创建失败\n");
						getchar(); getchar();
						break;
					}
					if (loadAnswer(L, Ans) == OK) printf("分配结果存储空间成功\n");
					else {
						printf("分配结果存储空间失败\n");
						getchar(); getchar();
						break;
					}
					getchar(); getchar();
					break;
				case 2:
					//printf("\n--------printCNF功能待实现！\n");
					if (printCNF(L) == OK) printf("cnf公式打印成功\n");
					else printf("打印失败\n");
					getchar(); getchar();
					break;
				case 3:
					//printf("\n--------DPLL功能待实现！\n");
					start = clock();
					if (DPLL(L, Ans, 1) == OK) {
						tag = 1;
						printf("求解成功\n");
					}
					else {
						tag = 0;
						printf("求解失败\n");
					}
					end = clock();
					duration = ((double)(end - start)) / CLK_TCK * 1000;
					printf("运行时间为：%f毫秒\n", duration);
					getchar(); getchar();
					break;
				case 4:
					//printf("\n--------verify功能待实现！\n");
					if (verify(L, Ans) == TRUE) printf("------------答案合理\n");
					else printf("---------答案不合理\n");
					getchar(); getchar();
					break;
				case 5:
					//printf("\n--------prfAnswer功能待实现！\n");
					for (int i = 1; i < Ans.length; i++) {
						if (Ans.elem[i] == -1)  printf("%5d", -i);
						else printf("%5d", i);
						if (i % 10 == 0)  printf("\n");
					}
					getchar(); getchar();
					break;
				case 6:
					//printf("\n------ResWrite功能待实现\n");
					if (SaveSolution(tag, duration, Ans, filename) == OK) printf("结果写入成功\n");
					else printf("结果写入失败\n");
					getchar(); getchar();
					break;
				case 0:
					break;
				}
			}
			//getchar();getchar();
			break;
		case 2:
			op_2 = 1;
			while (op_2) {//puzzle部分系统 
				system("cls");	printf("\n\n");
				printf("     Menu for Linear Table On Sequence Structure    \n");
				printf("----------------------------------------------------\n");
				printf("       1. WriteCNF         2. loadCNF             \n");
				printf("       3. DPLL             4. verify                  \n");
				printf("       5. prfAnswer        6. ResWrite              \n");
				printf("       0. Exit                                      \n");
				printf("----------------------------------------------------\n");
				printf("     请选择你的操作[0~6]:\n");
				scanf("%d", &op_2);
				switch (op_2) {
				case 1:
					printf("\n--------WriteFile功能待实现！\n");
					printf("请输入选择的文件名\n");
					scanf("%s", filename);
					if (WriteCNF(filename) == OK)  printf("棋局文件写入成功\n");
					else printf("棋局文件写入失败\n");
					getchar(); getchar();
					break;
				case 2:
					printf("\n--------loadCNF功能待实现！\n");
					if (loadCNF(&L, filename) == OK) printf("cnf公式创建成功\n");
					else {
						printf("cnf公式创建失败\n");
						getchar(); getchar();
						break;
					}
					if (loadAnswer(L, Ans) == OK) printf("分配结果存储空间成功\n");
					else {
						printf("分配结果存储空间失败\n");
						getchar(); getchar();
						break;
					}
					getchar(); getchar();
					break;
				case 3:
					printf("\n--------DPLL功能待实现！\n");
					start = clock();
					if (DPLL(L, Ans, 1) == OK) {
						tag = 1;
						printf("求解成功\n");
					}
					else {
						tag = 0;
						printf("求解失败\n");
					}
					end = clock();
					duration = ((double)(end - start)) / CLK_TCK * 1000;
					printf("运行时间为：%f毫秒\n", duration);
					getchar(); getchar();
					break;
				case 4:
					printf("\n--------verify功能待实现！\n");
					if (verify(L, Ans) == TRUE) printf("------------答案合理\n");
					else printf("----------答案不合理\n");
					getchar(); getchar();
					break;
				case 5:
					printf("\n--------prfAnswer功能待实现！\n");
					for (int i = 1; i < Ans.length; i++) {
						if (Ans.elem[i] == -1)  printf("0 ");
						else printf("1 ");
						if (i % 6 == 0)  printf("\n");
					}
					getchar(); getchar();
					break;
				case 6:
					printf("\n------ResWrite功能待实现\n");
					if (ResWrite_2(tag, duration, Ans, filename) == OK) printf("结果写入成功\n");
					else printf("结果写入失败\n");
					getchar(); getchar();
					break;
				case 0:
					break;
				}
			}
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	return 0;
}
