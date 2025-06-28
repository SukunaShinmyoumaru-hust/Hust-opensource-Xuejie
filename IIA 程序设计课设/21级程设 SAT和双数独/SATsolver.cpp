#ifndef data_structure_project_2022_1
#define data_structure_project_2022_1
#include "definition.h"

void cnfopen(formula &original, char *input) {//读入CNF文件
	fp = fopen(input, "r");
	if (!fp)
		printf("cnf文件读取失败！\n");
	else {
		char c;
		int literal_read, literalnum_in_the_clause;
		int literal_read_temp[3500];//存储读入的文字
		literal *frontl = NULL;
		clause *frontc = NULL;
		while (fread(&c, sizeof(char), 1, fp))//不断读取字符，直到遇到p为止
			if (c == 'p')
				break;
		fread(&c, sizeof(char), 5, fp);
		fscanf(fp, "%d", &original.literalnum);//读入文字数
		fscanf(fp, "%d", &original.clausenum);//读入子句数
		original.currentclausenum = original.clausenum;
		original.emptyclausenum = 0;
		for (int i = 1; i <= original.clausenum; i++) {
			literalnum_in_the_clause = 0;
			clause *newclause = (clause *)malloc(sizeof(clause));//创建新子句
			if (i > 1)
				frontc->next = newclause;
			frontc = newclause;
			if (i == 1)
				original.head = newclause;//确定公式的头子句
			while (fscanf(fp, "%d", &literal_read) != EOF && literal_read)
				literal_read_temp[++literalnum_in_the_clause] = literal_read;
			if (literalnum_in_the_clause > 0) {
				if (literalnum_in_the_clause == 1)
					original.singleclausenum++;
				literal *head = (literal *)malloc(sizeof(literal));
				head->word = literal_read_temp[1];
				head->next = NULL;
				newclause->head = head;
				frontl = head;
				for (int j = 2; j <= literalnum_in_the_clause; j++) { //生成一条子句链表
					literal *newliteral = (literal *)malloc(sizeof(literal));
					newliteral->word = literal_read_temp[j];
					newliteral->next = NULL;
					frontl->next = newliteral;
					frontl = newliteral;
				}
			}
			newclause->next = NULL;
			newclause->num = literalnum_in_the_clause;
			literalnum_in_the_clause = 0;
		}
		printf("-----------------------------------------------------------\n");
		printf("cnf文件读取完成，共包含%d个文字，%d个子句，%d个单子句！\n", original.literalnum, original.clausenum,
		       original.singleclausenum);
	}
	fclose(fp);
}

void printformula(formula &original) {//遍历并输出公式
	clause *tempc = original.head;
	literal *templ = NULL;
	int clausecount = 0;
	printf("-----------------------------------------------------------\n");
	printf("cnf公式逐行显示如下，共包含%d个文字，%d个子句：\n", original.literalnum, original.clausenum);
	while (tempc->next != NULL) {
		templ = tempc->head;
		printf("(子句%d)   ", ++clausecount);
		while (templ->next != NULL) {
			printf("%d∨", templ->word);
			templ = templ->next;
		}
		printf("%d\n", templ->word);
		tempc = tempc->next;
	}
	templ = tempc->head;
	printf("(子句%d)   ", ++clausecount);
	while (templ->next != NULL) {
		printf("%d∨", templ->word);
		templ = templ->next;
	}
	printf("%d\n", templ->word);
	printf("-----------------------------------------------------------\n");
	printf("遍历完成！\n");
}

void deleteformula(formula &original) {//删除整个公式
	clause *deletec = original.head, *deletetempc;
	while (deletec != NULL) {//从头子句向后搜索
		literal *deletel = deletec->head, *deletetempl;
		while (deletel != NULL) {//从头文字向后搜索
			deletetempl = deletel->next;
			free(deletel);
			deletel = deletetempl;
		}
		deletetempc = deletec->next;
		free(deletec);
		deletec = deletetempc;
	}
	original.head = NULL;
	original.clausenum = 0;
	original.literalnum = 0;
	original.singleclausenum = 0;
	original.currentclausenum = 0;//初始化
}

int findunitclause(formula &original) {//查找单子句
	clause *find = original.head;
	if (!original.singleclausenum)//公式中没有单子句
		return 0;
	while (find != NULL) {
		if (find->num == 1)
			return find->head->word;
		find = find->next;
	}
	return 0;
}

void destroyclause(formula &original, int unitclause) { //删除所有包含某文字的子句
	clause *findc = original.head, *front;
	literal *findl = findc->head;
	while (findl != NULL && findc == original.head) {
		if (findl->word == unitclause) {
			original.head = findc->next; //删除第一个子句
			if (findc->num == 1)
				original.singleclausenum--;//单子句数量-1
			findc = findc->next;
			if (findc != NULL)
				findl = findc->head;
			else
				findl = NULL;
			original.currentclausenum--;
		} else
			findl = findl->next;
	}
	front = findc;
	if (findc != NULL)
		findc = findc->next;
	while (findc != NULL) { //遍历余下所有子句
		findl = findc->head;
		while (findl != NULL) {
			if (findl->word == unitclause) {
				front->next = findc->next;
				original.currentclausenum--;
				if (findc->num == 1)
					original.singleclausenum--;//单子句数量-1
				break;
			}
			findl = findl->next;
		}
		if (findl == NULL)
			front = front->next;
		findc = findc->next;
	}
}

