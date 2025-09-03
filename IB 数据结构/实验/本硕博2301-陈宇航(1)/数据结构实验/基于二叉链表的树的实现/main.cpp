/* Binary Trees Structure*/
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

// 定义数据元素类型
typedef int status;
typedef int KeyType;

// 二叉树结点数据类型定义
typedef struct {
    KeyType key; // 结点关键字
    char others[20]; // 数据
} TElemType;

// 二叉链表结点的定义
typedef struct BiTNode {
    TElemType data; // 结点数据
    struct BiTNode *lchild, *rchild; // 左右子树指针
} BiTNode, *BiTree;

// 二叉树的集合类型定义
typedef struct {
// 元素的集合
    struct {
        char name[30]; // 标识元素的名称
        BiTree T; // 二叉树
    } elem[10]; // 最多存储 10 个元素
    int length; // 元素个数
} LISTS;

LISTS Lists; // 二叉树集合的定义 Lists


/*---- 函数申明 ----*/
status CreateBiTree(BiTree &T, TElemType definition[]);  //创建
status DestroyBiTree(BiTree &T);  //销毁
status ClearBiTree(BiTree &T);  //清空
status BiTreeEmpty(BiTree &T);  //判空
int BiTreeDepth(BiTree T);   //求深度
BiTNode* LocateNode(BiTree T, KeyType e);   //查找结点
status Assign(BiTree &T, KeyType e, TElemType value);  //结点赋值
BiTNode* GetSibling(BiTree T, KeyType e);  //获得兄弟结点
status InsertNode(BiTree &T, KeyType e, int LR, TElemType c);  //插入结点
BiTree findrightTNode(BiTree T);  //找到右子树
status DeleteNode(BiTree &T, KeyType e);  //删除结点
void visit(BiTree T);  //遍历中调用的访问函数
status PreOrderTraverse(BiTree T, void(*visit)(BiTree));  //前序遍历
status InOrderTraverse(BiTree T, void(*visit)(BiTree));  //中序遍历
status PostOrderTraverse(BiTree T, void(*visit)(BiTree));  //后续遍历
status LevelOrderTraverse(BiTree T, void(*visit)(BiTree));  //层序遍历
status SaveBiTree(BiTree T, char FileName[]);  //保存到文件
status LoadBiTree(BiTree &T, char FileName[]);  //从文件中加载
int MaxPathSum(BiTree T);  //最大路径和
BiTree LowestCommonAncestor(BiTree T, int e1, int e2);  //最近公共祖先
BiTree InvertTree(BiTree T);  //翻转二叉树
void menufirst();  //管理多个图的菜单
void menu();  //管理单个图的菜单
void fun01();  //管理多个图的封装函数
void fun02(BiTree & T );    //管理单个图的封装函数

/*---- main主函数 ----*/
int main()
{
    system("color ob");//设置颜色
    fun01();
}




/*----  函数定义  ----*/

status CreateBiTree(BiTree &T, TElemType definition[])
{
    /*
     * 根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，
     * 将根节点指针赋值给T并返回OK，如果有相同的关键字，返回ERROR。
     */

    static int i = 0; // 静态变量，记录当前已经处理到的序列下标
    int j = 0, k = 0; // 循环计数器

    // 第一次调用时，检查数据是否合法
    if (i == 0) {
        // 依次检查每个节点关键字是否合法
        for (j = 0; (definition + j)->key != -1; j++) {
            for (k = j + 1; (definition + k)->key != -1; k++) {
                // 如果有两个关键字相同，且不为0，返回错误
                if ((definition + j)->key == (definition + k)->key && (definition + j)->key != 0)
                    return ERROR;
            }
        }
    }

    // 递归出口：序列遍历结束，T为空值，返回OK
    if ((definition + i)->key == -1) {
        T = NULL;
        i = 0;
        return OK;
    }

    // 如果当前节点为0，表示空结点，无需创建二叉树结点，i自增并返回OK
    if ((definition + i)->key == 0) {
        i++;
        return OK;
    }

    // 创建二叉树结点，分别处理其左右子树，递归构建整棵二叉树
    T = (BiTree)malloc(sizeof(BiTNode));
    T->lchild = NULL;
    T->rchild = NULL;
    T->data = *(definition + i);
    i++;
    CreateBiTree(T->lchild, definition);
    CreateBiTree(T->rchild, definition);
    return OK;
}

status DestroyBiTree(BiTree &T)
{//将二叉树设置成空，并删除所有结点，释放结点空间

    if (T != NULL) // 如果这个二叉树不为空
    {
        if (T->lchild) // 如果左子树不为空
            DestroyBiTree(T->lchild); // 递归地销毁左子树（因为左子树也是一棵二叉树）

        if (T->rchild) // 如果右子树不为空
            DestroyBiTree(T->rchild); // 递归地销毁右子树（因为右子树也是一棵二叉树）

        free(T); // 释放当前节点的空间（因为当前节点的左右子树已经被销毁了）

        T = NULL; // 将当前节点的指针设置为NULL，表示这个节点已经被销毁了
    }

    return OK; // 返回操作成功
}

// 初始条件是二叉树 T 存在；操作结果是将二叉树 T 清空
status ClearBiTree(BiTree &T)
{
// 如果二叉树 T 不为空，则需要清空它；否则直接返回 OK
    if (T != NULL)
    {
// 如果 T 的左子树不为空，则递归清空左子树
        if (T->lchild)
            DestroyBiTree(T->lchild);

        // 如果 T 的右子树不为空，则递归清空右子树
        if (T->rchild)
            DestroyBiTree(T->rchild);

        // 释放 T 的内存空间，并将 T 的指针设为 NULL
        free(T); // 使用递归依次释放左子树、右子树、根节点指针
        T = NULL;
    }

// 返回函数执行结果
    return OK;
}

status BiTreeEmpty(BiTree &T)
{
    //初始条件是二叉树T存在；操作结果是若T为空二叉树则返回TRUE，否则返回FALSE
    if (T != NULL)
        return FALSE;
    else
        return TRUE;
}

