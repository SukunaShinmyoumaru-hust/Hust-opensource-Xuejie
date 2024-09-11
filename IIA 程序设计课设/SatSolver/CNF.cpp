#include "Sat_Puzzle.h"
int loadCNF(pcnf* L, char filename[80]) {
	pcNode p;    //子句结点 
	plNode q;    //文字节点
	piNode r;    //索引节点 
	int i;
	//为cnf结构变量L，子句头结点cNode结构变量p分配存储空间
	*L = (cnf*)malloc(sizeof(struct cnf));
	p = (cNode*)malloc(sizeof(struct cNode));
	p->flag = 0;
	p->exist = 1;
	(*L)->firstclau = p;	//首字句指向p
	FILE* fp;
	int num = 0;
	int k = 0;              //记录每个子句文字个数 
	int m = 0;             //记录子句的个数 
	int positive;         //记录正负 
	char c;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return 0;
	}
	fscanf(fp, "%c", &c);
	while (!feof(fp)) {
		if (c == 'c') {            //cnf文件注释部分 
			while (c != '\n') {
				fscanf(fp, "%c", &c);
			}
			fscanf(fp, "%c", &c);
		}
		else if (c == 'p') {	//cnf文件信息头
			while (c != '\n') {
				while (c < '0' || c>'9') {
					fscanf(fp, "%c", &c);
				}
				while (c >= '0' && c <= '9') {        //获取变元数 
					num = num * 10;			//前面所有位+1，空出新的个位待读取
					num += c - '0';			//个位
					fscanf(fp, "%c", &c);	//读取到空格为止
				}

				(*L)->vari_num = num;	//存储变元数
				(*L)->Index_List = (index*)malloc((num + 1) * sizeof(index));//分配索引表空间
				for (i = 0; i <= num; i++) {//索引表指针初始化
					(*L)->Index_List[i].firstneg = NULL;
					(*L)->Index_List[i].firstpos = NULL;
				}
				num = 0;
				while (c < '0' || c>'9') {
					fscanf(fp, "%c", &c);
				}
				while (c >= '0' && c <= '9') {        //获取子句数 
					num = num * 10;
					num += c - '0';
					fscanf(fp, "%c", &c);
				}
				(*L)->clau_num = num;
				num = 0;
			}
			fscanf(fp, "%c", &c);
		}
		else if ((c >= '0' && c <= '9') || (c == '-')) {//cnf文件的子句
			//处理文字结构lNode q
			q = (lNode*)malloc(sizeof(struct lNode));	//文字结点q
			q->exist = 1;
			p->firstl = q;		//首字句的第一个文字为q
			while (c != '\n') {
				while (c != '0') {
					while (c != ' ') {
						positive = 1;	//默认文字为正
						while ((c >= '0' && c <= '9') || (c == '-')) {
							if (c == '-') {
								fscanf(fp, "%c", &c);
								positive = 0;	//文字为负
							}
							num = num * 10;	//记录文字绝对值
							num += c - '0';
							fscanf(fp, "%c", &c);
						}
						//处理该文字q的索引表
						r = (iNode*)malloc(sizeof(struct iNode));//新的索引结点
						r->p_cnode = p;	//该文字本次在子句p中出现
						if (positive == 0) {                    //为负的情况 
							q->l = -num;
							r->next = (*L)->Index_List[num].firstneg;
							(*L)->Index_List[num].firstneg = r;
						}
						else {                    //为正的情况 
							q->l = num;
							r->next = (*L)->Index_List[num].firstpos;//倒序插入链表
							(*L)->Index_List[num].firstpos = r;
						}
						num = 0;
						k++;	//该子句已经处理了k个文字
					}
					fscanf(fp, "%c", &c);
					if (c == '0') q->next = NULL;//子句结束
					else {
						q->next = (lNode*)malloc(sizeof(struct lNode));
						q = q->next;
						q->exist = 1;
					}
				}
				fscanf(fp, "%c", &c);
			}
			fscanf(fp, "%c", &c);
			p->l_num = k;		//该子句有k个文字
			k = 0;
			m++;

			if (m < (*L)->clau_num) {//还有新的子句
				p->next = (cNode*)malloc(sizeof(struct cNode));
				p = p->next;
				p->flag = 0;
				p->exist = 1;
			}
			else p->next = NULL;
		}
		else fscanf(fp, "%c", &c);	//防止文件结尾有多个换行 
	}
	fclose(fp);
	return OK;
}


