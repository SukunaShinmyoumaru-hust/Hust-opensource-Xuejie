/* Linear Table On Sequence Structure*/
/*---- 头文件的申明 ----*/
#include<stdio.h>
#include<stdlib.h>
#include "string.h"

/*---- 预定义 ----*/
// 定义布尔类型TRUE和FALSE
#define TRUE 1
#define FALSE 0

// 定义函数返回值类型
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAX_VERTEX_NUM 20

// 定义数据元素类型
typedef int ElemType;
typedef int status;
typedef int KeyType;
typedef enum {DG,DN,UDG,UDN} GraphKind;

//定义顶点类型，包含关键字和其他信息
typedef struct {
    KeyType key; //关键字
    char others[20]; //其他信息
} VertexType;

//定义邻接表结点类型
typedef struct ArcNode {
    int adjvex; //顶点在顶点数组中的下标
    struct ArcNode *nextarc; //指向下一个结点的指针
} ArcNode;

//定义头结点类型和数组类型（头结点和边表构成一条链表）
typedef struct VNode{
    VertexType data; //顶点信息
    ArcNode *firstarc; //指向第一条弧的指针
} VNode,AdjList[MAX_VERTEX_NUM];

//定义邻接表类型，包含头结点数组、顶点数、弧数和图的类型
typedef struct {
    AdjList vertices; //头结点数组
    int vexnum,arcnum; //顶点数和弧数
    GraphKind kind; //图的类型（有向图、无向图等）
} ALGraph;

//定义图集合类型，包含一个结构体数组，每个结构体包含图的名称和邻接表
typedef struct {
    struct {
        char name[30]="0"; //图的名称
        ALGraph G; //对应的邻接表
    }elem[30]; //图的个数
    int length; //图集合中图的数量
}Graphs;

Graphs graphs;  //图的集合的定义

/*---- 函数申明 ----*/
status isrepeat(VertexType V[]);  //判断是否有重复结点
status CreateCraph(ALGraph &G,VertexType V[],KeyType VR[][2]);  //创建
status DestroyGraph(ALGraph &G);  //销毁
status LocateVex(ALGraph G, KeyType u);  //查找
status PutVex(ALGraph &G, KeyType u, VertexType value);  //顶点赋值
status FirstAdjVex(ALGraph G, KeyType u);  //获得第一邻接点
status NextAdjVex(ALGraph G, KeyType v, KeyType w);  //获得下一邻接点
status InsertVex(ALGraph &G,VertexType v);  //插入顶点
status DeleteVex(ALGraph &G, KeyType v);  //删除顶点
status InsertArc(ALGraph &G,KeyType v,KeyType w);  //插入弧
status DeleteArc(ALGraph &G,KeyType v,KeyType w);  //删除弧
void dfs(ALGraph G , void (*visit)(VertexType),int nownode);
status DFSTraverse(ALGraph G,void (*visit)(VertexType));  //dfs遍历
void BFS(ALGraph G,void  (*visit)(VertexType),int i);
status BFSTraverse(ALGraph G,void (*visit)(VertexType));  //bfs遍历
void visit(VertexType p);  //遍历的时候调用的输出函数
int * VerticesSetLessThanK(ALGraph  G,int v,int k);  //顶点小于k的顶点集合
int ShortestPathLength(ALGraph G,int v,int w);  //顶点间的最短路径
int ConnectedComponentsNums(ALGraph G);  //图的分量
status SaveGraph(ALGraph G, char FileName[]);  //图的文件保存
status LoadGraph(ALGraph &G, char FileName[]);  //图的文件读取
void menu();  //多个图管理的菜单
void menu2();  //单个图管理的菜单
void fun01();  //多个图管理的封装函数
void fun02(ALGraph &G);  //单个图管理的封装函数

/*----  main主函数  ----*/
int main()
{
    system("color ob");
    fun01();  //封装处理函数

    return 0;
}




/*----  函数的定义 ----*/

status isrepeat(VertexType V[])  // 查找重复节点
{
    int i=0;
    int flag[1000]={0};  //设计标记数组
    while (V[i].key != -1)
    {
        if(flag[V[i].key] != 0)  // 如果有重复的结点，返回 1
        {
            return 1;
        }
        flag[V[i].key]++;  // 标记关键字，以检测重复节点
        i++;
    }
    return 0;
}

/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR*/
status CreateCraph(ALGraph &G, VertexType V[], KeyType VR[][2])
{
    if(G.vexnum != 0)  // 如果图已经创建，不能再次初始化
    {
        printf("该图已经初始化，不能再次初始化\n");
        return INFEASIBLE;
    }
    int i=0;
    int flag[100000];  // 标记每个关键字出现的位置
    int flagvr[500][500]={0};  // 标记每条边是否出现过，防止重复边和自环的出现

    memset(flag,-1,sizeof(flag));  // 标记数组初始化为 -1
    if(isrepeat(V)== 1 || V[0].key==-1 || (V[1].key ==-1 && VR[0][0]!=-1))
    {
        return ERROR;  // 如果出现空图、自环、以及重复结点等，返回错误代码
    }

    while (V[i].key!= -1)
    {
        if(i >= MAX_VERTEX_NUM)  // 如果超出节点的最大数量，返回错误代码
        {
            return ERROR;
        }
        G.vertices[i].data = V[i];  // 将节点信息存储到 vertices 数组中
        G.vertices[i].firstarc = NULL; // 初始化节点的第一个邻接点为空
        flag[V[i].key] = i;  // 标记每个节点的位置
        i++;
    }

    G.vexnum=i;   // 存储节点数量

    i=0;

    while (VR[i][0]!= -1) // 创建边
    {
        flagvr[VR[i][0]][VR[i][1]]++;  // 标记边是否出现过

        // 如果出现环和重复的边，返回错误代码
        if(VR[i][0]==VR[i][1] || (flagvr[VR[i][0]][VR[i][1]]+flagvr[VR[i][1]][VR[i][0]]) > 1)
        {
            return ERROR;
        }

        if(flag[VR[i][0]] == -1)  // 如果边连接的节点没有出现过，返回错误代码
        {
            return ERROR;
        }

        // 插入结点，使用头插法，即插入到邻接链表的前面
        ArcNode *last = G.vertices[flag[VR[i][1]]].firstarc;
        ArcNode *p = (ArcNode *) malloc(sizeof(ArcNode));
        p->adjvex = flag[VR[i][0]];
        p->nextarc = NULL;

        if(last == NULL)  // 如果是第一个邻接点，直接插入
        {
            G.vertices[flag[VR[i][1]]].firstarc = p;
            i++; // 继续下一条边的操作

        } else{  // 如果不是第一个邻接点，使用头插法进行插入
            p->nextarc = last;
            G.vertices[flag[VR[i][1]]].firstarc = p;
            i++; // 继续下一条边的操作
        }
    }

    i=0;

    while(VR[i][1]!=-1) // 创建另一条方向的边
    {
        if(flag[VR[i][1]] == -1)
        {
            return ERROR;
        }

        // 插入结点，使用头插法，即插入到邻接链表的前面
        ArcNode *last = G.vertices[flag[VR[i][0]]].firstarc;
        ArcNode *p =(ArcNode*) malloc(sizeof(ArcNode));
        p->adjvex = flag[VR[i][1]];
        p->nextarc = NULL;

        if(last == NULL)  // 如果是第一个邻接点，直接插入
        {
            G.vertices[flag[VR[i][0]]].firstarc = p;
            i++; // 继续下一条边的操作
        }
        else{ // 如果不是第一个邻接点，使用头插法进行插入
            p->nextarc = last;
            G.vertices[flag[VR[i][0]]].firstarc = p;
            i++; // 继续下一条边的操作
        }
    }

    G.arcnum=i;  // 存储边的数量
    return OK;
}