int BiTreeDepth(BiTree T)
{
    //求二叉树T的深度

    int depth = 0;  //定义变量 depth 并初始化为 0

    if (T != NULL)
    {
        int lchilddepth, rchilddepth;
        //递归求解左子树的深度
        lchilddepth = BiTreeDepth(T->lchild);
        //递归求解右子树的深度
        rchilddepth = BiTreeDepth(T->rchild);

        //取左右子树深度较大值并将其加 1，即为当前节点所在子树的深度
        if (lchilddepth >= rchilddepth)
            depth = lchilddepth + 1;
        else
            depth = rchilddepth + 1;
    }

    return depth;   //返回当前节点所在子树的深度
}

BiTNode* LocateNode(BiTree T, KeyType e)
{//查找结点
    if (T == NULL) // 如果树为空，则返回空指针
        return NULL;
    BiTree st[100], p; // 定义一个栈 st 和当前遍历的结点 p
    int top = 0; // top 表示栈顶指针
    st[top++] = T; // 将根节点入栈
    while (top != 0) // 当栈不为空时
    {
        p = st[--top]; // 取出栈顶元素
        if (p->data.key == e) // 若当前结点的值等于 e，则返回当前结点
            return p;
        if (p->rchild != NULL) // 若当前结点的右子树不为空，则将右子树入栈
            st[top++] = p->rchild;
        if (p->lchild != NULL) // 若当前结点的左子树不为空，则将左子树入栈
            st[top++] = p->lchild;
    }
    return NULL; // 如果未找到结点，则返回空指针
}

//函数功能：给二叉树中某个结点赋值
status Assign(BiTree &T, KeyType e, TElemType value)
{
//判断二叉树是否存在
    if (T == NULL)
    {
        printf("二叉树不存在\n");
        return ERROR;
    }

    int flag = 0;//用于标记是否找到了目标结点
    BiTree st[100], p;//定义一个数组作为栈，一个指针用于遍历二叉树
    int top = 0;//栈顶指针
    st[top++] = T;//将二叉树根节点入栈

//循环遍历二叉树
    while (top != 0)
    {
        p = st[--top];//弹出栈顶元素

        //判断插入的结点关键字是否和二叉树中的其他结点重复
        if (p->data.key == value.key && e != value.key)
        {
            printf("关键字重复\n");
            return ERROR;
        }

        //找到了目标结点
        if (p->data.key == e)
        {
            p->data = value;//将目标结点的数据修改为新的数据
            flag = 1;//标记为已找到
        }

        //遍历左右子树
        if (p->rchild != NULL)
            st[top++] = p->rchild;//右子树入栈
        if (p->lchild != NULL)
            st[top++] = p->lchild;//左子树入栈
    }

//判断是否成功修改了结点数据
    if(flag)
    {
        return OK;
    }
    return ERROR;
}

status InsertNode(BiTree &T, KeyType e, int LR, TElemType c)
// 参数说明：
// T：待插入结点的二叉树
// e：待插入结点的父节点关键字
// LR：待插入结点的左孩子还是右孩子，当LR为-1时作为根结点插入
// c：待插入结点
{
    BiTree t;  // 定义一个二叉树结点t
    int top = 0;  // 栈顶指针初始化为0，这个变量好像没用到
    if (LR == -1)  // 如果待插入结点要插入为根结点
    {
        t = (BiTree)malloc(sizeof(BiTNode));  // 动态分配内存并定义为二叉树结点
        t->rchild = T;  // 将原有的二叉树T挂在新结点的右子树上
        t->lchild = NULL;  // 新结点的左子树为空
        t->data = c;  // 新结点的数据域为待插入数据c
        T = t;  // 原有的二叉树T被替换为新结点t
        return OK;  // 插入成功
    }
    if (T == NULL)
        return ERROR;  // 如果待插入的二叉树为空，则返回错误
    BiTree q = (BiTree)malloc(sizeof(BiTNode));  // 动态分配内存并定义为二叉树结点
    q->lchild = q->rchild = NULL;  // 左右子树均为空
    q->data = c;  // 新结点的数据域为待插入数据c
    if(LocateNode(T,c.key) != NULL)  // 如果新结点的关键字已经存在于T中，则返回错误
    {
        printf("关键字重复\n");
        return ERROR;
    }
    BiTree p = LocateNode(T, e);  // 定位待插入结点的父结点
    if (!p)  // 如果找不到父结点，则返回错误
        return ERROR;
    else{
        if (LR) {  // 如果要插入的结点为父节点的右孩子
            q->rchild = p->rchild;  // 将父节点的右子树挂在新结点的右子树上
            p->rchild = q;  // 将新结点挂在父节点的右子树上
            return OK;  // 插入成功
        }
        if (!LR) {  // 如果要插入的结点为父节点的左孩子
            q->rchild = p->lchild;  // 将父节点的左子树挂在新结点的右子树上
            p->lchild = q;  // 将新结点挂在父节点的左子树上
            return OK;  // 插入成功
        }
    }
}

// 找到二叉树中最右边的结点
BiTree findrightTNode(BiTree T)
{
// 创建一个栈来保存已遍历的结点
    BiTree stack[1000], p = NULL;
// 定义栈的顶部指针为0
    int top = 0;
// 如果二叉树不为空
    if (T != NULL)
    {
// 把二叉树的根节点放入栈中
        stack[top++] = T;
// 循环，直到栈为空
        while (top)
        {
// 取出栈顶元素，并把该元素赋给p
            p = stack[--top];
// 将p的右子树放入栈中
            if (p->rchild != NULL)
                stack[top++] = p->rchild;
// 将p的左子树放入栈中
            if (p->lchild != NULL)
                stack[top++] = p->lchild;
        }
    }
// 返回最后一个遍历到的结点
    return p;
}

