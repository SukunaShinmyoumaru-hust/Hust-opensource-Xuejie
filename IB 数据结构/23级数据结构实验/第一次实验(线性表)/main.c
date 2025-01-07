#define _CRT_SECURE_NO_WARNINGS
#include "fun.h"
int main(void) {
	Lists.elem[0].L.elem = NULL; Lists.length = 1;
	int op = 1;
	int n = 0;
	int u = 0;//默认对第一个线性表操作
	ElemType e1, e2, e3, e5, e6, e7; int f, i, m, j, k;//用于临时储存数据或进行状态判断
	char filename[80];//进行读写操作的文件名
	char listname[20];//用于多线性表命名，定位等操作
	while (op) {
		system("cls");	printf("\n\n");//清屏并创建一个整洁的菜单
		printf("      Menu for Linear Table On Sequence Structure \n");
		printf("---------------------------------------------------------\n");
		printf("    	  1. InitList          11. ListDelete\n");
		printf("    	  2. DestroyList       12. ListTrabverse\n");
		printf("    	  3. ClearList         13. MaxSubArray\n");
		printf("    	  4. ListEmpty         14. SubArrayNum\n");
		printf("    	  5. ListLength        15. SortList\n");
		printf("    	  6. GetElem           16. SaveList\n");
		printf("          7. LocateElem        17. LoadList\n");
		printf("          8. PriorElem         18. InitValue\n");
		printf("    	  9. NextElem          19. ChooseList(默认首线性表操作)\n");
		printf("          10. ListInsert       20. NameList\n");
		printf("          21.AddList           22. RemoveList\n");
		printf("          23.LocateList        24. AllListTrabverse\n");
		printf("          0.Exit\n");
		printf("---------------------------------------------------------\n");
		printf("    请选择你的操作[0~24]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			if (InitList(Lists.elem[n].L) == OK) printf("线性表创建成功！\n");
			else printf("线性表创建失败！\n");
			getchar(); getchar();
			break;
		case 2:
			if (DestroyList(Lists.elem[n].L) == OK) printf("线性表销毁成功！\n");
			else printf("线性表销毁失败！\n");
			getchar(); getchar();
			break;
		case 3:
			if (ClearList(Lists.elem[n].L) == OK) printf("线性表已清除！\n");
			else printf("线性表清除失败！\n");
			getchar(); getchar();
			break;
		case 4:
			if (ListEmpty(Lists.elem[n].L) == TRUE) printf("线性表为空！\n");
			else if (ListEmpty(Lists.elem[n].L) == FALSE) printf("线性表不为空!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 5:
			if (ListLength(Lists.elem[n].L) == INFEASIBLE) printf("线性表不存在!\n");
			else printf("线性表的长度为%d\n", ListLength(Lists.elem[n].L));
			getchar(); getchar();
			break;
		case 6:
			printf("请输入需要获取的元素位置:\n");
			scanf("%d", &i);
			if (GetElem(Lists.elem[n].L, i, e1) == INFEASIBLE) printf("线性表不存在!\n");
			else if (GetElem(Lists.elem[n].L, i, e1) == ERROR) printf("输入的位置不在线性表有效范围内!\n");
			else printf("该位置的元素值为%d，已保存在e1中\n", e1);
			getchar(); getchar();
			break;
		case 7:
			printf("请输入要查找的元素e:\n");
			scanf("%d", &e2);
			if (LocateElem(Lists.elem[n].L, e2) == INFEASIBLE) printf("线性表不存在!\n");
			else if (LocateElem(Lists.elem[n].L, e2) == ERROR)  printf("该数据不存在!\n");
			else printf("该元素在线性表中第一次出现的位置序号为%d\n", LocateElem(Lists.elem[n].L, e2));
			getchar(); getchar();
			break;
		case 8:
			ElemType e4; ElemType pre;
			printf("请输入需要查找前驱的元素:\n");
			scanf("%d", &e4);
			if (PriorElem(Lists.elem[n].L, e4, pre) == INFEASIBLE) printf("线性表不存在!\n");
			else if (PriorElem(Lists.elem[n].L, e4, pre) == ERROR) printf("前驱不存在!\n");
			else printf("%d的前驱值为%d,已保存在pre中\n", e4, pre);
			getchar(); getchar();
			break;
		case 9:
			ElemType next;
			printf("请输入需要查找后继的元素e:\n");
			scanf("%d", &e5);
			if (NextElem(Lists.elem[n].L, e5, next) == INFEASIBLE) printf("线性表不存在!\n");
			else if (NextElem(Lists.elem[n].L, e5, next) == ERROR) printf("后继不存在!\n");
			else printf("%d的后继值为%d,已保存在next中\n", e5, next);
			getchar(); getchar();
			break;
		case 10:
			printf("请输入要插入的位置（线性表第几个元素之前）和元素值\n");
			scanf("%d%d", &i, &e6);
			f = ListInsert(Lists.elem[n].L, i, e6);
			if (f == INFEASIBLE) printf("线性表不存在!\n");
			else if (f == ERROR) printf("插入位置不正确!\n");
			else printf("插入成功!\n");

			getchar(); getchar();
			break;
		case 11:
			printf("请输入要删除的元素位置(第几个元素)\n");
			scanf("%d", &i);
			m = ListDelete(Lists.elem[n].L, i, e7);
			if (m == INFEASIBLE) printf("线性表不存在!\n");
			else if (m == ERROR) printf("删除位置不正确!\n");
			else printf("删除成功!删除的数据%d已保存在e7中\n", e7);
			getchar(); getchar();
			break;
		case 12:
			j = ListTraverse(Lists.elem[n].L);
			if (j == INFEASIBLE) printf("线性表不存在！\n");
			if (j == OK && !Lists.elem[n].L.length) printf("空线性表\n");
			getchar(); getchar();
			break;
		case 13:
			if (!Lists.elem[n].L.elem) printf("线性表不存在!\n");
			else if (Lists.elem[n].L.length == 0) printf("线性表为空!\n");
			else printf("最大连续子数组和为%d\n", MaxSubArray(Lists.elem[n].L));
			getchar(); getchar();
			break;
		case 14:
			if (!Lists.elem[n].L.elem) printf("线性表不存在!\n");
			else if (Lists.elem[n].L.length == 0) printf("线性表为空!\n");
			else
			{
				printf("请输入目标和K的值:\n");
				scanf("%d", &k);
				printf("和为%d的连续子数组数目为%d\n", k, SubArrayNum(Lists.elem[n].L, k));
			}
			getchar(); getchar();
			break;
		case 15:
			j = SortList(Lists.elem[n].L);
			if (j == INFEASIBLE) printf("线性表不存在!\n");
			else if (j == ERROR) printf("线性表为空!\n");
			else printf("排序已完成!\n");
			getchar(); getchar();
			break;
		case 16:
			printf("请输入文件路径:\n");
			scanf("%s", filename);
			if (SaveList(Lists.elem[n].L, filename) == OK) printf("数据写入完成!\n");
			else printf("写入失败!\n");
			getchar(); getchar();
			break;
		case 17:
			printf("请输入文件路径:\n");
			scanf("%s", filename);
			if (LoadList(Lists.elem[n].L, filename) == OK) printf("数据读入完成!\n");
			else printf("读入失败!\n");
			getchar(); getchar();
			break;
		case 18:
			if (!Lists.elem[n].L.elem) printf("线性表不存在!\n");
			else {
				printf("请连续输入要存入线性表的值(以0结束输入):\n");
				scanf("%d", &e3);
				while (e3 != 0)
				{
					ListInsert(Lists.elem[n].L, Lists.elem[n].L.length + 1, e3);
					scanf("%d", &e3);
				}
				printf("数据已存入线性表!\n");
			}
			getchar(); getchar();
			break;
		case 19:
			printf("请选择需要操作的线性表编号(1-10):\n");
			u = n;
			scanf("%d", &n); n--;
			if (n + 1 > Lists.length)
			{
				printf("切换失败!\n");
				n = u;
			}
			else printf("切换成功!\n");
			getchar(); getchar();
			break;
		case 20:
			printf("请对当前线性表进行命名:\n");
			scanf("%s", Lists.elem[n].name);
			printf("命名成功!\n");
			getchar(); getchar();
			break;
		case 21:
			if (Lists.length > 9) printf("线性表已达10个，无法添加!\n");
			else
			{
				printf("请输入要添加的线性表名:\n");
				scanf("%s", listname);
				AddList(Lists, listname);
				printf("添加成功!\n");
			}
			getchar(); getchar();
			break;
		case 22:
			printf("请输入要删除的线性表名:\n");
			scanf("%s", listname);
			if (RemoveList(Lists, listname) == OK) printf("删除成功!\n");
			else printf("删除失败!\n");
			getchar(); getchar();
			break;
		case 23:
			printf("请输入要查找的线性表名:\n");
			scanf("%s", listname);
			f = LocateList(Lists, listname);
			if (f != 0) printf("该线性表的逻辑序号为%d\n", f);
			else printf("没有找到该线性表\n");
			getchar(); getchar();
			break;
		case 24:

			for (int i = 0; i < 10; i++)
			{
				if (Lists.elem[i].L.elem && Lists.elem[i].L.length)
				{
					printf("%s ", Lists.elem[i].name);//打印线性表名
					ListTraverse(Lists.elem[i].L);//打印数据
					printf("\n");

				}
			}
			getchar(); getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
	return 0;
}