status DestroyGraph(ALGraph &G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    // 如果图不存在，返回“不可行”的错误信息
    if(G.vexnum == 0 )
    {
        return INFEASIBLE;
    }
    ArcNode *p =NULL;
    ArcNode *sub =NULL;
    int i =0;
    // 循环遍历所有的顶点
    while (i<G.vexnum)
    {
        sub = G.vertices[i].firstarc;
        // 对每个顶点，循环遍历它的每个邻接点
        while (sub)
        {
            p = sub;
            sub = sub->nextarc;
            // 删除该邻接点对应的边
            free(p);
            p = NULL;
        }
        i++;
    }
    // 重置计数器，表示图中没有顶点和边
    G.vexnum =0;
    G.arcnum =0;
    return OK;
}

int LocateVex(ALGraph G, KeyType u)
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；

{
    // 如果图不存在，返回“不可行”的错误信息
    if(G.vexnum == 0 )
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int i =0;
    // 循环遍历所有的顶点
    while(i<G.vexnum)
    {
        // 如果找到关键字值为u的顶点，返回它的位序
        if(G.vertices[i].data.key == u)
        {
            return i;
        }
        i++;
    }
    // 如果没找到关键字值为u的顶点，返回-1
    return -1;
}


//顶点赋值：函数名称是PutVex (G,u,value)；初始条件是图G存在，u是和G中顶点关键字类型相同的给定值；
//操作结果是对关键字为u的顶点赋值value；
status PutVex(ALGraph &G, KeyType u, VertexType value)
{
    // 如果图不存在，返回错误信息 INFEASIBLE
    if(G.vexnum == 0 )
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int i=0;  //用来记录下标
    int num=0; int flag=-1;  //计数和标记
    while (i<G.vexnum)
    {
        // 如果关键字不唯一，返回错误信息 ERROR
        if(value.key == G.vertices[i].data.key  && value.key != u)
        {
            printf("关键字不唯一,操作失败\n");
            return ERROR;
        }
        // 如果查找到了指定的顶点，记录其出现的次数和下标
        if(G.vertices[i].data.key == u)
        {
            num++;       //用来记录出现的次数
            flag =i;     //保存下标
        }
        i++;
    }
    // 如果未查找到指定的顶点或者查找到的次数不唯一，返回错误信息 ERROR
    if(num != 1)
    {
        printf("查找失败,无法操作\n");
        return ERROR;
    }
    // 将找到的符合条件的顶点的值修改成指定的 value 值
    G.vertices[flag].data = value;
    // 操作成功，返回 OK
    return OK;
}
//获得第一邻接点：函数名称是FirstAdjVex(G, u)；初始条件是图G存在，u是G中顶点的位序；
//操作结果是返回u对应顶点的第一个邻接顶点位序，如果u的顶点没有邻接顶点，否则返回其它表示“不存在”的信息；
int FirstAdjVex(ALGraph G, KeyType u)
{
    // 如果图不存在，返回错误信息 INFEASIBLE
    if(G.vexnum == 0 )  //图不存在
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    // 在图 G 中寻找给定关键字对应的顶点
    int i=0;  //用来计数
    while (i<G.vexnum)
    {
        if(G.vertices[i].data.key == u)
        {
            // 如果找到了顶点，则返回该顶点对应的第一邻接顶点的位序
            return G.vertices[i].firstarc->adjvex;
        }
        i++;
    }
    // 如果未找到给定关键字对应的顶点，返回信息“不存在”，即 -1
    return -1;
}

//获得下一邻接点：函数名称是NextAdjVex(G, v, w)；初始条件是图G存在，v和w是G中两个顶点的位序，v对应G的一个顶点,w对应v的邻接顶点；操作结果
// 是返回v的（相对于w）下一个邻接顶点的位序，如果w是最后一个邻接顶点，返回其它表示“不存在”的信息；
// 参数说明：G为有向图，v是源节点，w是目标节点
int NextAdjVex(ALGraph G, KeyType v, KeyType w)
{
// 如果图不存在，则返回“不存在”的信息
    if (G.vexnum == 0 )
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int i = 0;
    int flagv = -1, flagw = -1; // 用来记录v和w对应的下标
// 找到v和w在G.vertices数组中的下标
    while (i < G.vexnum)
    {
        if (G.vertices[i].data.key == v)
        {
            flagv = i;
        }
        if (G.vertices[i].data.key == w)
        {
            flagw = i;
        }
        i++;
    }
// 若找不到v或w对应的结点，返回“不存在”的信息
    if (flagv == -1 || flagw == -1)
    {
        printf("v或w对应的结点不存在\n");
        return -1;
    }
    ArcNode* p = G.vertices[flagv].firstarc;
    ArcNode* ptail = p->nextarc;
// 遍历源节点的邻接链表，找到目标节点w
    while (ptail)
    {
        if (p->adjvex == flagw)
        {
// 如果w不是最后一个邻接顶点，则返回其下一个邻接顶点的位序，否则返回“不存在”的信息
            return ptail->adjvex;
        }
        p = ptail;
        ptail = p->nextarc;
    }
    return -1;
}