//删除结点
status DeleteNode(BiTree &T, KeyType e)
{
    if (T == NULL) //如果T是空树，就无法删除，返回错误代码
        return INFEASIBLE;

    BiTree stack[1000], p, lp, TNode; //定义栈，以及三个指针
    int top = 0; //栈顶

    if (T != NULL) //如果T不为空树
    {
        if (T->data.key == e) //如果T的根结点就是要删除的结点
        {
            lp = T; //记下要删除的结点
            if (T->lchild == NULL && T->rchild == NULL) //如果要删除的结点没有左右子树
            {
                free(lp); //直接释放内存
                T = NULL; //将根结点设为NULL
                return OK; //返回成功操作
            }
            else if (T->lchild != NULL && T->rchild == NULL) //如果要删除的结点只有左子树
            {
                T = T->lchild; //将根结点的左子树变成新的根结点
                free(lp); //释放内存
                return OK; //返回成功操作
            }
            else if (lp->lchild == NULL && lp->rchild != NULL) //如果要删除的结点只有右子树
            {
                T = T->rchild; //将根结点的右子树变成新的根结点
                free(lp); //释放内存
                return OK; //返回成功操作
            }
            else //如果要删除的结点既有左子树又有右子树
            {
                TNode = findrightTNode(T->lchild); //找到要删除的结点的左子树的最右结点
                TNode->rchild = T->rchild; //将要删除的结点的右子树挂在左子树的最右结点下
                T = T->lchild; //将左子树作为新的树
                free(lp); //释放内存
                return OK; //返回成功操作
            }
        }
        stack[top++] = T; //根结点入栈
        while (top) //如果栈还有元素
        {
            p = stack[--top]; //栈顶元素出栈
            if (p->rchild != NULL) //如果栈顶元素有右子树
            {
                if (p->rchild->data.key == e) //如果栈顶元素的右子树就是要删除的结点
                {
                    lp = p->rchild; //记下要删除的结点
                    if (lp->lchild == NULL && lp->rchild == NULL) //如果要删除的结点没有左右子树
                    {
                        free(lp); //直接释放内存
                        p->rchild = NULL; //将父结点的右子树设为NULL
                        return OK; //返回成功操作
                    }
                    else if (lp->lchild != NULL && lp->rchild == NULL) //如果要删除的结点只有左子树
                    {
                        p->rchild = lp->lchild;
                        free(lp);
                        return OK; //返回成功操作
                    }
                    else if (lp->lchild == NULL && lp->rchild != NULL) //如果要删除的结点只有右子树
                    {
                        p->rchild = lp->rchild;
                        free(lp);
                        return OK; //返回成功操作
                    }
                    else //如果要删除的结点既有左子树又有右子树
                    {
                        TNode = findrightTNode(lp->lchild); //找到要删除的结点的左子树的最右结点
                        TNode->rchild = lp->rchild; //将要删除的结点的右子树挂在左子树的最右结点下
                        p->rchild = lp->lchild; //将左子树作为父结点的右子树
                        free(lp);
                        return OK; //返回成功操作
                    }
                }
                stack[top++] = p->rchild; //右子树入栈
            }
            if (p->lchild != NULL) //如果栈顶元素有左子树
            {
                if (p->lchild->data.key == e) //如果栈顶元素的左子树就是要删除的结点
                {
                    lp = p->lchild; //记下要删除的结点
                    if (lp->lchild == NULL && lp->rchild == NULL) //如果要删除的结点没有左右子树
                    {
                        free(lp); //直接释放内存
                        p->lchild = NULL; //将父结点的左子树设为NULL
                        return OK; //返回成功操作
                    }
                    else if (lp->lchild != NULL && lp->rchild == NULL) //如果要删除的结点只有左子树
                    {
                        p->lchild = lp->lchild;
                        free(lp);
                        return OK; //返回成功操作
                    }
                    else if (lp->lchild == NULL && lp->rchild != NULL) //如果要删除的结点只有右子树
                    {
                        p->lchild = lp->rchild;
                        free(lp);
                        return OK; //返回成功操作
                    }
                    else //如果要删除的结点既有左子树又有右子树
                    {
                        TNode = findrightTNode(lp->lchild); //找到要删除的结点的左子树的最右结点
                        TNode->rchild = lp->rchild; //将要删除的结点的右子树挂在左子树的最右结点下
                        p->lchild = lp->lchild; //将左子树作为父结点的左子树
                        free(lp);
                        return OK; //返回成功操作
                    }
                }
                stack[top++] = p->lchild; //左子树入栈
            }
        }
    }
    return ERROR; //返回错误操作
}

//一个简单的输出函数
void visit(BiTree T)
{
    if(T->data.key!=-1)
    printf(" %d,%s", T->data.key, T->data.others);
}

// 先序遍历二叉树T
status PreOrderTraverse(BiTree T, void(*visit)(BiTree))
{
// 如果T是空树，直接返回
    if (T == NULL)
        return OK;
// 定义一个栈st，同时定义栈的指针top和一个指针p
    BiTree st[100], p;
    int top = 0;
// 根节点先入栈
    st[top++] = T;
// 当栈不为空时，循环遍历
    while (top != 0)
    {
// 指针p指向栈顶元素
        p = st[--top];
// 对p进行访问
        visit(p);
// 如果p右子树不为空，右子树先入栈
        if (p->rchild != NULL)
            st[top++] = p->rchild;
// 如果p左子树不为空，左子树后入栈，保证左子树可以先遍历
        if (p->lchild != NULL)
            st[top++] = p->lchild;
    }
// 遍历完成，返回OK
    return OK;
}

//中序遍历二叉树T
status InOrderTraverse(BiTree T, void(*visit)(BiTree))
{
// 如果T为空，则直接返回
    if (T == NULL)
        return OK;
// 如果T非空，则进行遍历操作
    if (T != NULL)
    {
// 对T的左子树进行遍历
        if (InOrderTraverse(T->lchild, visit))
        {
// 对T进行访问操作
            visit(T);
// 对T的右子树进行遍历
            if (InOrderTraverse(T->rchild, visit))
// 如果遍历成功，则返回OK，表示遍历操作成功
                return OK;
        }
// 如果左子树或右子树遍历失败，则返回ERROR，表示遍历操作失败
        return ERROR;
    }
// 如果T为空，则直接返回
    else
        return OK;
}

