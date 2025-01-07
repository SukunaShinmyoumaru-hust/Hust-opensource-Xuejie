 #define _CRT_SECURE_NO_WARNINGS
#include"fun.h"
int main() {
	char filename[100] = "C:\\Users\\HUAWEI\\Documents\\二叉树读写测试.txt";//读写文件存储文件名
	TElemType definition[100];//先根遍历结点序列
	BiTree T[10];//树的线性表
	memset(T, 0, sizeof(T));//初始化
	int ans = 0, i = 0;//创建二叉树需要用到的变量
	int depth = 0;//二叉树深度
	int op = 1;//菜单选择
	KeyType key, key2; TElemType v = { 0,0 }; BiTree t = NULL, t2 = NULL;//实现查找等功能需要用到的key,value,指针
	BiTree t3 = NULL;//存储功能16的返回值
	int LR = -1;//实现插入功能方向选择
	int num = 0;//num为0，默认在第一棵树进行操作
	int j;//函数状态判断变量
	while (op) {
		system("cls");//系统函数，用于清屏
		system("color F");//界面文字颜色
		printf("\n\n");
		printf("     Menu  for  Binary  Tree  On  Binary  Linked  List \n");
		printf("------------------------------------------------------\n");
		printf("    	  1. CreateBiTree       11. PreOrderTraverse\n");
		printf("    	  2. DestroyBiTree      12. InOrderTraverse \n");
		printf("    	  3. ClearBiTree        13. PostOrderTraverse\n");
		printf("    	  4. BiTreeEmpty        14. LevelOrderTraverse\n");
		printf("    	  5. BiTreeDepth        15. MaxPathSum      \n");
		printf("    	  6. LocateNode         16. LowestCommonAncestor\n");
		printf("          7. Assign             17. InvertTree \n");
		printf("          8. GetSibling         18. SaveBiTree  \n");
		printf("          9. InsertNode         19. LoadBiTree\n");
		printf("          10.DeleteNode         20. ChangeBiTree \n");
		printf("                                             \n");
		printf("    	  0. Exit\n");
		printf("------------------------------------------------------\n");
		printf("    请选择你的操作[0~20]:");
		scanf("%d", &op);
		switch (op)
		{
		case 1:
			if (T[num]) printf("二叉树已存在!\n");
			else {
				i = 0;
				index = 0;//将i和索引index归0，否则会出现错误
				printf("请输入二叉树的先序遍历序列:\n");
				do {
					scanf("%d%s", &definition[i].key, definition[i].others);
				} while (definition[i++].key != -1);
				ans = CreateBiTree(T[num], definition);

				if (ans == OK)
				{
					printf("二叉树创建成功!\n");
					PreOrderTraverse(T[num]);
					printf("\n");
					InOrderTraverse(T[num]);
				}
				else printf("创建失败，请检查输入!\n");
			}
			getchar(); getchar();
			break;
		case 2:
			if (!T[num]) printf("二叉树不存在!\n");
			else {
				DestroyBiTree(T[num]);
				printf("二叉树已删除!\n");
			}
			getchar(); getchar();
			break;
		case 3:
			if (!T[num]) printf("二叉树不存在!\n");
			else {
				ClearBiTree(T[num]);
				printf("二叉树已清空!\n");
			}
			getchar(); getchar();
			break;
		case 4:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (BiTreeEmpty(T[num])) printf("二叉树为空!\n");
			else printf("二叉树不为空!\n");
			getchar(); getchar();
			break;
		case 5:
			depth = BiTreeDepth(T[num]);
			if (depth == 0) printf("二叉树不存在!\n");
			else if (depth == -1) printf("二叉树为空!\n");
			else printf("二叉树的深度为%d\n", depth);
			getchar(); getchar();
			break;
		case 6:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				printf("请输入要查找结点的关键字:\n");
				scanf("%d", &key);
				t = LocateNode(T[num], key);
				if (t) printf("该节点为%d,%s\n", t->data.key, t->data.others);
				else printf("未找到关键字为%d的结点\n", key);
			}
			getchar(); getchar();
			break;
		case 7:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				printf("请输入要赋值结点的关键字:\n");
				scanf("%d", &key);
				printf("请输入赋值value的数据(key others[]):\n");
				scanf("%d%s", &v.key, v.others);
				if (Assign(T[num], key, v)) printf("已完成结点赋值!\n");
				else printf("错误!请检查key的存在性或重复性\n");
			}
			getchar(); getchar();
			break;
		case 8:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				printf("请输入要查找兄弟结点的结点的关键字:\n");
				scanf("%d", &key);
				t = LocateNode(T[num], key);
				if (!t) printf("未找到关键字为%d的结点\n", key);
				else {
					t = GetSibling(T[num], key);
					if (t) printf("其兄弟节点为%d,%s\n", t->data.key, t->data.others);
					else printf("未找到兄弟结点!\n");
				}
			}
			getchar(); getchar();
			break;
		case 9:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				printf("请输入待插入结点的父结点的关键字:\n");
				scanf("%d", &key);
				printf("待插入结点作为左孩子(输入0)还是右孩子(输入1)?\n");
				scanf("%d", &LR);
				printf("请输入待插入结点的数据(key others[]):\n");
				scanf("%d%s", &v.key, v.others);
				j = InsertNode(T[num], key, LR, v);
				if (j == INFEASIBLE) printf("原二叉树已存在关键字为%d的结点，无法插入\n", v.key);
				else if (j == ERROR) printf("原二叉树中不存在关键字为%d的结点,无法插入\n", key);
				else printf("插入成功!\n");
			}
			getchar(); getchar();
			break;
		case 10:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				printf("请输入待删除的关键字:\n");
				scanf("%d", &key);
				j = DeleteNode(T[num], key);
				if (j == INFEASIBLE) printf("未找到关键字为%d的结点\n", key);
				else if (j == OK) printf("删除成功");
			}
			getchar(); getchar();
			break;
		case 11:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				PreOrderTraverse(T[num]);
			}
			getchar(); getchar();
			break;
		case 12:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				InOrderTraverse(T[num]);
			}
			getchar(); getchar();
			break;
		case 13:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				PostOrderTraverse(T[num]);
			}
			getchar(); getchar();
			break;
		case 14:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				LevelOrderTraverse(T[num]);
			}
			getchar(); getchar();
			break;
		case 15:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				printf("最大路径和为%d", MaxPathSum(T[num]));
			}
			getchar(); getchar();
			break;
		case 16:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else
			{
				printf("请输入两个结点的关键字:\n");
				scanf("%d%d", &key, &key2);
				t = LocateNode(T[num], key);
				t2 = LocateNode(T[num], key2);
				if (t && t2) {
					t3 = LowestCommonAncestor(T[num], key, key2);
					if (t3) printf("最近公共祖先为 %d,%s\n", t3->data.key, t3->data.others);
				}
				else printf("输入的关键字不都存在于二叉树中\n");

			}
			getchar(); getchar();
			break;
		case 17:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				InvertTree(T[num]);
				printf("翻转完成!\n");
			}
			getchar(); getchar();
			break;
		case 18:
			if (!T[num]) printf("二叉树不存在!\n");
			else if (T[num]->data.key == -1) printf("二叉树为空!\n");
			else {
				if (SaveBiTree(T[num], filename))printf("写入完成!\n");
				else printf("写入失败!\n");
			}
			getchar(); getchar();
			break;
		case 19:
			if (T[num]) printf("二叉树已存在!\n");
			else {
				if (LoadBiTree(T[num], filename)) printf("读入完成!\n");
				else printf("读入失败!\n");
			}
			getchar(); getchar();
			break;
		case 20:
			printf("要切换至第几个二叉树(1-10)?\n");
			scanf("%d", &num);
			num--;
			if (num < 0 || num>9)
			{
				printf("输入无效，已默认切换至第一个树\n");
				num = 0;
			}
			else printf("切换成功!\n");
			getchar(); getchar();
			break;
		case 21://层序遍历2(带回车)
			printBinaryTree(T[num]);
			getchar(); getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()