//插入顶点：函数名称是InsertVex(G,v)；初始条件是图G存在，v和G中的顶点具有相同特征；操作结果是在图G中增加新顶点v。
// （在这里也保持顶点关键字的唯一性）
// 参数说明：G为有向图，v为要插入的结点
status InsertVex(ALGraph& G, VertexType v)
{
// 如果图不存在，则返回“不存在”的信息
    if (G.vexnum == 0)
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int i = 0;  //记录下标
// 如果图中顶点数量已达到最大限制，则返回ERROR
    if (G.vexnum == MAX_VERTEX_NUM)
    {
        printf("超出所能容纳的最大顶点管理空间\n");
        return ERROR;
    }
// 查找图中是否已有KEY相同的结点
    while (i < G.vexnum)
    {
        if (G.vertices[i].data.key == v.key)
        {
            printf("关键字不唯一\n");
            return ERROR;
        }
        i++;
    }
// 在G.vertices数组的最后一个位置插入新结点，更新G.vexnum
    G.vertices[G.vexnum].data = v;
    G.vertices[G.vexnum].firstarc = NULL;
    G.vexnum++;
    return OK;
}

//删除顶点：函数名称是DeleteVex(G,v)；初始条件是图G存在，v是和G中顶点关键字类型相同的给定值；
// 操作结果是在图G中删除关键字v对应的顶点以及相关的弧
status DeleteVex(ALGraph &G, KeyType v)
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
// 请在这里补充代码，完成本关任务
//若图不存在或未初始化，则返回不可行状态
    if(G.vexnum == 0 )
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
//若图中只有一个顶点，则无法删除，返回错误状态
    if(G.vexnum == 1)
    {
        printf("图中只有一个顶点，不能删除\n");
        return ERROR;
    }
    int i =0;  //标记下标
//寻找要删除的顶点
    while (i<G.vexnum)
    {
        if(G.vertices[i].data.key == v)
        {
//删除与这个顶点有关的弧
            while (G.vertices[i].firstarc){
                G.arcnum--;
                ArcNode * p = G.vertices[i].firstarc;
                G.vertices[i].firstarc = p->nextarc;
                free(p);
                p = NULL;
            }
            break;
        }
        i++;
    }
    int location = i;  //记录位置
//若要删除的顶点不存在，则返回错误状态
    if(i == G.vexnum)
    {
        printf("要删除的顶点不存在.无法操作\n");
        return ERROR;
    }
//将删除顶点之后的顶点位置全部向前移动一个位置，覆盖掉要删除的位置
    while (i<G.vexnum-1)
    {
        G.vertices[i] = G.vertices[i+1];
        i++;
    }
    G.vexnum--;
//下面还要进行与这个顶点有关的弧的删除操作，以及将所有大于要删除位置的顶点位置减一
    ArcNode * train = NULL;   //记录操作
    ArcNode * p = NULL;
    int k =0;
    while (k < G.vexnum)
    {
        train = G.vertices[k].firstarc;
        p = train;
        while (train != NULL)
        {
//找到与要删除的顶点有关的弧进行删除
            if(location == train->adjvex)
            {
                if(train == p)
                {
                    G.vertices[k].firstarc = train->nextarc;
                    train = train->nextarc;
                    free(p);
                    p = NULL;
                    continue;
                }
                p->nextarc = train->nextarc;
                p = train;
                train = p->nextarc;
                free(p);
                p=NULL;
                continue;

            }
            //将所有大于要删除位置的顶点位置减一
            if(train->adjvex > location)
            {
                train->adjvex--;
            }
            p = train;
            train = p->nextarc;

        }
        k++;
    }
//删除成功，返回操作成功状态
    return OK;
}

//插入弧：函数名称是InsertArc(G,v,w)；初始条件是图G存在，v、w是和G中顶点关键字类型相同的给定值；
//操作结果是在图G中增加弧<v,w>，如果图G是无向图，还需要增加<w,v>；
status InsertArc(ALGraph &G,KeyType v,KeyType w)
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
    if(G.vexnum == 0 )  //如果图不存在
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    if(v == w)   //如果插入的是重边
    {
        printf("插入的是重边\n");
        return ERROR;
    }
    int flagv =-1, flagw =-1;

    //找到插入点 v 和 w 的下标
    int i =0;
    while (i<G.vexnum)
    {
        if(G.vertices[i].data.key == v)
        {
            flagv =i;
        }
        if(G.vertices[i].data.key == w)
        {
            flagw =i;
        }
        i++;
    }

    //如果找不到插入点 v 或 w
    if(flagv == -1 || flagw == -1)
    {
        printf("找不到要插入的顶点\n");
        return ERROR;
    }

    ArcNode *pv = NULL;
    ArcNode *pw = NULL;

    //检查插入的是否为重复的边
    pv = G.vertices[flagv].firstarc;
    while (pv)
    {
        if(pv->adjvex == flagw)  //找到了重复的边
        {
            return ERROR;
        }
        pv = pv->nextarc;
    }

    //分别创建结构体 newv 和 neww，构建新边
    ArcNode *newv = (ArcNode *) malloc(sizeof(ArcNode));
    newv->adjvex = flagw; //邻接点下标为 w
    newv->nextarc = NULL; //下一条边为空
    if(G.vertices[flagv].firstarc != NULL) //如果 v 有边
    {
        newv->nextarc =G.vertices[flagv].firstarc; //新边指向 v 的第一条边
    }
    G.vertices[flagv].firstarc = newv; //更新头指针，即 v 的第一条边为新边

    //和上面的操作类似
    ArcNode *neww = (ArcNode *) malloc(sizeof(ArcNode));
    neww->adjvex = flagv; //邻接点下标为 v
    neww->nextarc = NULL; //下一条边为空
    if(G.vertices[flagw].firstarc != NULL) //如果 w 有边
    {
        neww->nextarc =G.vertices[flagw].firstarc; //新边指向 w 的第一条边
    }
    G.vertices[flagw].firstarc = neww; //更新头指针，即 w 的第一条边为新边

    G.arcnum++; //边数加 1
    return OK; //插入成功
}