// 后序遍历二叉树T
status PostOrderTraverse(BiTree T, void(*visit)(BiTree))
{
// 如果二叉树为空，则遍历结束
    if (T == NULL)
        return OK;
// 如果二叉树非空
    if (T != NULL)
    {
// 递归遍历左子树，如果左子树遍历成功
        if (PostOrderTraverse(T->lchild, visit)) {
// 递归遍历右子树，如果右子树遍历成功
            if (PostOrderTraverse(T->rchild, visit)) {
// 访问当前结点
                visit(T);
                return OK;
            }
        }
// 如果左子树或右子树遍历失败，则返回失败
        return 0;
    }
    else
        return OK;
}

//层序遍历
status LevelOrderTraverse(BiTree T, void(*visit)(BiTree))
{//按层遍历二叉树T
    if (T == NULL) //如果输入的二叉树为空树，直接返回
        return OK;
    BiTree st[200], p; //定义一个数组，数组成员为BiTree类型，和一个结点p
    int front = 0, rear = 0; //定义两个变量，front代表队列开头，rear代表队列结尾
    st[rear++] = T; //将输入的二叉树的根节点放入数组的第一个结点
    do //使用循环实现遍历
    {
        p = st[front++]; //将队列开头的元素赋值给变量p，并更新队列开头
        visit(p); //对当前结点进行遍历
        if (p->lchild != NULL) //如果当前结点有左孩子，将其放入队尾
            st[rear++] = p->lchild;
        if (p->rchild != NULL) //如果当前结点有右孩子，将其放入队尾
            st[rear++] = p->rchild;
    } while (rear != front); //队列仍有结点，继续循环
    return OK; //遍历完成，返回状态码
}

//将二叉树的结点数据写入到文件FileName中
status SaveBiTree(BiTree T, char FileName[]) {
    if (T == NULL) //二叉树未创建，则操作不能完成
        return INFEASIBLE;
    FILE *fp = fopen(FileName, "w"); //以写入方式打开文件
    if (fp == NULL)
        return ERROR; //文件指针打不开，错误
    BiTree st[100]; //定义数组模拟栈
    int mark[100], p = 0; //记录每个结点的状态，p为栈顶指针
    // 初始化栈顶指针
    st[0] = T, mark[0] = 0;
    while (p != -1) { //栈非空则继续遍历
        if (mark[p] == 0) { //第一次访问该结点
            //将结点数据写入文件
            fprintf(fp, "%d %s ", st[p]->data.key, st[p]->data.others);
            mark[p]++; //将状态置为已访问左子树
            if (st[p]->lchild == NULL)
                fprintf(fp, "%d null ", 0); //如果左子树为空，写入null
            else {
                st[p + 1] = st[p]->lchild; //否则将左子树结点入栈
                p++; //指针后移
                mark[p] = 0; //新结点状态初始化
            }
        }
        else if (mark[p] == 1) { //第二次访问该结点
            mark[p]++; //状态置为已访问右子树
            if (st[p]->rchild == NULL)
                fprintf(fp, "%d null ", 0); //如果右子树为空，写入null
            else {
                st[p + 1] = st[p]->rchild; //否则将右子树结点入栈
                p++; //指针后移
                mark[p] = 0; //新结点状态初始化
            }
        }
        else if (mark[p] == 2) { //第三次访问该结点
            mark[p] = 0; //状态置为未访问
            st[p] = NULL; //将该结点出栈
            p--; //指针前移
        }
    }
    // 写入结束标志符
    fprintf(fp, "%d null", -1);
    fclose(fp); //关闭文件指针
    return OK;
}

//该函数用于读取文件中的数据，创建一颗二叉树
status LoadBiTree(BiTree &T, char FileName[])
{
    //如果传入的二叉树已经存在，则无法进行操作
    if (T != NULL)
    {
        printf("二叉树已经初始化，无法操作\n");
        return INFEASIBLE;
    }

    //尝试打开文件
    FILE *fp = fopen(FileName, "r");
    if (fp == NULL)
        return ERROR;//文件指针打不开，返回错误
    TElemType definitionfile[100];//定义结构体类型数组，用于存储读取出来的值
    BiTree st[100];//定义指向节点的指针数组
    int mark[100], p = 0;//定义一个标记数组，和一个标记位置的指针p，并初始化为0

    //读入文件中的数据，存储到definitionfile[]数组中
    int t = -1;
    do {
        t++;
        fscanf(fp, "%d%s", &definitionfile[t].key, definitionfile[t].others);
    } while (definitionfile[t].key != -1);

    //判断文件中的第一个结点是否为NULL，如果不是则创建根节点
    if (definitionfile[0].key != -1) {
        T = st[0] = (BiTNode*)malloc(sizeof(BiTNode)), mark[0] = 0;
        st[0]->data = definitionfile[t++];
    }
    else
        return INFEASIBLE;//返回错误

    t = 0;
    //如果文件中的第一个结点不是NULL，则开始循环建立树形结构
    while (definitionfile[t].key != -1) {
        //如果标记位置上的值为0，则说明需要在该节点的左子节点插入新节点
        if (mark[p] == 0)
        {
            mark[p]++;
            //如果该节点的左子节点为NULL，则Mark数组位置+1，插入新节点
            if (definitionfile[t].key == 0)
                st[p]->lchild = NULL;
            else
            {
                st[p]->lchild = (BiTNode*)malloc(sizeof(BiTNode));
                //将新节点指向的位置的值赋值为definitionfile[t]的值，Mark数组位置+1
                st[p + 1] = st[p]->lchild;
                p++;
                st[p]->data = definitionfile[t];
                mark[p] = 0;
            }
            t++;
        }
            //如果标记位置上的值为1，则说明需要在该节点的右子节点插入新节点
        else if (mark[p] == 1)
        {
            mark[p]++;
            //如果该节点的右子节点为NULL，则Mark数组位置+1，插入新节点
            if (definitionfile[t].key == 0)
                st[p]->rchild = NULL;
            else {
                st[p]->rchild = (BiTNode*)malloc(sizeof(BiTNode));
                //将新节点指向的位置的值赋值为definitionfile[t]的值，Mark数组位置+1
                st[p + 1] = st[p]->rchild;
                p++;
                st[p]->data = definitionfile[t];
                mark[p] = 0;
            }
            t++;
        }
            //如果标记位置上的值为2，则说明该节点的左右子节点都创建好了，需要回退到上一级节点
        else if (mark[p] == 2)
        {
            mark[p] = 0;//重置Mark数组位置上的值为0
            st[p] = NULL;//将该位置的指针置NULL
            p--;//标记位置指针退回上一级节点
        }
    }

    fclose(fp);//关闭文件
    return OK;
}