void deleteclause(formula &original, int negative) { //去除子句中的负文字
	int flag;//若在某子句内找到负文字则为1，否则为0
	clause *findc = original.head;
	literal *findl = original.head->head, *front;
	while (findc != NULL) {
		flag = 0;
		findl = findc->head;
		while (findl != NULL) {
			if (findl->word == negative) {
				flag = 1;
				findc->num--;
				if (!findc->num)
					original.emptyclausenum++;
				if (findl != findc->head) {
					front = findc->head;
					while (front->next != findl)
						front = front->next;
					front->next = findl->next;
					free(findl);
					front = front->next;
					findl = front;
				} else {
					if (findl->next == NULL) {
						free(findc->head);
						findc->head = NULL;
						break;
					} else {
						findc->head = findl->next;
						free(findl);
						findl = findc->head;
					}
				}
				continue;
			}
			findl = findl->next;
		}
		if (findc->num == 1 && flag == 1)
			original.singleclausenum++;
		findc = findc->next;
	}
}

int existemptyclause(formula &original) {//判断是否存在空子句
	clause *findc = original.head;
	while (findc != NULL) {
		if (!findc->num)
			return TRUE;
		findc = findc->next;
	}
	return FALSE;
}

int isemptyformula(formula &original) {//判断公式是否为空
	if (!original.currentclausenum)
		return TRUE;
	return FALSE;
}

void copyformula(formula &temp, formula &original) {//复制整个公式，边读边写
	temp.clausenum = original.clausenum;
	temp.currentclausenum = original.currentclausenum;
	temp.literalnum = original.literalnum;
	temp.singleclausenum = original.singleclausenum;
	temp.emptyclausenum = original.emptyclausenum;//初始化，复制数据项
	clause *copyc = original.head;
	literal *copyl;//两个copy指针指在原链表上
	literal *frontl = NULL;
	clause *frontc = NULL;//两个front指针指在新链表上
	while (copyc != NULL) {
		clause *newclause = (clause *)malloc(sizeof(clause));
		newclause->num = copyc->num;
		copyl = copyc->head;
		while (copyl != NULL) {
			literal *newliteral = (literal *)malloc(sizeof(literal));
			newliteral->word = copyl->word;
			if (copyl == copyc->head) {
				newclause->head = newliteral;
				frontl = newliteral;
			} else {
				frontl->next = newliteral;
				frontl = frontl->next;
			}
			newliteral->next = NULL;
			copyl = copyl->next;
		}
		if (copyc == original.head) {
			frontc = newclause;
			temp.head = newclause;
		} else {
			frontc->next = newclause;
			frontc = frontc->next;
		}
		newclause->next = NULL;
		copyc = copyc->next;
	}
}

void addunitclause(formula &original, int key) { //在原公式中添加单子句
	clause *newclause = (clause *)malloc(sizeof(clause));
	if (original.head == NULL) {
		original.head = newclause;
		newclause->next = NULL;
	} else {
		newclause->next = original.head;
		original.head = newclause;
	}
	literal *singleliteral = (literal *)malloc(sizeof(literal));
	singleliteral->word = key;
	singleliteral->next = NULL;
	newclause->head = singleliteral;
	newclause->num = 1;
	original.currentclausenum++;//当前子句数+1
	original.singleclausenum++;//单子句数+1
}

int DPLL(formula &original, int strategy) {//DPLL算法，两种策略合一
	int unitclause = findunitclause(original), key, max = 0;
	int countposi[3500] = {0}, countnega[3500] = {0}; //分别统计正负文字出现次数
	while (unitclause) {//不断找单子句
		if (unitclause > 0)
			result[unitclause] = 1;//正文字取真
		else
			result[-unitclause] = 0; //负文字取假
		destroyclause(original, unitclause); //从公式中删除所有包含该文字的子句
		if (isemptyformula(original))
			return OK;//判断现在是否为空集
		deleteclause(original, -unitclause); //删除所有子句中的负文字
		if (existemptyclause(original)) //判断现在是否有空子句（不可满足）
			return ERROR;
		unitclause = findunitclause(original);
	}
	formula temp;
	copyformula(temp, original); //创建新公式temp用来在DPLL过程中分支
	if (strategy == 1) { //改进前策略，按照从前到后的顺序选取变元key
		key = original.head->head->word;
		addunitclause(original, key);//在原公式中加入新变元
		addunitclause(temp, -key);//在原公式中加入负的新变元
	} else if (strategy == 2) { //改进后策略，选取出现次数最多的key
		clause *searchc = original.head;
		literal *searchl;
		while (searchc != NULL) {
			searchl = searchc->head;
			while (searchl != NULL) {
				if (searchl->word > 0)
					countposi[searchl->word]++;
				else
					countnega[-searchl->word]++;
				searchl = searchl->next;
			}
			searchc = searchc->next;
		}
		for (int i = 1; i <= original.literalnum; i++)
			if (max < countposi[i]) {
				key = i;
				max = countposi[i];
			}
		if (!max)//不存在正文字，所以找出现次数最多的负文字
			for (int i = 1; i <= original.literalnum; i++)
				if (max < countnega[i]) {
					key = i;
					max = countnega[i];
				}
		addunitclause(original, key);//在原公式中加入新变元
		addunitclause(temp, -key);//在原公式中加入负的新变元
	}
	if (DPLL(original, strategy))//分裂策略
		return OK;
	else
		return DPLL(temp, strategy);
}

void saveres(char *output, formula &original, int time, int DPLLstatus) {//将结果输出到.res文件
	if ((fp = fopen(output, "w")) == NULL)
		printf("保存失败！\n");
	else {
		fprintf(fp, "s %d\nv ", DPLLstatus);
		for (int i = 1; i <= original.clausenum; i++) {
			if (result[i] > 0)
				fprintf(fp, "%d ", i);
			else
				fprintf(fp, "-%d ", i);
		}
		fprintf(fp, "\n");
		fprintf(fp, "t %d", time);
	}
	fclose(fp);
}
#endif