status DeleteArc(ALGraph &G,KeyType v,KeyType w)
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    if(G.vexnum == 0 )  //图不存在
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    if(v == w)  //如果v和w相等，说明删除环，返回错误
    {
        printf("你输入的是环\n");
        return ERROR;
    }
    int i = 0;
    int flagv = -1;     //用来记录下标
    int flagw = -1;
    int sign = 0;  //用来标记是否有边

    //查找边<v,w>对应的顶点下标
    while (i<G.vexnum)
    {
        if(G.vertices[i].data.key == v)
        {
            flagv = i ;
        }
        if(G.vertices[i].data.key == w)
        {
            flagw = i ;
        }
        i++;
    }

    if(flagv == -1 || flagw == -1)   //边<v,w>不存在
    {
        printf("不存在这条边的顶点\n");
        return ERROR;
    }

    //遍历v顶点的出边
    ArcNode * getv = NULL;
    ArcNode * getw = NULL;
    ArcNode * pre = NULL;

    //查找边<v,w>对应的出边，然后删除
    getv = G.vertices[flagv].firstarc;
    pre = G.vertices[flagv].firstarc;

    while (getv)
    {
        if(getv->adjvex == flagw)  //如果找到边<v,w>
        {
            sign = 1;  //有边标记为1

            if(getv == pre)  //如果边是第一条出边
            {
                G.vertices[flagv].firstarc = getv->nextarc;  //直接将该边的下一条边作为第一条出边
                free(getv);  //释放当前边
                getv = NULL;
                pre = NULL;
                break;
            }
            else  //如果边不是第一条出边
            {
                pre->nextarc = getv->nextarc;  //将该边从前一条出边的nextarc中删掉，接上后一条边
                pre = getv;  //更新前一条边的指针到当前边
                free(getv);  //释放当前边
                getv = NULL;
                break;
            }
        }
        pre = getv;  //前指针更新为当前边
        getv = pre->nextarc;  //当前边更新为下一条出边
    }

    if(sign == 0)  //如果没有找到边，返回错误
    {
        printf("不存在这条边\n");
        return ERROR;
    }
    getw = G.vertices[flagw].firstarc;
    pre = G.vertices[flagw].firstarc;
    //如果图是无向图，还需要删除边<w,v>
    while (getw)
    {
        if(getw->adjvex == flagv)  //如果找到边<w,v>
        {
            if(getw == pre)  //如果边是第一条出边
            {
                G.vertices[flagw].firstarc = getw->nextarc;  //直接将该边的下一条边作为第一条出边
                free(getw);  //释放当前边
                getw = NULL;
                break;
            }
            else  //如果边不是第一条出边
            {
                pre->nextarc = getw->nextarc;  //将该边从前一条出边的nextarc中删掉，接上后一条边
                pre = getw;  //更新前一条边的指针到当前边
                free(getw);  //释放当前边
                getw = NULL;
                break;
            }
        }
        pre = getw;  //前指针更新为当前边
        getw = pre->nextarc;  //当前边更新为下一条出边
    }


    G.arcnum--;  //边数减1
    return OK;
}

//（11）深度优先搜索遍历：
//函数名称是DFSTraverse(G,visit())；
//初始条件是图G存在；
//操作结果是图G进行深度优先搜索遍历，
//依次对图中的每一个顶点使用函数visit访问一次，
//且仅访问一次；

//定义一个标记数组，用于标记每个顶点是否已经被遍历过
int flag11[100];

//定义一个深度优先搜索函数，并传入图G、visit函数和当前遍历的节点
void dfs(ALGraph G , void (*visit)(VertexType),int nownode)
{
    //首先访问当前节点
    visit(G.vertices[nownode].data);
    //将当前节点标记为已遍历过
    flag11[nownode] = 1;

    //遍历当前节点的所有邻接节点
    ArcNode * p = G.vertices[nownode].firstarc;
    while (p)
    {
        //如果邻接节点没有被遍历过，则递归遍历它
        if(flag11[p->adjvex] == 0)
        {
            dfs(G,visit,p->adjvex);
        }
        p = p->nextarc;
    }
}

//定义图的深度优先搜索遍历函数
status DFSTraverse(ALGraph G,void (*visit)(VertexType))
{
    //对图中每个顶点进行标记初始化
    memset(flag11,0,sizeof(flag11));
    //如果图不存在，返回INFEASIBLE（不可行）
    if(G.vexnum == 0 )
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int i ;
    //对每个未被遍历过的顶点进行深度优先搜索
    for( i=0;i<G.vexnum ;i++)
    {
        if(flag11[i] == 0)
        {
            dfs(G,visit,i);
        }
    }
    return OK;
}


//（12）广度优先搜索遍历：函数名称是BFSTraverse(G,visit())；初始条件是图G存在；
// 操作结果是图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次。
int flag12[100] ;
void BFS(ALGraph G,void  (*visit)(VertexType),int i)
{
    int head = 0,tail = 0;  //定义头指针head和尾指针tail
    int Que[100];  //一个队列Que，用于存放待遍历的顶点。
    Que[0] = i;
    while (head<=tail)
    {

        visit(G.vertices[Que[head]].data);
        ArcNode * p = G.vertices[Que[head]].firstarc;
        while (p)
        {
            if(flag12[p->adjvex] == 0)
            {

                tail++;
                Que[tail] = p->adjvex;
                flag12[p->adjvex]++;

            }
            p = p->nextarc;
        }
        head++;
    }
}

status BFSTraverse(ALGraph G,void (*visit)(VertexType))
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    memset(flag12,0,sizeof (flag12));  //将flag12数组全部置为0
    if(G.vexnum == 0 )  //图不存在
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int i ;
    for( i =0;i< G.vexnum ;i++)
    {
        if(flag12[i] == 0)
        {  //遍历所有顶点，如果该顶点未被访问，则将其标记为已访问并调用BFS函数
            flag12[i]=1;
            BFS(G,visit,i);
        }
    }
    return OK;



    /********** End **********/
}

//visit 函数

void visit(VertexType p)
{
    printf("%d %s",p.key,p.others);
}