int MaxPathSum(BiTree T)
{//初始条件是二叉树T存在；操作结果是返回根节点到叶子结点的最大路径和；

    // 如果当前结点是叶子结点，则直接返回该结点的键值
    if (T->lchild == NULL && T->rchild == NULL)
        return T->data.key;

        // 如果左子树为空，则仅考虑右子树节点的路径和
    else if (T->lchild == NULL && T->rchild != NULL)
        return MaxPathSum(T->rchild) + T->data.key;

        // 如果右子树为空，则仅考虑左子树节点的路径和
    else if (T->lchild != NULL && T->rchild == NULL)
        return MaxPathSum(T->lchild) + T->data.key;

    // 如果左右子树都非空，则计算左右子树的最大路径和，并将当前节点的键值加上左右子树的最大路径和中的较大值
    int leftmax = 0, rightmax = 0;
    leftmax = MaxPathSum(T->lchild);
    rightmax = MaxPathSum(T->rchild);
    if (leftmax > rightmax)
        return leftmax + T->data.key;
    else
        return rightmax + T->data.key;
}

//该函数的功能是：返回二叉树T中e1节点和e2节点的最近公共祖先
BiTree LowestCommonAncestor(BiTree T, int e1, int e2)
{
//先找到节点p1和p2，分别代表e1和e2在二叉树中对应的结点
    BiTree p1 = LocateNode(T,e1);
    BiTree p2 = LocateNode(T,e2);
//设置一个标志变量flag，用于标记是否判断过结点是否存在
    static int flag = 0;
    if(flag == 0)//如果flag是0，说明还没有判断过结点是否存在
    {
        flag = 1;
//如果e1或e2对应的结点不存在，或者它们中有一个对应的结点不存在，
//则输出错误并返回NULL
        if( p1 == NULL || p2 == NULL)
        {
            printf("输入的关键字错误\n");
            return NULL;
        }
    }

//如果二叉树为空，或者T结点的关键字为e1或e2，则返回T结点
    if (T == NULL || T->data.key == e1 || T->data.key == e2)
        return T;
//递归查找左子树
    BiTree left = LowestCommonAncestor(T->lchild, e1, e2);
//递归查找右子树
    BiTree right = LowestCommonAncestor(T->rchild, e1, e2);
//如果left为空，说明这两个节点在T结点的右子树上，我们只需要返回右子树查找的结果即可
    if (left == NULL)
        return right;
//如果right为空，说明这两个节点在T结点的左子树上，我们只需要返回左子树查找的结果即可
    if (right == NULL)
        return left;
//如果left和right都不为空，说明这两个节点一个在T的左子树上一个在T的右子树上
//T结点就是e1和e2的公共祖先！
    return T;
}

//函数名称：BiTree InvertTree(BiTree T)
//函数功能：将二叉树T翻转，使其所有节点的左右节点互换
//参数说明：二叉树T
//返回值：翻转后的二叉树T

BiTree InvertTree(BiTree T)
{
//如果二叉树为空，则直接返回
    if (T == NULL)
        return NULL;

//递归处理左子树，返回左子树翻转后的结果
    BiTree left = InvertTree(T->lchild);

//递归处理右子树，返回右子树翻转后的结果
    BiTree right = InvertTree(T->rchild);

//交换左右节点
    T->lchild = right;
    T->rchild = left;

//返回翻转后的结果
    return T;
}

