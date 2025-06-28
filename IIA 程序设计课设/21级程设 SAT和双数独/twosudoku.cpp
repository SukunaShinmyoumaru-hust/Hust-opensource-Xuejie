#include "definition.h"
#include "SATsolver.cpp"

/*列举双数独游戏中的变元编码：
格式：abcd a左上/右下大格子 b/c行/列位置 d填入的数
数独1：
1111 1112 1113 ... 1119
1121 1122 1123 ... 1129
...
1191 1192 1193 ... 1199
...
1991 1992 1993 ... 1999 共9*9*9=729个literal
数独2：同上，第一位改成2，共729个literal。因此总共有1458个变元
对于重叠部分：每个格对应18条子句

对于literal的表示，设计转换公式：abcd=(a-1)*729+(b-1)*81+(c-1)*9+d
以下循环中，四位数格式为aijd
*/
clause *frontc = NULL; //记录复制过程中子句的位置

int convert(int x) {//将四位数语义编码从1到1458连续编码
	int a = x / 1000, i = (x - a * 1000) / 100, j = (x - a * 1000 - i * 100) / 10, d = x % 10;
	return (a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d;
}

int reverse(int x) { //将变元转化为abcd的形式
	int a, b, c, d;
	a = (x - 1) / 729 + 1;
	b = (x - (a - 1) * 729 - 1) / 81 + 1;
	c = (x - (a - 1) * 729 - (b - 1) * 81 - 1) / 9 + 1;
	d = x - (a - 1) * 729 - (b - 1) * 81 - (c - 1) * 9;
	return a * 1000 + b * 100 + c * 10 + d;
}

void addclausefull(formula &newdouble, int count, int temp[50]) {
	//添加完整的子句，count代表句中变元数量
	clause *newclause = (clause *)malloc(sizeof(clause));
	literal *frontl = NULL;
	newclause->next = NULL;
	newclause->num = count;
	for (int i = 1; i <= count; i++) {
		literal *newliteral = (literal *)malloc(sizeof(literal));
		newliteral->word = temp[i];
		newliteral->next = NULL;
		if (i == 1) {
			newclause->head = newliteral;
			frontl = newliteral;
		} else {
			frontl->next = newliteral;
			frontl = newliteral;
		}
	}
	if (newdouble.head == NULL) {
		newdouble.head = newclause;
		frontc = newclause;
	} else {
		frontc->next = newclause;
		frontc = frontc->next;
	}
	newdouble.clausenum++;
	newdouble.currentclausenum++;
	if (count == 1)
		newdouble.singleclausenum++;
}

void generatecnf(formula &newdouble) {//生成双数独约束条件CNF子句集
	newdouble.clausenum = 0;
	newdouble.currentclausenum = 0;
	newdouble.emptyclausenum = 0;
	newdouble.literalnum = 1458;
	newdouble.singleclausenum = 0;
	newdouble.head = NULL; //初始化
	//加入初始提示数信息
	addunitclause(newdouble, convert(1173));
	addunitclause(newdouble, convert(1185));
	addunitclause(newdouble, convert(1217));
	addunitclause(newdouble, convert(1236));
	addunitclause(newdouble, convert(1254));
	addunitclause(newdouble, convert(1299));
	addunitclause(newdouble, convert(1322));
	addunitclause(newdouble, convert(1384));
	addunitclause(newdouble, convert(1415));
	addunitclause(newdouble, convert(1446));
	addunitclause(newdouble, convert(1469));
	addunitclause(newdouble, convert(1514));
	addunitclause(newdouble, convert(1588));
	addunitclause(newdouble, convert(1626));
	addunitclause(newdouble, convert(1639));
	addunitclause(newdouble, convert(1668));
	addunitclause(newdouble, convert(1675));
	addunitclause(newdouble, convert(1716));
	addunitclause(newdouble, convert(1729));
	addunitclause(newdouble, convert(1753));
	addunitclause(newdouble, convert(1813));
	addunitclause(newdouble, convert(1834));
	addunitclause(newdouble, convert(2148));
	addunitclause(newdouble, convert(2195));
	addunitclause(newdouble, convert(2247));
	addunitclause(newdouble, convert(2274));
	addunitclause(newdouble, convert(2387));
	addunitclause(newdouble, convert(2445));
	addunitclause(newdouble, convert(2482));
	addunitclause(newdouble, convert(2499));
	addunitclause(newdouble, convert(2523));
	addunitclause(newdouble, convert(2643));
	addunitclause(newdouble, convert(2658));
	addunitclause(newdouble, convert(2667));
	addunitclause(newdouble, convert(2724));
	addunitclause(newdouble, convert(2781));
	addunitclause(newdouble, convert(2797));
	addunitclause(newdouble, convert(2839));
	addunitclause(newdouble, convert(2896));
	addunitclause(newdouble, convert(2913));
	addunitclause(newdouble, convert(2928));
	addunitclause(newdouble, convert(2949));
	frontc = newdouble.head;
	while (frontc->next != NULL)
		frontc = frontc->next;

	//每个单元格只能填入唯一一个数字,此处将中间重叠的9个格视为两个单独的格子
	for (int a = 1; a <= 2; a++)
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++) {
				for (int d = 1; d <= 9; d++) {
					temp[d] = (a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d;
				}
				addclausefull(newdouble, 9, temp);
				for (int m = 1; m <= 8; m++)
					for (int n = m + 1; n <= 9; n++) {
						temp[1] = -((a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + m);
						temp[2] = -((a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + n);
						addclausefull(newdouble, 2, temp);
					}
			}

	//行约束
	for (int a = 1; a <= 2; a++)
		for (int i = 1; i <= 9; i++) {
			for (int d = 1; d <= 9; d++) {
				for (int j = 1; j <= 9; j++)
					temp[j] = (a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d;
				addclausefull(newdouble, 9, temp);
			}
			for (int d = 1; d <= 9; d++)
				for (int j = 1; j <= 8; j++)
					for (int k = j + 1; k <= 9; k++) {
						temp[1] = -((a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d);
						temp[2] = -((a - 1) * 729 + (i - 1) * 81 + (k - 1) * 9 + d);
						addclausefull(newdouble, 2, temp);
					}
		}

	//列约束
	for (int a = 1; a <= 2; a++)
		for (int j = 1; j <= 9; j++) {
			for (int d = 1; d <= 9; d++) {
				for (int i = 1; i <= 9; i++)
					temp[i] = (a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d;
				addclausefull(newdouble, 9, temp);
			}
			for (int d = 1; d <= 9; d++)
				for (int i = 1; i <= 8; i++)
					for (int k = i + 1; k <= 9; k++) {
						temp[1] = -((a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d);
						temp[2] = -((a - 1) * 729 + (k - 1) * 81 + (j - 1) * 9 + d);
						addclausefull(newdouble, 2, temp);
					}
		}

	//3*3盒子约束
	for (int a = 1; a <= 2; a++)
		for (int horizontal = 1; horizontal <= 3; horizontal++)
			for (int vertical = 1; vertical <= 3; vertical++) {//3*3的大盒子（循环其实也可以只用一个变量，依据%9来确定位置）
				for (int d = 1; d <= 9; d++) {
					int count = 0;
					for (int i = (horizontal - 1) * 3 + 1; i <= horizontal * 3; i++)
						for (int j = (vertical - 1) * 3 + 1; j <= vertical * 3; j++)
							temp[++count] = (a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d;
					addclausefull(newdouble, 9, temp);
				}
				for (int d = 1; d <= 9; d++)//9个格子相互不能有重复数字
					for (int i = (horizontal - 1) * 3 + 1; i <= horizontal * 3; i++)
						for (int j = (vertical - 1) * 3 + 1; j <= vertical * 3; j++) {
							int itemp = i;
							while (itemp <= horizontal * 3) {
								if (itemp == i)
									for (int k = j + 1; k <= vertical * 3; k++) {
										temp[1] = -((a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d);
										temp[2] = -((a - 1) * 729 + (i - 1) * 81 + (k - 1) * 9 + d);
										addclausefull(newdouble, 2, temp);
									} else
									for (int k = (vertical - 1) * 3 + 1; k <= vertical * 3; k++) {
										temp[1] = -((a - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + d);
										temp[2] = -((a - 1) * 729 + (itemp - 1) * 81 + (k - 1) * 9 + d);
										addclausefull(newdouble, 2, temp);
									}
								itemp++;
							}
						}
			}

	//中间位置的两个数独共享一个盒子
	for (int i = 7; i <= 9; i++)
		for (int j = 7; j <= 9; j++)
			for (int d = 1; d <= 9; d++) {
				temp[1] = -((i - 1) * 81 + (j - 1) * 9 + d);
				temp[2] = 729 + (i - 7) * 81 + (j - 7) * 9 + d;
				addclausefull(newdouble, 2, temp);
				temp[1] = (i - 1) * 81 + (j - 1) * 9 + d;
				temp[2] = -(729 + (i - 7) * 81 + (j - 7) * 9 + d);
				addclausefull(newdouble, 2, temp);
			}
}

int solvesudoku(formula &newdouble) { //将DPLL求解的结果转移到board数组内
	if (!DPLL(newdouble, 2))
		return 0;
	else {
		for (int i = 1; i <= newdouble.literalnum; i++)
			if (result[i]) {
				board[reverse(i) / 10] = reverse(i) % 10;
				fullboard[reverse(i) / 10] = reverse(i) % 10;
			}
		for (int i = 7; i <= 9; i++)
			for (int j = 7; j <= 9; j++)
				fullboard[200 + (i - 6) * 10 + (j - 6)] = fullboard[100 + i * 10 + j];//同一个格子的另一种表示
		return 1;
	}
}

void printtwosudoku1(formula &newdouble) {//打印初始状态双数独棋盘
	for (int i = 0; i < 300; i++) {//初始化两个棋盘数组
		board[i] = 0;
		fullboard[i] = 0;
	}
	clause *findc = newdouble.head;
	int count = newdouble.singleclausenum;
	while (findc != NULL && count > 0) {
		if (findc->num == 1 && findc->head->word > 0) {
			board[reverse(findc->head->word) / 10] = reverse(findc->head->word) % 10;
			fullboard[reverse(findc->head->word) / 10] = reverse(findc->head->word) % 10;
			count--;
		}
		findc = findc->next;
	}
	for (int i = 1; i <= 6; i++) {//第一个大数独
		printf("|");
		for (int j = 1; j <= 9; j++) {
			if (board[100 + 10 * i + j])
				printf("  %d", board[100 + 10 * i + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}
		printf("\n");
		if (i == 3)
			printf("——————————————————\n");
		if (i == 6)
			printf("——————————————————————————————\n");
	}
	for (int i = 7; i <= 9; i++) { //重叠部分
		printf("|");
		for (int j = 1; j <= 9; j++) {
			if (board[100 + 10 * i + j])
				printf("  %d", board[100 + 10 * i + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}

		for (int j = 4; j <= 9; j++) {
			if (board[200 + 10 * (i - 6) + j])
				printf("  %d", board[200 + 10 * (i - 6) + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}
		printf("\n");
	}
	printf("                        ——————————————————\n");
	for (int i = 4; i <= 9; i++) {//第二个大数独
		printf("                        |");
		for (int j = 1; j <= 9; j++) {
			if (board[200 + 10 * i + j])
				printf("  %d", board[200 + 10 * i + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}
		printf("\n");
		if (i == 6)
			printf("                        ——————————————————\n");
	}
}

void printtwosudoku2(formula &newdouble) {//打印完成状态双数独棋盘
	for (int i = 1; i <= 6; i++) {//第一个大数独
		printf("|");
		for (int j = 1; j <= 9; j++) {
			if (board[100 + 10 * i + j])
				printf("  %d", board[100 + 10 * i + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}
		printf("\n");
		if (i == 3)
			printf("——————————————————\n");
		if (i == 6)
			printf("——————————————————————————————\n");
	}
	for (int i = 7; i <= 9; i++) { //重叠部分
		printf("|");
		for (int j = 1; j <= 9; j++) {
			if (board[100 + 10 * i + j])
				printf("  %d", board[100 + 10 * i + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}

		for (int j = 4; j <= 9; j++) {
			if (board[200 + 10 * (i - 6) + j])
				printf("  %d", board[200 + 10 * (i - 6) + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}
		printf("\n");
	}
	printf("                        ——————————————————\n");
	for (int i = 4; i <= 9; i++) {//第二个大数独
		printf("                        |");
		for (int j = 1; j <= 9; j++) {
			if (board[200 + 10 * i + j])
				printf("  %d", board[200 + 10 * i + j]);
			else
				printf("  _");
			if (j == 3 || j == 6 || j == 9)
				printf("  |");
		}
		printf("\n");
		if (i == 6)
			printf("                        ——————————————————\n");
	}
}

void digholes(formula &newdouble, int fullboard[300], int holecount) {
	int position;//挖洞位置，每次随机生成
	int trycount;//其它可能的解数量
	while (holecount > 0) {
		position = rand() % (299 - 111 + 1) + 111; //生成在111到299之间的一随机数
		if (!board[position])
			continue;
		while (board[position]) { //遍历确保挖去后解唯一
			if (!board[position])
				continue;
			trycount = 0;
			formula test;
			copyformula(test, newdouble);
			for (int i = 1; i <= 9; i++)
				if (i != fullboard[position])
					temp[++trycount] = i;
			addclausefull(test, 8, temp);
			if (!DPLL(test, 2)) { //说明此处只能填这一个数，可以挖洞
				board[position] = 0;
				if (position == 177 || position == 178 || position == 179 || position == 187 || position == 188 || position == 189
				        || position == 197 || position == 198 || position == 199) //重叠的位置
					board[200 + ((position - 100) / 10 - 6) * 10 + position % 10 - 6] = 0;
				if (position == 211 || position == 212 || position == 213 || position == 221 || position == 222 || position == 223
				        || position == 231 || position == 232 || position == 233)
					board[100 + ((position - 200) / 10 + 6) * 10 + position % 10 + 6] = 0;
				break;
			} else //说明挖这个洞不合法
				while (1) {
					position = rand() % (299 - 111 + 1) + 111; //生成在111到299之间的一随机数
					if (board[position])
						break;
				}
		}
		holecount--;
	}
}