int * VerticesSetLessThanK(ALGraph  G,int v,int k)  //函数定义，返回指针类型，输入参数包括图G、起始顶点v和距离上限k
{
    k--;  //由于是从起始点算起的距离，所以距离上限k需要减1
    if(G.vexnum == 0 )  //如果图不存在，返回NULL
    {
        printf("该图不存在或未初始化\n");
        return NULL;
    }
    int record[100] = {0};  //记录访问过的结点，初始化为0（表示未访问过）
    int i = 0;
    int flag = -1;  //flag变量初始化为-1（表示没找到起始结点）

    //遍历图的顶点，找到起始结点v，记录其位置到flag变量中
    for( ; i < G.vexnum ; i++)
    {
        if(G.vertices[i].data.key == v)
        {
            flag = i;
            break;
        }
    }
    record[flag] = 1;  //标记起始结点v已经被访问过

    if(flag == -1)  //如果未找到起始点，返回NULL
    {
        printf("找不到结点\n");
        return NULL;
    }
    static int srr[100];  //静态数组用于存储距离小于k的顶点集合
    int num = 0;  //num变量用于记录已经存储了多少个顶点
    srr[num++] = flag;  //将起始点v加入到顶点集合中
    //下面进行查找
    int Que[100][2];  //二维数组表示队列，用于存储待访问的结点及其距离
    memset(Que,0,sizeof (Que));  //初始化队列为0（表示未被访问过）
    int head = 0, tail = 0;  //队列的头和尾指针
    Que[head][0] = flag;  //起始结点v作为队列的第一个元素
    Que[head][1] = 0;  //起始结点v的距离为0

    //队列非空且队列中第一个结点距离不超过k的情况下，进行队列的遍历
    while (head <= tail && Que[head][1] != (k+1))
    {

        ArcNode * p = G.vertices[Que[head][0]].firstarc;  //获取队头元素的邻接链表
        while (p)
        {
            if(record[p->adjvex] == 0)  //如果邻接结点未被访问过
            {
                if(Que[head][1] <= (k-1))
                {
                    srr[num++] = p->adjvex;  //将邻接结点加入到顶点集合中
                }

                tail++;  //队列尾指针加1
                Que[tail][0] = p->adjvex;  //将邻接结点加入到队列中
                Que[tail][1] = Que[head][1] + 1;  //计算邻接结点距离
                record[p->adjvex]++;  //标记邻接结点已经被访问过
            }
            p = p->nextarc;  //遍历下一个邻接结点
        }
        head++;  //处理完队头结点，队头指针加1
    }
    srr[num] = -1;  //将数组以-1结尾，以便在函数外部访问到数组长度

    return srr;  //返回存储顶点集合的数组指针
}

int ShortestPathLength(ALGraph G, int v, int w)
{
    if (G.vexnum == 0) //图不存在
    {
        printf("该图不存在或未初始化\n");
        return INFEASIBLE;
    }
    int head = 0, tail = 0; // 定义队列头和尾
    int record[100] = {0}; // 记录每个节点是否被访问过
    int arr[100][2]; // 定义存储节点和距离的队列
    memset(arr, 0, sizeof(arr)); // 初始化队列
    int i = 0;
    int flag = -1; // 记录v节点的索引值
    int flagw = -1; // 记录w节点的索引值
    for (; i < G.vexnum; i++) // 遍历所有节点
    {
        if (G.vertices[i].data.key == v) // 找到v节点
        {
            flag = i;
        }
        if (G.vertices[i].data.key == w) // 找到w节点
        {
            flagw = i;
        }
    }
    if (flag == -1 || flagw == -1) // 如果v或w节点不存在
    {
        printf("没有找到v对应的结点\n");
        return INFEASIBLE;
    }
    arr[head][0] = flag; // 首个节点为v节点
    while (head <= tail) // 当队列非空时循环
    {
        ArcNode *p = G.vertices[arr[head][0]].firstarc; // 找到当前节点的第一条边

        if (G.vertices[arr[head][0]].data.key == w) // 如果找到w节点
        {
            return arr[head][1]; // 返回距离
        }
        while (p) // 遍历当前节点的所有边
        {
            if (record[p->adjvex] == 0) // 如果该节点未被访问过
            {
                tail++; // 队列尾部加入该节点
                arr[tail][0] = p->adjvex; // 存储节点
                arr[tail][1] = arr[head][1] + 1; // 存储距离
                record[arr[head][0]]++; // 标记该节点已被访问
            }
            p = p->nextarc; // 遍历下一条边
        }
        head++; // 处理下一个节点
    }
    return -1; // 如果没有找到路径，返回-1
}

// 定义一个全局数组flag16用于标记顶点是否被访问过
int flag16[100] = {0};

// 定义深度优先搜索函数dfs，其中G为图，nownode为当前节点
void dfs(ALGraph G, int nownode) {
    // 将当前节点标记为已访问
    flag16[nownode] = 1;

    // 遍历当前节点的邻接节点
    ArcNode *p = G.vertices[nownode].firstarc;
    while (p) {
        // 如果当前邻接节点未被访问，则递归调用dfs函数
        if (flag16[p->adjvex] == 0) {
            dfs(G, p->adjvex);
        }
        // 继续遍历下一个邻接节点
        p = p->nextarc;
    }
}

// 定义连通分量计数函数ConnectedComponentsNums，其中G为图
int ConnectedComponentsNums(ALGraph G) {
    // 每次使用之前要将flag16数组清空
    memset(flag16, 0, sizeof(flag16));

    int i;
    // 当图为空或未初始化时，返回0
    if (G.vexnum == 0) {
        printf("为初始化或者为空\n");
        return 0;
    }

    int count = 0;
    // 遍历所有顶点
    for (i = 0; i < G.vexnum; i++) {
        // 如果当前顶点未被访问，则递归调用dfs函数，并将计数器count加1
        if (flag16[i] == 0) {
            count++;
            dfs(G, i);
        }
    }

    // 返回连通分量的计数器count
    return count;
}