void fun01()
{
    menufirst(); // 输出主菜单，提供可选的操作命令
    int a ; // 命令编号/选择
    printf("请输入一个命令\n");
    scanf("%d",&a);

    while (a) // 当输入非0时，继续进行操作
    {
        fflush(stdin); //清空输入流，防止上一次操作结束后输入了数据而影响本次操作

        int feedback; // 操作返回值

        switch (a) { // 根据命令编号进行相应的操作
            case 1: // 创建一个新的二叉树
                printf("现在进行创建一个新的二叉树\n");
                printf("请输入你想创建的二叉树的名字\n");
                char name1[30]; // 用于存储输入的二叉树名字
                scanf("%s",name1);
                int i ,flag ; flag = 0; // 标记位，用于判断是否已存在同名二叉树

                // 要进行名字的判断，遍历数组中的所有二叉树名字
                for( i =0;i<Lists.length;i++)
                {
                    if(strcmp(name1,Lists.elem[i].name)  == 0) //如果名字已经存在，则无法创建这个二叉树
                    {
                        printf("该二叉树已经存在，创建失败\n");
                        flag = 1;
                    }
                }

                if(flag == 0) // 如果不存在同名二叉树，则可以创建
                {
                    //将新的二叉树名字加入到数组Lists中，并将Lists的长度加1
                    strcpy(Lists.elem[Lists.length].name,name1);
                    Lists.length++;
                    printf("创建成功力\n");
                }
                break;

            case 2: // 删除二叉树
                int flag2 ; // 标记位，用于记录要删除的二叉树在数组中的位置
                printf("现在进行删除二叉树的操作\n");
                printf("请输入你想删除的二叉树的名字\n");
                char name2[30]; // 用于存储目标二叉树名字
                scanf("%s",name2);
                flag2 = -1; //flag2用于标记要删除的二叉树在Lists数组中的位置

                //遍历数组中的所有二叉树名字，如果存在目标二叉树，则更新标记位
                for( i =0;i<Lists.length;i++)
                {
                    if(strcmp(name2,Lists.elem[i].name)  == 0)
                    {
                        flag2 = i;
                    }
                }

                if(flag2 == -1) // 如果不存在目标二叉树，则无法进行删除操作
                {
                    printf("该二叉树不存在，无法删除\n");
                }
                else{
                    feedback = DestroyBiTree(Lists.elem[flag2].T); // 调用DestroyBiTree函数销毁指定位置处的二叉树

                    if(feedback == OK) // 如果操作成功
                    {
                        //将Lists数组中指定位置之后的元素向前移动一个位置，同时将Lists的长度减1
                        int k;
                        for( k = 0 ;k < Lists.length-1 ;k++)
                        {
                            Lists.elem[k] = Lists.elem[k+1];
                        }
                        Lists.length--;
                        printf("删除成功\n");
                    }
                }
                break;

            case 3: // 查询创建了哪些二叉树
                printf("现在进行查询创建了哪些二叉树\n");
//                printf("所有的二叉树如下:\n");
                char name0[30]; // 用于存储目标二叉树名字
                scanf("%s",name0);
                //遍历数组中的所有二叉树名字，输出每个二叉树的名称
                for(i = 0; i<Lists.length ;i++)
                {
                    if(strcmp(Lists.elem[i].name,name0) == 0)
                    printf("%d)   %s\n",i+1,Lists.elem[i].name);
                }
                break;

            case 4: // 对二叉树进行操作
                printf("现在进行二叉树的查找和操作\n");
                printf("请输入你想查找和操作的二叉树的名字\n");
                char name3[30]; // 用于存储目标二叉树名字
                scanf("%s",name3);
                int flag3 ;flag3 = -1; //flag3用于标记要操作的二叉树在Lists数组中的位置

                //遍历数组中的所有二叉树名字，如果存在目标二叉树，则更新标记位
                for( i =0 ; i<Lists.length ;i++)
                {
                    if(strcmp(Lists.elem[i].name,name3) == 0)
                    {
                        flag3 = i;
                    }
                }

                if(flag3 ==-1) // 如果不存在目标二叉树，则无法进行操作
                {
                    printf("不存在这个二叉树\n");
                    system("pause");
                }
                else {
                    //调用fun02函数对特定位置处的二叉树进行操作
                    fun02(Lists.elem[flag3].T);
                }
                break;

            default:
                printf("输入的命令错误，请再次输入"); //如果输入的命令不在可选范围内，则提示输入命令错误
        }

        printf("请输入下一个命令\n");
        scanf("%d",&a);
        system("cls"); //每次操作结束后，清空屏幕并重新输出主菜单
        menufirst();
    }
}


/**
 * 定义函数fun02，传入参数BiTree &T
 */
