#include "definition.h"
#include "SATsolver.cpp"
#include "twosudoku.cpp"

int SATsolver() {
	int op = 1, strategy; //记录用户输入
	char input[50];
	char output[50];//输入输出文件名
	formula original;
	original.clausenum = 0;
	original.literalnum = 0;
	original.singleclausenum = 0;
	original.currentclausenum = 0;
	original.head = NULL;//初始化CNF公式
	while (op) {
		system("cls");
		printf("\n                       SAT问题求解                       \n");
		printf("-----------------------------------------------------------\n");
		printf("     1.读取cnf算例文件                  2.遍历并输出子句   \n");
		printf("     3.DPLL求解（同时输出到.res文件）   4.去除当前子句集   \n");
		printf("     0.退出\n");
		printf("-----------------------------------------------------------\n");
		printf("请选择操作（0-4）:");
		scanf("%d", &op);
		switch (op) {
			case 1:
				if (original.head != NULL) {
					printf("-----------------------------------------------------------\n");
					printf("已存在cnf算例文件！\n");
					system("pause");
					break;
				} else {
					printf("-----------------------------------------------------------\n");
					printf("请输入cnf算例文件名称：");
					scanf("%s", input);
					strcpy(output, input);
					output[strlen(output) - 3] = 'r';
					output[strlen(output) - 2] = 'e';
					output[strlen(output) - 1] = 's';
					cnfopen(original, input);
					system("pause");
					break;
				}
			case 2:
				if (original.head == NULL) {
					printf("-----------------------------------------------------------\n");
					printf("不存在cnf算例文件！\n");
					system("pause");
					break;
				} else {
					printformula(original);
					system("pause");
					break;
				}
			case 3:
				if (original.head == NULL) {
					printf("-----------------------------------------------------------\n");
					printf("不存在cnf算例文件！\n");
					system("pause");
					break;
				} else {
					printf("-----------------------------------------------------------\n");
					printf("请选择算法（1为原始算法，2为优化算法）：");
					scanf("%d", &strategy);
					clock_t ta = clock();//记录算法开始时间
					if (DPLL(original, strategy) == OK) {
						clock_t tb = clock();//记录算法结束时间
						printf("-----------------------------------------------------------\n");
						printf("问题求解完成，一种可行的结果如下：\n");
						for (int i = 1; i <= original.literalnum; i++) {
							if (result[i])
								printf("%d ", i);
							else
								printf("-%d ", i);
						}
						saveres(output, original, (tb - ta) * 1000 / CLOCKS_PER_SEC, 1);
						printf("\n-----------------------------------------------------------\n");
						printf("\n求解用时%d ms，答案已保存！\n", (tb - ta) * 1000 / CLOCKS_PER_SEC);
					} else {
						clock_t tc = clock();
						saveres(output, original, (tc - ta) * 1000 / CLOCKS_PER_SEC, 0);
						printf("问题无解，求解用时%d ms，答案已保存！\n", (tc - ta) * 1000 / CLOCKS_PER_SEC);
					}
					system("pause");
					break;
				}
			case 4:
				if (original.head == NULL) {
					printf("-----------------------------------------------------------\n");
					printf("不存在cnf算例文件！\n");
					system("pause");
					break;
				} else {
					deleteformula(original);
					system("pause");
					break;
				}
			case 0:
				return 0;
		}
	}
	return 0;
}

int twosudoku() {
	int op = 1, ans = 0, holecount = 0; //记录用户输入、DPLL求解结果和挖洞数量
	int a, x, y, value; //记录第a个数独x行y列填入的值value
	formula newdouble;
	newdouble.clausenum = 0;
	newdouble.literalnum = 0;
	newdouble.singleclausenum = 0;
	newdouble.currentclausenum = 0;
	newdouble.head = NULL;//初始化双数独CNF
	while (op) {
		system("cls");
		printf("\n                        双数独游戏                       \n");
		printf("-----------------------------------------------------------\n");
		printf("          1.求解双数独游戏              2.生成双数独\n");
		printf("          0.退出\n");
		printf("-----------------------------------------------------------\n");
		printf("请选择操作（0-2）：");
		scanf("%d", &op);
		switch (op) {
			case 1:
				generatecnf(newdouble);
				printf("下面展示初始双数独棋盘格局：\n");
				printtwosudoku1(newdouble);
				system("pause");
				printf("\n\nDPLL求解结果如下：\n");
				ans = solvesudoku(newdouble);
				printf("%d（1代表有解，0代表无解）\n\n", ans);
				if (ans)
					for (int i = 1; i <= newdouble.literalnum; i++) {
						if (result[i])
							printf("%d ", i);
						else
							printf("-%d ", i);
					}
				system("pause");
				printf("\n\n原始双数独求解结果如下：\n");
				printtwosudoku2(newdouble);
				system("pause");
				break;
			case 2:
				if (!fullboard[111]) {
					printf("不存在初始棋盘，请先求解！\n");
					system("pause");
					break;
				}
				printf("请输入挖洞数量：\n");
				scanf("%d", &holecount);
				digholes(newdouble, fullboard, holecount);//挖洞
				printtwosudoku2(newdouble);
				while (holecount > 0) {//游玩过程
					printf("请输入填入数字的数独位置（左上角为1，右下角为2）、行、列和值，用空格隔开：\n");
					scanf("%d %d %d %d", &a, &x, &y, &value);
					if (!board[a * 100 + x * 10 + y] && fullboard[a * 100 + x * 10 + y] == value) {
						board[a * 100 + x * 10 + y] = value;
						holecount--;
						printf("此方格输入正确！\n");
					} else
						printf("输入有误，请重新输入！\n");
					system("pause");
					system("cls");
					printf("\n\n\n\n\n\n");
					printtwosudoku2(newdouble);
				}
				printf("数独求解正确！\n");
				system("pause");
				break;
			case 0:
				return 0;
		}
	}
	return 0;
}

int main() {
	int op = 1;
	while (op) {
		system("cls");
		printf("\n              基于SAT的双数独游戏求解程序                \n");
		printf("\n           大数据2101班  李嘉鹏  U202115652              \n");
		printf("-----------------------------------------------------------\n");
		printf("          1.SAT问题求解           2.生成双数独\n");
		printf("          0.退出\n");
		printf("-----------------------------------------------------------\n");
		printf("请选择操作（0-2）：");
		scanf("%d", &op);
		switch (op) {
			case 1:
				SATsolver();
				system("pause");
				break;
			case 2:
				twosudoku();
				system("pause");
				break;
			case 0:
				return 0;
		}
	}
	return 0;
}