status SaveGraph(ALGraph G, char FileName[])  //保存图的数据到文件
{
    if(G.vexnum ==0 )  //如果图是空的，直接返回错误
    {

        printf("图是空的\n");

        return -1;

    }
    FILE * fp = fopen(FileName,"w");  //打开文件，只可写入
    if(fp == NULL)
    {
        return ERROR;  //如果无法打开文件，返回错误
    }

    //先写入 结点数 和 边数
    fprintf(fp,"%d %d\n",G.vexnum,G.arcnum);  //写入顶点数和边数
    // 再写入顶点
    for(int k = 0;k<G.vexnum;k++)   //遍历每一个顶点
    {
        fprintf(fp,"%d %s\n",G.vertices[k].data.key,G.vertices[k].data.others);  //写入顶点的key和others
    }
    //下面输入每个结点对应的边

    for(int i = 0;i< G.vexnum ;i++)  //遍历每一个结点
    {
        ArcNode * p = G.vertices[i].firstarc;  //从顶点的第一条边开始遍历
        while (p)
        {
            fprintf(fp,"%d ",p->adjvex);  //写入边的邻接点编号
            p = p->nextarc;  //遍历下一条边
        }
        fprintf(fp,"-1\n");  //一条边结束后写入-1
    }
    fclose(fp);  //关闭文件
    return OK;  //返回成功
}

status LoadGraph(ALGraph &G, char FileName[])  //从文件中读取图的数据
{
    if(G.vexnum !=0)  //如果图不为空，则无法读取
    {
        printf("这个图不是空的，无法读取\n");
    }
    FILE *fp = fopen(FileName,"r");  //打开文件，只可读取
    if(fp == NULL)
    {
        return ERROR;  //如果无法打开文件，返回错误
    }
    fscanf(fp,"%d %d\n",&G.vexnum,&G.arcnum);  //读取顶点数和边数
    for(int i = 0;i<G.vexnum ;i++)  //遍历每一个顶点
    {
        fscanf(fp,"%d %s\n",&G.vertices[i].data.key,G.vertices[i].data.others);  //读取顶点的key和others
        G.vertices[i].firstarc = NULL;  //顶点的第一条边为NULL
    }
    for(int k = 0;k<G.vexnum ;k++)  //遍历每一个结点
    {
        ArcNode *p = G.vertices[k].firstarc;  //从顶点的第一条边开始遍历
        ArcNode * newnode = (ArcNode * ) malloc(sizeof(ArcNode));  //新建一个结点
        fscanf(fp,"%d ",&newnode->adjvex);  //读取新结点的邻接点编号
        newnode->nextarc = NULL;  //将新结点的下一条边设为NULL
        while (newnode->adjvex != -1)  //如果读取的邻接点编号不是-1
        {
            if(G.vertices[k].firstarc == NULL)  //如果当前顶点的第一条边为NULL
            {
                G.vertices[k].firstarc = newnode;  //将新结点设为该顶点的第一条边
                p = G.vertices[k].firstarc;  //令p指向该顶点的第一条边
            }
            else{
                p->nextarc = newnode;  //将新结点接到p指向的边的后面
                p = newnode;  //令p指向新结点
            }
            newnode = (ArcNode * ) malloc(sizeof(ArcNode));   //新建一个结点
            fscanf(fp,"%d ",&newnode->adjvex);  //读取新结点的邻接点编号
            newnode->nextarc = NULL;  //将新结点的下一条边设为NULL
        }

    }
    fclose(fp);  //关闭文件
    return OK;  //返回成功
}

void menu()
{
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
    printf("1.创建一个图\n");
    printf("2.删除一个图\n");
    printf("3.查询已经创建的图\n");
    printf("4.查找一个图和进行操作\n");
    printf("0.退出多个图的管理\n");
    printf("    へ　　　　　／|\n");
    printf("　　/＼7　　　 ∠＿/\n");
    printf("　 /　│　　 ／　／\n");
    printf("　│　Z ＿,＜　／　　 /`ヽ\n");
    printf("　│　　　　　ヽ　　 /　　〉\n");
    printf(" Y　　　　　`　 /　　/\n");
    printf("　?●　?　●　　??〈　　/\n");
    printf("　()　 へ　　　　|　＼〈\n");
    printf("　>? ?_　 ィ　 │ ／／\n");
    printf("　 / へ　　 /　?＜| ＼＼\n");
    printf("　 ヽ_?　　(_／　 │／／\n");
    printf("　　7　　　　　　　|／\n");
    printf("　　＞―r￣￣`?―＿\n");

    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
}

void menu2()
{
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    putchar('\n');
    printf("           Menu for Graph On Sequence Structure \n");
//	printf("-------------------------------------------------\n");
    printf("    	  1. 创建初始化图                    7. 插入顶点\n");
    printf("    	  2. 销毁图                          8. 删除顶点\n");
    printf("    	  3. 查找顶点                        9. 插入弧 \n");
    printf("    	  4. 顶点赋值                        10. 删除弧\n");
    printf("    	  5. 获取第一邻接点                  11. 深度优先搜索\n");
    printf("    	  6. 获取下一邻接点                  12. 广度优先搜索\n");
    printf("         13.查看图关系\n");
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
    printf("    	  14.距离小于k的顶点集合                15.顶点间最短路径\n");
    printf("    	  16.图的连通分量                      17.保存到文件\n");
    printf("    	  18.从文件里面加载                      \n");
    printf("    	  0.exit                        \n");
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
    //printf("一些附加的功能");

    printf("    请选择你的操作[0~13]:");
    putchar('\n');
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    putchar('\n');



    putchar('\n');

}