void fun02(BiTree &T)
{
    /**
     * 定义变量definition[100]，用于存储输入的二叉树的内容
     * 定义变量op、i、next，用于接收用户输入的命令
     */
    TElemType definition[100];
    int op = 0,i =0,next = 0;

    /**
     * 清空控制台输出
     * 调用函数menu()，输出主菜单
     * 输出提示语句，让用户输入命令
     */
    system("cls");
    menu();
    printf("请输入你的命令\n");
    scanf("%d",&op);

    /**
     * 进入循环，只要op不为0，就执行代码块内的操作
     */
    while (op)
    {
        /**
         * 使用switch语句，根据不同的命令执行不同的操作
         */
        switch (op) {
            case 1:
                i = 0;
                /**
                 * 如果T已经存在，输出相应的提示语句，执行break跳出switch语句
                 */
                if(T)
                {
                    printf("二叉树已经初始化，操作失败\n");
                    break;
                }
                /**
                 * 输入需要创建的二叉树的内容
                 * 调用CreateBiTree函数进行创建
                 * 如果创建成功，输出相应的提示语句
                 * 如果创建失败，输出相应的提示语句
                 */
                printf("请输入二叉树内容：\n");
                do {
                    scanf("%d%s", &definition[i].key, definition[i].others);
                } while (definition[i++].key != -1);
                if (CreateBiTree(T, definition) == OK)
                {
                    printf("二叉树创建成功\n");
                }
                else
                    printf("二叉树创建失败！\n");
                break;
            case 2:
                /**
                 * 调用DestroyBiTree函数进行二叉树的销毁
                 * 如果销毁成功，输出相应的提示语句
                 * 如果销毁失败，输出相应的提示语句
                 */
                printf("现在进行二叉树的销毁\n");
                if (DestroyBiTree(T) == OK)
                    printf("二叉树销毁成功!\n");
                else
                    printf("二叉树销毁失败!\n");
                break;
            case 3:
                /**
                 * 调用ClearBiTree函数进行二叉树的清空
                 * 如果清空成功，输出相应的提示语句
                 * 如果清空失败，输出相应的提示语句
                 */
                printf("现在进行二叉树的清空\n");
                if (ClearBiTree(T) == OK)
                    printf("二叉树清空成功!\n");
                else
                    printf("二叉树清空失败!\n");
                break;
            case 4:
                /**
                 * 调用BiTreeEmpty函数判断二叉树是否为空
                 * 如果为空，输出相应的提示语句
                 * 如果不为空，输出相应的提示语句
                 */
                printf("现在进行二叉树的判空操作\n");
                if (BiTreeEmpty(T) == TRUE)
                    printf("二叉树是空树!\n");
                else if (BiTreeEmpty(T) == FALSE)
                    printf("二叉树不是空树!\n");
                else
                    printf("二叉树不存在!\n");
                break;
            case 5:
                /**
                 * 调用BiTreeDepth函数求二叉树的深度
                 * 如果求解成功，输出相应的提示语句
                 * 如果求解失败，输出相应的提示语句
                 */
                printf("现在求二叉树的深度\n");
                int j5;  //接收二叉树的深度函数的返回值
                if (T == NULL) {
                    printf("二叉树不存在!\n");
                    break;
                }
                j5 = BiTreeDepth(T);
                if(j5 == -1)
                {
                    printf("操作失败\n");
                }
                else{
                    printf("二叉树的深度为%d!\n", j5);
                }
                break;
            case 6:
                /**
                 * 输入需要查找的结点关键字
                 * 调用LocateNode函数进行查找
                 * 如果查找成功，输出相应的提示语句及查找结果
                 * 如果查找失败，输出相应的提示语句
                 */
                // 定义需要查找的结点关键字
                int e6;
                // 定义二叉树结点指针
                BiTree p6;
                // 判断二叉树是否存在
                if (T == NULL) {
                    printf("二叉树不存在!\n");
                    break;
                }
                // 提示用户输入需要查找的结点关键字
                printf("请输入你要查找的结点关键字：\n");
                // 读入用户输入的需要查找的结点关键字
                scanf("%d", &e6);
                // 调用LocateNode函数进行二叉树的查找操作，返回查找结果到p6中
                p6 = LocateNode(T, e6);
                // 判断是否查找到目标结点
                if (p6 == NULL)
                    printf("查找失败!\n");
                else
                    // 输出查找结果
                    printf("查找成功!其值为：%s\n",p6->data.others);
                break;
            case 7:
                int e7, j7; // 定义变量
                KeyType k7; // 定义关键字类型
                TElemType value7; // 定义结点内容类型
                if (T == NULL) { // 如果二叉树为空，输出提示语句
                    printf("二叉树不存在!\n");
                    break;
                }
                printf("请输入你要赋值的结点的关键字：\n"); // 提示输入
                scanf("%d", &k7); // 获取关键字
                printf("请输入你要赋值的内容：\n"); // 提示输入结点内容
                scanf("%d %s", &value7.key, &value7.others); // 获取结点内容
                j7 = Assign(T,k7,value7); // 调用赋值函数，返回状态值
                if (j7 == ERROR) // 如果赋值失败，输出相应的提示语句
                    printf("赋值失败!\n");
                else if (j7 == INFEASIBLE) // 如果二叉树不存在，输出相应的提示语句
                    printf("二叉树不存在!\n");
                else // 如果赋值成功，输出相应的提示语句
                    printf("赋值成功!\n");
                break;

            case 8:
                int e8; // 定义变量
                BiTree p8; // 定义二叉树指针
                if (T == NULL) { // 如果二叉树为空，输出相应的提示语句
                    printf("二叉树不存在!\n");
                    break;
                }
                printf("请输入你要获得的兄弟结点的关键字：\n"); // 提示输入
                scanf("%d", &e8); // 获取关键字
                p8 = GetSibling(T, e8); // 调用获取兄弟结点函数，返回兄弟结点指针
                if (p8 == NULL) // 如果获取失败，输出相应的提示语句
                    printf("获取失败!\n");
                else // 如果获取成功，输出相应的提示语句和兄弟结点内容
                    printf("获取兄弟结点成功!其值为：%d %s\n",p8->data.key, p8->data.others);
                break;
            case 9:    //插入结点
                int e9, j9, LR; // e9、j9、LR为变量，用于存储用户输入的值
                TElemType value9; // value9为结构体类型，用于存储用户输入的内容
                if (T == NULL) { // 如果二叉树不存在，则输出提示信息并结束
                    printf("二叉树不存在!\n");
                    getchar(); getchar();
                    break;
                }
                printf("请输入你要插入结点的关键字和LR：\n"); // 提示用户输入关键字和LR
                scanf("%d %d", &e9, &LR); // 从输入流中获取用户输入

                printf("请输入待插入的内容（格式：1 a）：\n"); // 提示用户输入待插入内容
                scanf("%d%s", &value9.key,value9.others); // 从输入流中获取用户输入
                j9 = InsertNode(T, e9, LR,value9); // 调用InsertNode函数插入结点
                if (j9 == ERROR) // 如果插入失败，输出提示信息
                    printf("插入结点失败!\n");
                else if (j9 == OK) // 如果插入成功，输出提示信息
                    printf("插入结点成功!\n");

                break; // 结束case 9

            case 10:   //删除结点
                int e10, j10; // e10、j10为变量，用于存储用户输入的值
                if (T == NULL) { // 如果二叉树不存在，则输出提示信息并结束
                    printf("二叉树不存在!\n");
                    break;
                }
                printf("请输入你要删除结点的关键字：\n"); // 提示用户输入待删除结点的关键字
                scanf("%d", &e10); // 从输入流中获取用户输入
                j10 = DeleteNode(T, e10); // 调用DeleteNode函数删除结点
                if (j10 == ERROR) // 如果删除失败，输出提示信息
                    printf("删除结点失败!\n");
                else if (j10 == OK) // 如果删除成功，输出提示信息
                    printf("删除结点成功!\n");

                break; // 结束case 10

            case 11:  //先序遍历
                int j11; // j11为变量，用于存储先序遍历函数的返回值
                if (T == NULL) { // 如果二叉树不存在，则输出提示信息并结束
                    printf("二叉树不存在!\n");
                    break;
                }
                j11 = PreOrderTraverse(T,visit); // 调用PreOrderTraverse函数完成先序遍历
                if (j11 == OK) // 如果遍历成功，输出提示信息
                    printf("\n完成先序遍历!\n");
                else{ // 如果遍历失败，输出提示信息
                    printf("遍历失败\n");
                }

                break; // 结束case 11

            case 12:
                int j12;//声明一个整型变量 j12，用于存储 InOrderTraverse 函数的返回值
                if (T == NULL) {//如果二叉树 T 不存在
                    printf("二叉树不存在!\n");//输出提示信息
                    break;//跳出 switch-case 循环
                }
                j12 = InOrderTraverse(T, visit);//执行中序遍历函数 InOrderTraverse，并将返回值存储到 j12 变量中
                if (j12 == OK)//如果遍历成功
                    printf("\n完成中序遍历!\n");//输出提示信息
                else
                    printf("\n遍历失败!\n");//输出提示信息

                break;//跳出 switch-case 循环
            case 13:
                int j13;//声明一个整型变量 j13，用于存储 PostOrderTraverse 函数的返回值
                if (T == NULL) {//如果二叉树 T 不存在
                    printf("二叉树不存在!\n");//输出提示信息

                    break;//跳出 switch-case 循环
                }
                j13 = PostOrderTraverse(T,visit);//执行后序遍历函数 PostOrderTraverse，并将返回值存储到 j13 变量中
                if (j13 == OK)//如果遍历成功
                    printf("\n完成后序遍历!\n");//输出提示信息
                else
                    printf("\n遍历失败!\n");//输出提示信息

                break;//跳出 switch-case 循环
            case 14:
                int j14;//声明一个整型变量 j14，用于存储 LevelOrderTraverse 函数的返回值
                if (T == NULL) {//如果二叉树 T 不存在
                    printf("二叉树不存在!\n");//输出提示信息

                    break;//跳出 switch-case 循环
                }
                j14 = LevelOrderTraverse(T, visit);//执行按层遍历函数 LevelOrderTraverse，并将返回值存储到 j14 变量中
                if (j14 == OK)//如果遍历成功
                    printf("\n完成按层遍历!\n");//输出提示信息
                else
                    printf("\n遍历失败!\n");//输出提示信息

                break;//跳出 switch-case 循环
            case 15:
                int j15;//声明一个整型变量 j15，用于存储 MaxPathSum 函数的返回值
                if (T == NULL) {//如果二叉树 T 不存在
                    printf("二叉树不存在!\n");//输出提示信息
                    getchar(); getchar();//暂停程序执行，等待用户输入

                    break;//跳出 switch-case 循环
                }
                j15 = MaxPathSum(T);//执行计算二叉树最大路径和函数 MaxPathSum，并将返回值存储到 j15 变量中
                printf("二叉树最大路径和为：%d!\n",j15+1);//输出计算结果

                break;//跳出 switch-case 循环

            case 16:
                if (T == NULL) {
                    printf("二叉树不存在!\n");

                    break;
                }


                int i16, j16; //需要查找公共祖先的第一个节点的关键字和第二个节点的关键字
                BiTree T16;  //查找到的公共祖先节点
                printf("请输入你要搜索公共祖先的两个结点的关键字：\n");
                scanf("%d %d", &i16, &j16);
                T16 = LowestCommonAncestor(T, i16, j16);
                if (T16 == NULL)
                {
                    printf("查找失败!\n");
                }

                else
                {
                    printf("查找成功！\n");
                    printf("公共祖先的关键字为：%d,其内容为%s\n", T16->data.key, T16->data.others);
                }


                break;

            case 17:
                if (T == NULL) {
                    printf("二叉树不存在!\n");
                    getchar();
                    break;
                }
                InvertTree(T);
                printf("已成功翻转二叉树！\n");

                break;
            case 18:
                int j18;
                char FileName18[30];  //保存到的文件名
                printf("请输入要写入的文件名：\n");
                scanf("%s", FileName18);
                j18 = SaveBiTree(T, FileName18);
                if (j18 == INFEASIBLE)
                    printf("二叉树不存在!\n");
                else
                    printf("成功将二叉树写入文件名为：%s的文件中!\n", FileName18);

                break;
            case 19:
                int j19;
                char FileName19[30];   //待读取数据的文件名
                printf("请输入要读取的文件名：\n");
                scanf("%s", FileName19);
                j19 = LoadBiTree(T, FileName19);
                if (j19 == INFEASIBLE)
                    printf("二叉树存在!无法覆盖！\n");
                else if (j19 == ERROR) {
                    printf("读取文件失败！\n");
                }
                else{
                    printf("成功将%s文件中的数据读入到二叉树中!\n", FileName19);
                }

                break;


            case 0:
                break;
        }
        putchar('\n');
        printf("请输入下一个命令\n");
        scanf("%d",&op);
        system("cls");
        if(op != 0)   //如果退出就加载第一个菜单
        {
            menu();
        }
        else{
            menufirst();
        }


    }
}