int printCNF(pcnf L) {//打印整个cnf公式
	if (L->clau_num == 0) {
		printf("cnf公式为空\n");
		return OK;
	}
	pcNode p = L->firstclau;	//子句指针
	plNode q = p->firstl;	//文字指针
	printf("cnf公式变元数：%d  子句数：%d\n", L->vari_num, L->clau_num);
	int i = 1;
	int j;
	while (p) {
		if (p->exist == 0) p = p->next;
		else {
			printf("第%d句有%d个文字   ", i++, p->l_num);
			q = p->firstl;
			j = 1;
			while (q) {
				if (q->exist == 0)  q = q->next;
				else {
					printf("第%d个文字为%d   ", j++, q->l);
					q = q->next;
				}
			}
			printf("\n");
			p = p->next;
		}

	}
	return OK;
}

int loadAnswer(pcnf L, AnsList& An) {//为存储答案的顺序表分配存储空间 
	An.elem = (int*)malloc((L->vari_num + 1) * sizeof(int));//分配存储空间 
	if (!An.elem) {
		return ERROR;//分配存储空间失败 
	};
	An.length = L->vari_num + 1;//初始化线性表的长度为0 
	for (int i = 1; i < An.length; i++) {
		An.elem[i] = 0;                        //表示没有经过单子句简化赋值 
		//printf("%d  ",An.elem[i]);                           查看程序运行进度，没有必要打开 
	}
	return OK;
}
int write(char filename[80]) {   //将棋盘的初始条件写入文件
	int a[20], i = 0;
	printf("写入棋盘的初始条件，以0为结尾\n");
	scanf("%d", &a[0]);
	while (a[i] != 0) {
		i++;
		scanf("%d", &a[i]);
	}
	FILE* fp;
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return ERROR;
	}
	fprintf(fp, "ccccccBinary_Puzzle\n");
	fprintf(fp, "p cnf 36 %d\n", i + 2376);
	i = 0;
	while (a[i] != 0) {
		fprintf(fp, "%d 0\n", a[i]);
		i++;
	}
	fclose(fp);
	return OK;
}

int WriteCNF(char filename[80]) {     //将棋盘的初始条件的三个约束，变成cnf公式，写入文件 
	if (write(filename) != OK)  return FALSE;
	if (rule_1(filename) != OK) return FALSE;
	if (rule_2(filename) != OK) return FALSE;
	if (rule_3(filename) != OK) return FALSE;
	return OK;
}

int SaveSolution(int res, double time, AnsList& An, char filename[80]) {       //将求解结果写入文件 
	int i = 0;
	while (filename[i] != '\0') i++;
	filename[i - 3] = 't';    //只改变文件的扩展名 
	filename[i - 2] = 'x';
	filename[i - 1] = 't';
	FILE* fp;
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return ERROR;
	}
	fprintf(fp, "s %d\n", res);    //res是求解结果，1表示满足，0表示不满足，-1未定
	fprintf(fp, "v \n");
	for (i = 1; i < An.length; i++) {
		if (An.elem[i] == -1)  fprintf(fp, "%5d", -i);
		else fprintf(fp, "%5d", i);
		if (i % 10 == 0)  fprintf(fp, "\n");
	}
	fprintf(fp, "\nt %f ms\n", time);
	fclose(fp);
	return OK;
}

int ResWrite_2(int res, double time, AnsList& An, char filename[80]) {       //将求解结果写入文件 
	int i = 0, j;
	while (filename[i] != '\0') i++;
	filename[i - 3] = 't';    //只改变文件的扩展名 
	filename[i - 2] = 'x';
	filename[i - 1] = 't';
	FILE* fp;
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return ERROR;
	}
	fprintf(fp, "s %d\n", res);    //res是求解结果，1表示满足，0表示不满足，-1未定
	fprintf(fp, "v \n");
	for (i = 0; i < 6; i++) {
		fprintf(fp, "----------------------------\n  ");
		for (j = 1; j <= 6; j++) {
			if (An.elem[6 * i + j] == 1) fprintf(fp, "1  ");
			else fprintf(fp, "0  ");
			if (j != 6) fprintf(fp, "   ");
			else fprintf(fp, "\n");
		}
	}
	fprintf(fp, "----------------------------\n");
	fprintf(fp, "\nt %f ms\n", time);
	fclose(fp);
	return OK;
}