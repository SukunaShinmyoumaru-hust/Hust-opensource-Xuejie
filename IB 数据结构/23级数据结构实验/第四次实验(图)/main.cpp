#define _CRT_SECURE_NO_WARNINGS
#include"fun.h"
void main(void) {
	ALGraph G[10];//十个图
	int k = 0;//多个图管理
	int i = 0;//用于遍历
	KeyType u,w;//用于涉及输入关键字的函数功能
	VertexType value;//用于涉及输入value的函数
	int l,l2;//用于涉及返回整数的函数功能
	int length;//用于功能13
	VertexType V[21]; //用于创建图
	KeyType VR[100][2];
	int disk[21] = { 0 };//最短路径
	char FileName[] = "C:\\Users\\HUAWEI\\Documents\\图读写测试.txt";
	int op = 1;
	for (int i = 0; i < 10; i++)
	{
		G[i].vexnum = 0;
		G[i].arcnum = 0;
	}
	while (op) {
		system("cls");	printf("\n\n");
		printf("      Menu for Graphs based on adjacency tables \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. CreateCraph       11. DFSTraverse\n");
		printf("    	  2. DestroyGraph      12. BFSTraverse\n");
		printf("    	  3. LocateVex         13. VerticesSetLessThanK\n");
		printf("    	  4. PutVex            14. ShortestPathLength\n");
		printf("    	  5. FirstAdjVex       15. ConnectedComponentsNums\n");
		printf("    	  6. NextAdjVex        16. SaveGraph\n");
		printf("    	  7. InsertVex         17. LoadGraph\n");
		printf("    	  8. DeleteVex         18. ChangeGrapth\n");
		printf("    	  9. InsertArc         19. PrintTable\n");
		printf("    	  10. DeleteArc        \n");
		printf("    	  0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("    请选择你的操作[0~19]:");
		scanf("%d", &op);
		switch (op) {
		case 1:
			if (G[k].vexnum != 0) printf("图已存在!\n");
			else {
				printf("请输入顶点序列和关系对序列:\n");
				i = 0;
				do {
					scanf("%d%s", &V[i].key, V[i].others);
				} while (V[i++].key != -1);
				i = 0;
				do {
					scanf("%d%d", &VR[i][0], &VR[i][1]);
				} while (VR[i++][0] != -1);
				if (CreateCraph(G[k], V, VR) == ERROR) printf("输入数据错，无法创建");
				else printf("创建成功!");
			}
			getchar(); getchar();
			break;
		case 2:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				DestroyGraph(G[k]);
				printf("图已销毁!\n");
			}
			getchar(); getchar();
			break;
		case 3:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要查找的关键字u:\n");
				scanf("%d", &u);
				l = LocateVex(G[k], u);
				if (l == -1) printf("不存在关键字为%d的顶点\n", u);
				else printf("该顶点为 %d %s\n", G[k].vertices[l].data.key, G[k].vertices[l].data.others);
			}
			getchar(); getchar();
			break;
		case 4:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要赋值的顶点的关键字:\n");
				scanf("%d", &u);
				l = LocateVex(G[k], u);
				if (l == -1) printf("不存在关键字为%d的顶点\n", u);
				else {
					printf("请输入要赋的值 key others:\n");
					scanf("%d %s", &value.key, value.others);
					if (PutVex(G[k], u, value) == ERROR) printf("关键字不唯一!\n");
					else printf("赋值成功!\n");
				}
			}
			getchar(); getchar();
			break;
		case 5:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要查找第一邻接点的结点的关键字:\n");
				scanf("%d", &u);
				l = LocateVex(G[k], u);
				if (l == -1) printf("不存在关键字为%d的顶点\n", u);
				else {
					l = FirstAdjVex(G[k], u);
					if (l == -1) printf("不存在第一邻接点\n");
					else printf("第一邻接点为 %d %s\n", G[k].vertices[l].data.key, G[k].vertices[l].data.others);
				}
			}
			getchar(); getchar();
			break;
		case 6:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入u,w(查找u相对w的下一邻接点)\n");
				scanf("%d%d", &u,&w);
				l = LocateVex(G[k], u);
				if (l == -1) printf("不存在关键字为%d的顶点\n", u);
				else {
					l = LocateVex(G[k], w);
					if (l == -1) printf("不存在关键字为%d的顶点\n", w);
					else {
						l = NextAdjVex(G[k], u, w);
						if (l == -1) printf("不存在下一邻接点\n");
						else printf("下一邻接点为 %d %s\n", G[k].vertices[l].data.key, G[k].vertices[l].data.others);
					}
					
				}
			}
			getchar(); getchar();
			break;
		case 7:
			printf("请输入要插入的结点的key和others:\n");
			scanf("%d %s", &value.key, value.others);
			if (InsertVex(G[k], value) == ERROR) printf("插入失败，请检查关键字或顶点数\n");
			else printf("插入成功!");
			getchar(); getchar();
			break;
		case 8:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要删除的结点的关键字:\n");
				scanf("%d", &u);
				if (DeleteVex(G[k], u) == ERROR) printf("不存在该顶点!\n");
				else printf("删除成功!\n");
			}
			getchar(); getchar();
			break;
		case 9:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要插入的边对应两个顶点的关键字:\n");
				scanf("%d%d", &u, &w);
				if (InsertArc(G[k], u, w) == ERROR) printf("插入失败!请检查顶点和边的存在性\n");
				else printf("插入成功!\n");
			}
			getchar(); getchar();
			break;
		case 10:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要删除的边对应两个顶点的关键字:\n");
				scanf("%d%d", &u, &w);
				if (DeleteArc(G[k], u, w) == ERROR) printf("删除失败!请检查顶点和边的存在性\n");
				else printf("删除成功!\n");
			}
			getchar(); getchar();
			break;
		case 11:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				DFSTraverse(G[k]);
			}
			getchar(); getchar();
			break;
		case 12:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				BFSTraverse(G[k]);
			}
			getchar(); getchar();
			break;
		case 13:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入顶点关键字和目标距离k:\n");
				scanf("%d%d", &u, &length);
				l = LocateVex(G[k], u);
				if (l == -1) printf("不存在关键字为%d的顶点\n", u);
				else if (length < 1) printf("k的值不正确\n");
				else {

					Dijkstra(G[k], l, disk);
					for (i = 0; i < G->vexnum; i++)
					{
						if (disk[i] >= 0&&disk[i]<length) printf("顶点%d %s到目标顶点的距离为%d\n", G[k].vertices[i].data.key
							, G[k].vertices[i].data.others, disk[i]);
					}
				}
			}
			getchar(); getchar();
			break;
		case 14:
			if (G[k].vexnum == 0) printf("图不存在!\n");
			else {
				printf("请输入要查找距离的两顶点的关键字:\n");
				scanf("%d%d", &u, &w);
				l = LocateVex(G[k], u);
				l2 = LocateVex(G[k], w);
				if (l== -1 ||l2  == -1) printf("顶点不存在!\n");
				else {
					Dijkstra(G[k], l, disk);
					if (disk[l2] == -1) printf("不存在路径\n");
					else printf("距离为%d\n", disk[l2]);

				}
			}
			getchar(); getchar();
			break;
		case 15:
			if (G[k].vexnum == 0) printf("图为空!\n");
			else {
				printf("连通分量个数为%d", ConnectedComponentsNums(G[k]));
			}
			getchar(); getchar();
			break;
		case 16:
			if (G[k].vexnum == 0) printf("图为空!\n");
			else {
				SaveGraph(G[k], FileName);
				printf("已保存!\n");
			}
			getchar(); getchar();
			break;
		case 17:
			if (G[k].vexnum != 0) printf("图已存在!\n");
			else {
				LoadGraph(G[k],FileName);
				printf("保存成功!\n");
			}
			getchar(); getchar();
			break;
		case 18:
			printf("要切换至第几个图?\n");
			l = k;
			scanf("%d", &k);
			if (k < 0 || k>9) {
				printf("请输入1-10的数字\n");
				k = l;
			}
			else { k--; 
			printf("切换成功!\n");
			}
			getchar(); getchar();
			break;
		case 19:
			if (G[k].vexnum == 0) printf("图为空!\n");
			else PrintfGraph(G[k]);
			getchar(); getchar();
			break;
		case 0:
			break;
		}//end of switch
	}//end of while
	printf("欢迎下次再使用本系统！\n");
}//end of main()