void menufirst()
{
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
    printf("1.创建一个二叉树\n");
    printf("2.删除一个二叉树\n");
    printf("3.查询已经创建的二叉树\n");
    printf("4.查找一个二叉树和进行操作\n");
    printf("0.退出多个二叉树的管理\n");


    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
}

void menu()
{

    printf("       Menu for Binary Tree On Binary Linked List    \n");
    printf("------------------------------------------------------\n");
    printf("**     1. 创建初始化二叉树    11.前序遍历              **\n");
    printf("**     2. 销毁二叉树         12. 中序遍历             **\n");
    printf("**     3. 清空二叉树         13. 后序遍历             **\n");
    printf("**     4. 二叉树判空         14. 层序遍历             **\n");
    printf("**     5. 求二叉树的深度      15. 最大路径和           **\n");
    printf("**     6. 查找结点           16. 最近公共祖先         **\n");
    printf("**     7. 结点赋值           17. 翻转二叉树           **\n");
    printf("**     8. 获得兄弟结点        18. 二叉树的文件保存      **\n");
    printf("**     9. 插入结点           19. 二叉树的文件加载      **\n");
    printf("**     10. 删除结点          0. Exit                **\n");
    printf("------------------------------------------------------\n");
    printf("    请选择你的操作[0~19]:\n");

}

//获取指定结点e的兄弟结点
BiTNode* GetSibling(BiTree T, KeyType e)
{
    if (T == NULL) //若二叉树为空，则返回空指针
    {
        printf("二叉树不存在\n");
        return NULL;
    }

    BiTree st[100], p; //定义一个存放结点指针的数组，同时声明一个指向树结构体的指针p
    int top = 0; //定义一个栈顶指针，初始值为0
    st[top++] = T; //将整棵树压入栈中

    while (top != 0) { //当栈不为空时，进行以下操作
        p = st[--top]; //取出栈顶元素，同时栈顶指针减1
        if (p->rchild->data.key == e) //如果p的右子结点为要查找兄弟结点的结点e
            return p->lchild; //则返回p的左子结点
        if (p->lchild->data.key == e) //如果p的左子结点为要查找兄弟结点的结点e
            return p->rchild; //则返回p的右子结点
        //如果p的右子树和左子树都不为空，则将它们分别压入栈中
        if (p->rchild->rchild != NULL && p->rchild->lchild != NULL)
            st[top++] = p->rchild;
        if (p->lchild->rchild != NULL && p->lchild->lchild != NULL)
            st[top++] = p->lchild;
    }
    return NULL; //若未找到兄弟结点，则返回空指针
}
