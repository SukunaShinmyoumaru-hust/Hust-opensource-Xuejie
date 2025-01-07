#include"fun.h"
int main(void) 
{

	LinkList L[10]; //十个线性表
	int op = 1;//用于切换功能
	int k = 0 ; int x = 0;//用于多线性表管理
	ElemType e = 0, pre = 0, next = 0; int i = 0; int j = 0;//用于函数功能测试
	char filename[] = "C:\\Users\\HUAWEI\\Documents\\线性表写入测试.txt";//读写测试文件
	for (int i = 0; i < 10; i++)//初始化为空
	{
		L[i] = NULL;
	}
	
	while (op) {
		system("cls");	printf("\n\n");//清屏并创建一个整洁的菜单
		printf("      Menu for Linear Table On Sequence Structure \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. InitList         7. LocateElem\n");
		printf("    	  2. DestroyList      8. PriorElem\n");
		printf("    	  3. ClearList        9. NextElem \n");
		printf("    	  4. ListEmpty        10. ListInsert\n");
		printf("    	  5. ListLength       11. ListDelete\n");
		printf("    	  6. GetElem          12. ListTrabverse\n");
		printf("          13.InitValue        14. reverseList\n");
		printf("          15.RemoveNthFromEnd 16. sortList\n");
		printf("          17.SaveList         18. LoadList\n");
		printf("          19.ChangeList       20. RemoveList\n");
		printf("    	  0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~20]:");
		scanf("%d", &op);
		switch (op){
		case 1:
			if (InitList(L[k]) == OK) printf("线性表创建成功！\n");
			else printf("线性表创建失败！\n");
			getchar(); getchar();
			break;
		case 2:
			if (DestroyList(L[k]) == OK) printf("线性表销毁成功!\n");
			else printf("线性表销毁失败!\n");
			getchar(); getchar();
			break;
		case 3:
			if (ClearList(L[k]) == OK) printf("线性表清除成功!\n");
			else printf("线性表清除失败!\n");
			getchar(); getchar();
			break;
		case 4:
			j = ListEmpty(L[k]);
			if (j == TRUE) printf("线性表为空!\n");
			else if (j == FALSE) printf("线性表不为空!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 5:
			j = ListLength(L[k]);
			if (j == INFEASIBLE) printf("线性表不存在!\n");
			else printf("线性表的长度为%d\n", j);
			getchar(); getchar();
			break;
		case 6:
			printf("请输入要获取元素的位置:\n");
			scanf("%d", &i);
			j = GetElem(L[k], i, e);
			if (j == OK) printf("该元素为%d,数据已保存在e中\n",e);
			else if (j == ERROR) printf("位置不合法!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 7:
			printf("请输入要查找的元素值:\n");
			scanf("%d", &e);
			j = LocateElem(L[k], e);
			if (j == ERROR) printf("不存在该元素!\n");
			else if (j == INFEASIBLE) printf("线性表不存在!\n");
			else printf("该元素的位置序号为%d", j);
			getchar(); getchar();
			break;
		case 8:
			printf("请输入要查找前驱的元素:\n");
			scanf("%d", &e);
			j = PriorElem(L[k], e, pre);
			if (j == OK) printf("该元素的前驱为%d,数据已保存在pre中\n",pre);
			else if (j == ERROR) printf("不存在前驱!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 9:
			printf("请输入要查找后继的元素:\n");
			scanf("%d", &e);
			j = NextElem(L[k],e, next);
			if (j == OK) printf("该元素的后继为%d,数据已保存在next中\n",next);
			else if (j == ERROR) printf("不存在后继!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 10:
			printf("请输入插入位置和元素:\n");
			scanf("%d%d", &i, &e);
			if (ListInsert(L[k], i, e) == OK) printf("插入成功!\n");
			else printf("插入失败!请检查输入位置和线性表存在性\n");
			getchar(); getchar();
			break;
		case 11:
			printf("请输入要删除的位置:\n");
			scanf("%d", &i);
			j = ListDelete(L[k], i, e);
			if (j == OK) printf("删除成功，数据%d已保存在e中\n", e);
			else if (j == ERROR) printf("删除位置不正确!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 12:  
			if (ListTraverse(L[k])==INFEASIBLE) printf("线性表不存在！\n");
			getchar(); getchar();
			break;
		case 13:
			if (L[k] == NULL) printf("线性表不存在!\n");
			else if (L[k]->next != NULL) printf("只允许对空线性表进行操作!\n");
			else {
				j = 1;
				printf("请连续输入元素，0代表结束输入:\n");
				scanf("%d", &e);
				while (e != 0)
				{
					ListInsert(L[k], j++, e);
					scanf("%d", &e);
				}
				printf("输入成功!");
			}
			getchar(); getchar();
			break;
		case 14:
			if (reverseList(L[k]) == OK) printf("链表逆置成功!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 15:
			printf("要删除倒数第几个结点：\n");
			scanf("%d", &i);
			j=RemoveNthFromEnd(L[k], i,e);
			if (j == INFEASIBLE) printf("线性表不存在!\n");
			else if (j == ERROR) printf("线性表为空!\n");
			else if (i<0 || i>ListLength(L[k])) printf("输入位置不正确!\n");
			else printf("元素%d删除成功", e);
			getchar(); getchar();
			break;
		case 16:
			j = sortList(L[k]);
			if (j == INFEASIBLE) printf("线性表不存在!\n");
			else if (j == ERROR) printf("线性表为空!\n");
			else printf("线性表排序成功!\n");
			getchar(); getchar();
			break;
		case 17:
			if (SaveList(L[k], filename) == OK) printf("写入成功!\n");
			else printf("线性表不存在!\n");
			getchar(); getchar();
			break;
		case 18:
			if (LoadList(L[k], filename) == OK) printf("读入成功!\n");
			else printf("线性表已存在!\n");
			getchar(); getchar();
			break;
		case 19:
			printf("请输入要切换的线性表序号:\n");
			x = k;
			scanf("%d", &k);
			if (k < 1 || k>10)
			{
				printf("请输入1-10的数字!\n");
				k = x;
			}
			else {
				k--;
				printf("切换成功!\n");
			}
			getchar(); getchar();
			break;
		case 20:
			printf("请输入要删除的线性表序号:\n");
			scanf("%d", &x);
			if (x < 1 || x>10) printf("请输入1-10的数字!\n");
			else {
				L[x - 1] = NULL;
				printf("删除成功!\n");
			}
			getchar(); getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()