void fun01()        //这个函数负责多线性表的管理
{
    menu(); //调用菜单函数
    int a ; //定义整型变量a
    printf("请输入一个命令\n");
    scanf("%d",&a); //输入命令，保存在a中
    while (a) //如果a的值不为0，则循环执行代码块
    {
        fflush(stdin); //清空输入流，防止上一次操作结束后影响本次操作
        int feedback; //定义整型变量feedback
        switch (a) { //根据不同命令进行不同的操作
            case 1: //如果命令为1
                printf("现在进行创建一个新的图\n");
                printf("请输入你想创建的图的名字\n");
                char name1[30]; //定义字符串变量name1，长度为30
                scanf("%s",name1); //输入图的名字，保存在name1中
                int i ,flag ; flag = 0; //定义整型变量i和flag，flag 初始化为0
                //要进行名字的判断
                for( i =0;i<graphs.length;i++) //遍历所有已经存在的图
                {
                    if(strcmp(name1,graphs.elem[i].name)  == 0) //如果名字已经被使用，则提示创建失败
                    {
                        printf("该图已经存在，创建失败\n");
                        flag = 1; //将flag的值设为1，表示创建失败
                    }
                }
                if(flag == 0) //如果flag的值为0，表示没有相同的名字，则创建新图并保存名字
                {
                    strcpy(graphs.elem[graphs.length].name,name1); //使用strcpy函数将name1中的字符串复制到graphs.elem[graphs.length].name中
                    graphs.length++; //将已经存在的图的数量加1
                    printf("创建成功力\n");
                }
                break;
            case 2: //如果命令为2
                int flag2 ; //定义整型变量flag2
                printf("现在进行删除图的操作\n");
                printf("请输入你想删除的图的名字\n");
                char name2[30]; //定义字符串变量name2，长度为30
                scanf("%s",name2); //输入要删除的图的名字，保存在name2中

                flag2 = -1; //flag2 初始化为-1

                //要进行名字的判断
                for( i =0;i<graphs.length;i++) //遍历所有已经存在的图
                {
                    if(strcmp(name2,graphs.elem[i].name)  == 0) //如果找到了要删除的图的名字，则将flag2设置为该图在已存在图数组中的下标
                    {
                        flag2 = i;
                    }
                }

                if(flag2 == -1) //如果flag2的值还是-1，表示没有找到要删除的图的名字，则无法删除
                {
                    printf("该图不存在，无法删除\n");
                }
                else{ //如果找到了要删除的图的名字
                    if(1)
                    {
                        int k;
                        for( k = flag2 ;k < graphs.length-1 ;k++) //将该图在已存在图数组中的下标之后的所有图向前移动一个位置
                        {
                            graphs.elem[k] = graphs.elem[k+1];
                        }
                        graphs.length--; //已存在图的数量减1
                        printf("删除成功\n");
                    }
                }
                break;
            case 3: //如果命令为3
                printf("现在进行查询创建了哪些图\n");
                char name0[30]; //定义字符串变量name3，长度为30
                scanf("%s",name0); //输入要查找和操作的图的名字，保存在name3中
                int flag0 ;flag0 = -1; //定义整型变量flag3，初始化为-1
                for( i =0 ; i<graphs.length ;i++) //遍历已存在的图的数组
                {
                    if(strcmp(graphs.elem[i].name,name0) == 0) //如果找到了要查找和操作的图的名字，则将flag3设置为该图在已存在图数组中的下标
                    {
                        flag0 = i;
                    }
                }

                if(flag0 ==-1) //如果flag3还是-1，表示没有找到要查找和操作的图的名字，则提示不存在这个图
                {
                    printf("不存在这个图\n");
                    system("pause"); //暂停程序的执行，等待用户按下任意键
                }
                else { //如果找到了要查找和操作的图的名字
                    printf("第%d号位置  \n"
                           "",i); //调用fun02函数对该图进行操作
                }//
//                for(i = 0; i<graphs.length ;i++) //遍历已存在的图的数组，并按顺序输出每张图的名字
//                {
//                    printf("%d)   %s\n",i+1,graphs.elem[i].name);
//                }
                break;
            case 4: //如果命令为4
                printf("现在进行图的查找和操作\n");
                printf("请输入你想查找和操作的图的名字\n");
                char name3[30]; //定义字符串变量name3，长度为30
                scanf("%s",name3); //输入要查找和操作的图的名字，保存在name3中

                int flag3 ;flag3 = -1; //定义整型变量flag3，初始化为-1
                for( i =0 ; i<graphs.length ;i++) //遍历已存在的图的数组
                {
                    if(strcmp(graphs.elem[i].name,name3) == 0) //如果找到了要查找和操作的图的名字，则将flag3设置为该图在已存在图数组中的下标
                    {
                        flag3 = i;
                    }
                }

                if(flag3 ==-1) //如果flag3还是-1，表示没有找到要查找和操作的图的名字，则提示不存在这个图
                {
                    printf("不存在这个图\n");
                    system("pause"); //暂停程序的执行，等待用户按下任意键
                }
                else { //如果找到了要查找和操作的图的名字
                    fun02(graphs.elem[flag3].G); //调用fun02函数对该图进行操作
                }
                break;

            default: //如果命令无法识别，则提示输入错误，并重新显示菜单
                printf("输入的命令错误，请再次输入");
        }
        printf("请输入下一个命令\n");
        scanf("%d",&a); //提示输入下一个命令，保存在a中
        system("cls"); //清空控制台的输出，准备显示菜单
        menu(); //再次显示菜单
    }
}


void fun02(ALGraph &G)
{
    system("cls");     //清空屏幕
    printf("图存在鸭鸭\n");
    printf("现在对这个图进行操作\n");
    printf("别忘记初始化这个图鸭\n");
    int order;   //来接收命令
    menu2();    //展示菜单
    scanf("%d",&order);
    while (order)
    {
        fflush(stdin); //这里的清空输入流是防止上一次操作结束后输入了数据而影响本次操作
        int feedback;
        switch (order) {
            int feedback;
            case 1:
                printf("请输入顶点序列和关系对序列:\n");

                VertexType V[30];   //装顶点集合
                KeyType VR[100][2];  //装边集合
                int i,j; i =0;  //用来计数
                do {
                    scanf("%d%s",&V[i].key,V[i].others);
                } while(V[i++].key!=-1);
                i=0;
                do {
                    scanf("%d%d",&VR[i][0],&VR[i][1]);
                } while(VR[i++][0]!=-1);
                feedback = CreateCraph(graphs.elem[graphs.length-1].G,V,VR);
                if(feedback == OK)
                {
                    printf("图初始化成功\n");
                }
                else{
                    printf("初始化失败\n");
                }
                break;
            case 2:
                printf("现在进行图的销毁操作\n");
                feedback = DestroyGraph(G);
                if(feedback == OK)
                {
                    printf("图销毁成功了\n");
                }
                else{
                    printf("线性表未初始化或者不存在\n");
                }
                break;
            case 3:
                printf("现在进行查找顶点的操作\n");
                printf("请输入你想查找的顶点的关键字\n");
                int key ;     //来存储关键字
                scanf("%d",&key);
                feedback = LocateVex(G,key);
                if(feedback != -1)
                {
                    printf("所要查找的关键字为  %d  的顶点的位置序号为 %d \n",key,feedback);
                    printf("具体信息为%d  %s\n",G.vertices[feedback].data.key,G.vertices[feedback].data.others);
                } else{
                    printf("所要查找的顶点不存在\n");
                }
                break;
            case 4:
                printf("现在进行顶点赋值的操作\n");
                printf("请输入你想对哪一个关键字进行操作\n");
                int key4;      //存储关键字
                scanf("%d",&key4);
                printf("请输入你想改变的关键字和名称\n");
                VertexType value;
                scanf("%d %s",&value.key,value.others);
                feedback = PutVex(G,key4,value);
                if(feedback == OK)
                {
                    printf("操作成功\n");
                }
                break;
            case 5:
                printf("现在进行获取第一邻接点的操作\n");
                printf("输入你想操作的关键字\n");
                int key5;     //存储关键字
                scanf("%d",&key5);
                feedback =  FirstAdjVex(G,key5);
                if(feedback != -1)
                {
                    printf("获取成功,第一邻接点的位序是%d,具体信息为%d  %s",feedback,G.vertices[feedback].data.key,G.vertices[feedback].data.others);
                } else{
                    printf("操作失败\n");
                }
                break;
            case 6:
                printf("现在进行获取下一邻接点的操作\n");
                printf("请输入G中两个顶点的位序，v对应G的一个顶点,w对应v的邻接顶点\n");
                int v,w;   //来存储顶点的值
                scanf("%d %d",&v,&w);
                feedback = NextAdjVex(G,v,w);
                if(feedback != -1)
                {
                    printf("获取成功,下一邻接点的位序是%d,具体信息为%d  %s",feedback,G.vertices[feedback].data.key,G.vertices[feedback].data.others);
                } else{
                    printf("操作失败\n");
                }
                break;
            case 7:
                printf("现在进行插入顶点的操作\n");
                printf("输入你想插入的顶点的关键字和名称\n");
                VertexType v7;  //存储插入的顶点信息
                scanf("%d %s",&v7.key,v7.others);
                feedback = InsertVex(G,v7);
                if(feedback == OK)
                {
                    printf("插入成功\n");
                } else{
                    printf("插入失败\n");
                }
                break;
            case 8:
                printf("现在进行删除顶点的操作\n");
                printf("请输入你想删除的顶点的关键字\n");
                int key8;     //存储关键字
                scanf("%d",&key8);
                feedback = DeleteVex(G,key8);
                if(feedback == OK)
                {
                    printf("操作成功\n");
                }
                else{
                    printf("操作失败\n");
                }
                break;
            case 9:
                printf("现在进行插入弧的操作\n");
                int v9,w9;      //存储边的两个顶点
                printf("输入你想插入的弧\n");
                scanf("%d %d",&v9,&w9);
                feedback = InsertArc(G,v9,w9);
                if(feedback == OK)
                {
                    printf("操作成功\n");
                }
                else{
                    printf("操作失败\n");
                }
                break;
            case 10:
                printf("现在进行删除弧的操作\n");
                int v10,w10;      //存储要删除的边的两个顶点
                printf("输入你想删除的弧\n");
                scanf("%d %d",&v10,&w10);
                feedback = DeleteArc(G,v10,w10);
                if(feedback == OK)
                {
                    printf("操作成功\n");
                } else{
                    printf("操作失败\n");
                }
                break;
            case 11:
                printf("现在进行深度优先搜索\n");
                feedback = DFSTraverse(G,visit);
                if(feedback == OK)
                {
                    printf("操作成功\n");
                }
                else{
                    printf("操作失败\n");
                }
                break;
            case 12:
                printf("现在进行广度优先搜索\n");
                feedback = BFSTraverse(G,visit);
                if(feedback == OK)
                {
                    printf("操作成功\n");

                } else{
                    printf("操作失败\n");

                }
                break;
            case 13:
                int u ;  //用来计数
                for(u = 0;u< G.vexnum ;u++)
                {
                    printf("%d %s  ",G.vertices[u].data.key,G.vertices[u].data.others);
                    ArcNode * p = G.vertices[u].firstarc;
                    while (p){
                        printf(" %d ",p->adjvex);
                        p = p->nextarc;
                    }
                    putchar('\n');
                }
                break;
            case 14:
                printf("现在进行查找小于k的顶点集合的操作\n");
                printf("输入顶点的关键字\n");
                int key14;  //存储关键字
                scanf("%d",&key14);
                printf("输入距离k\n");
                int k;
                scanf("%d",&k);
                int * p;
                p = VerticesSetLessThanK(G,key14,k);

                if(p == NULL)
                {
                    printf("操作失败\n");
                }
                else{
                    printf("距离小于%d 的顶点的集合是：\n",k);
                    while ((*p) != -1)
                    {
                        printf("%d %s\n",G.vertices[*p].data.key,G.vertices[*p].data.others);

                        p++;
                    }
                    printf("操作成功\n");
                }
                break;
            case 15:
                printf("现在进行顶点间的最短路程的操作\n");
                printf("请输入顶点v和顶点w的关键字\n");
                int v15,w15;      //存储两个关键字
                scanf("%d %d",&v15,&w15);
                feedback = ShortestPathLength(G,v15,w15);
                if(feedback != -1)
                {
                    printf("最短路径为 %d \n",feedback);

                } else{
                    printf("操作失败\n");
                }
                break;
            case 16:
                printf("现在进行图的连通分量的计算\n");
                feedback = ConnectedComponentsNums(G);
                if(feedback != 0)
                {
                    printf("图的连通分量是%d\n",feedback);

                }

                break;
            case 17:
                printf("现在进行文件的保存操作\n");
                printf("请输入你想保存到哪一个文件\n");
                char name17[30];   //存储要保存的文件名
                scanf("%s",name17);
                feedback= SaveGraph(G,name17);
                if(feedback == OK)
                {
                    printf("保存成功\n");
                }
                break;
            case 18:
                printf("现在进行文件的读取操作\n");
                printf("你想读取哪一个文件的内容\n");
                char name18[30];  //存储要读取的文件名
                scanf("%s",name18);
                feedback = LoadGraph(G,name18);
                if(feedback == OK)
                {
                    printf("读取成功\n");
                }
                break;
            default:
                printf("命令输入有问题\n");



        }
        putchar('\n');
        printf("请输入下一个命令\n");
        scanf("%d",&order);
        system("cls");
        if(order != 0)
        {
            menu2();
        }
        else{
            menu();
        }

    }
}
