#define _CRT_SECURE_NO_WARNINGS
#include"def.h"

int max(int a, int b)
{
    return a >= b ? a : b;
}
void visit(BiTree T)
{
    printf(" %d,%s", T->data.key, T->data.others);
}
int KeyExistInTree(BiTree T, KeyType key) {
    if (!T) {
        return 0;
    }
    if (T->data.key == key) {
        return 1;
    }
    return KeyExistInTree(T->lchild, key) || KeyExistInTree(T->rchild, key);
}
int KeyExistInDef(TElemType definition[], int index, KeyType key)
{
    for (int i = 0; i < index; i++)
    {
        if (definition[i].key == key) return 1;
    }
    return 0;
}
status CreateBiTree(BiTree& T, TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
    // 用于记录当前处理的数组下标
    static int index = 0;
    // 若当前元素为空，则指针指向NULL，表示空子树
    if (definition[index].key == 0) {
        T = NULL;
        index++;
        return OK;
    }
    if (definition[index].key == -1) {
        return OK;
    }

    // 创建新节点
    T = (BiTree)malloc(sizeof(BiTNode));
    if (!T) {
        return ERROR;
    }

    // 判断关键字是否已存在，若存在则返回ERROR
    if (KeyExistInDef(definition, index, definition[index].key)) {
        return ERROR;
    }

    // 将数据复制到节点中
    T->data = definition[index++];

    // 递归构造左子树
    if (CreateBiTree(T->lchild, definition) == ERROR) {
        return ERROR;
    }
    // 递归构造右子树
    if (CreateBiTree(T->rchild, definition) == ERROR) {
        return ERROR;
    }

    return OK;
}
status ClearBiTree(BiTree& T)
//将二叉树设置成空，并删除所有结点，释放结点空间
{
    if (!T) {
        return OK; // 如果当前节点为空，直接返回
    }
    else
    {
        ClearBiTree(T->lchild);
        ClearBiTree(T->rchild);

    }
    free(T);
    T = NULL;
    return OK;
}
int BiTreeDepth(BiTree T)
//求二叉树T的深度
{
    if (!T) return 0;
    else return 1 + max(BiTreeDepth(T->lchild), BiTreeDepth(T->rchild));
}
BiTNode* LocateNode(BiTree T, KeyType e)
//查找结点
{
    if (T)
    {
        if (T->data.key == e) return T;
        else {
            BiTree l = LocateNode(T->lchild, e);
            BiTree r = LocateNode(T->rchild, e);
            if (l) return l;
            else if (r) return r;
            else return NULL;
        }
    }
    return NULL;
}
status Assign(BiTree& T, KeyType e, TElemType value)
//实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{

    BiTree t = LocateNode(T, e);
    if (t)
    {
        if (KeyExistInTree(T, value.key) && t->data.key != value.key) return ERROR;
        else
        {
            t->data = value;
            return OK;
        }
    }
    else return ERROR;
}
BiTNode* GetSibling(BiTree T, KeyType e)
//实现获得兄弟结点
{
    if (T) {
        if (T->lchild->data.key == e && T->rchild) return T->rchild;
        else if (T->rchild->data.key == e && T->lchild) return T->lchild;
        else
        {
            BiTree t1 = GetSibling(T->lchild, e);
            BiTree t2 = GetSibling(T->rchild, e);
            if (t1) return t1;
            else if (t2) return t2;
            else return NULL;
        }
    }
    return NULL;
}
BiTNode* GetSibling(BiTree T, KeyType e)
//实现获得兄弟结点
{
    if (T)
    {
        if (T->lchild && T->lchild->data.key == e && T->rchild) return T->rchild;
        else if (T->rchild && T->rchild->data.key == e && T->lchild) return T->lchild;
        else
        {
            BiTree t1 = GetSibling(T->lchild, e);
            BiTree t2 = GetSibling(T->rchild, e);
            if (t1) return t1;
            else if (t2) return t2;
            else return NULL;
        }
    }
    return NULL;
}
status InsertNode(BiTree& T, KeyType e, int LR, TElemType c)
//插入结点。此题允许通过增加其它函数辅助实现本关任务
{
    if (KeyExistInTree(T, c.key)) return ERROR;
    if (LR == -1)
    {
        BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
        NewNode->data = c;
        NewNode->rchild = T;
        T = NewNode;
        return OK;
    }
    else {
        BiTree t = LocateNode(T, e);
        if (!t) return ERROR;
        else {
            if (LR == 1)
            {
                BiTree tr = t->rchild;
                BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
                NewNode->data = c;
                t->rchild = NewNode;
                NewNode->rchild = tr;
            }
            else if (LR == 0)
            {
                BiTree tl = t->lchild;
                BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
                NewNode->data = c;
                t->lchild = NewNode;
                NewNode->rchild = tl;
            }
            return OK;
            
        }
    }
}
BiTNode* findParent(BiTree T, KeyType key) {
    if (T == NULL || (T->lchild == NULL && T->rchild == NULL)) {
        // 如果是空树或者是只有根节点的树，无法找到父节点
        return NULL;
    }
    // 如果根节点就是要找的节点，返回空，因为根节点没有父节点
    if (T->data.key == key) {
        return NULL;
    }

    // 如果左子树不为空且左子树的根节点的关键字等于要查找的关键字，则返回当前节点
    if (T->lchild != NULL && T->lchild->data.key == key) {
        return T;
    }

    // 如果右子树不为空且右子树的根节点的关键字等于要查找的关键字，则返回当前节点
    if (T->rchild != NULL && T->rchild->data.key == key) {
        return T;
    }

    // 递归在左子树中查找
    BiTNode* left = findParent(T->lchild, key);
    if (left != NULL) {
        return left;
    }

    // 递归在右子树中查找
    BiTNode* right = findParent(T->rchild, key);
    if (right != NULL) {
        return right;
    }

    // 如果左右子树都没有找到，则返回空
    return NULL;
}
int DeNode(BiTree T)
{
    int count = 0; // 初始化计数器为0
    // 检查左孩子是否存在，存在则增加计数器的值
    if (T->lchild != NULL) {
        count++;
    }
    // 检查右孩子是否存在，存在则增加计数器的值
    if (T->rchild != NULL) {
        count++;
    }
    return count; // 返回孩子节点的个数
}
status DeleteNode(BiTree& T, KeyType e)
{
    if (!T) return ERROR; // 树为空，返回错误
    BiTree current = LocateNode(T, e);
    if (!current) return ERROR;//找不到目标结点，返回错误
    int d = DeNode(current); // 获取节点的度
    BiTree parent = findParent(T, e);//找到父结点

    if (d == 0)
    {
        // 关键字为e的节点度为0，直接删除 
        if (parent->lchild == current)
            parent->lchild = NULL;
        if (parent->rchild == current)
            parent->rchild = NULL;
        free(current);
        current = NULL;
        return OK;
    }
    else if (d == 1) {
        // 关键字为e的节点度为1，用孩子节点代替被删除的节点，释放原节点
        BiTree child = (current->lchild != NULL) ? current->lchild : current->rchild;
        if (!parent) {
            free(current);
            current = NULL;
            T = child;
        }
        else {
            if (parent->lchild == current) {
                free(current);
                parent->lchild = child;
            }
            else {
                free(current);
                parent->rchild = child;
            }
        }
        return OK;
    }
    else if (d == 2) {
        // 关键字为e的节点度为2，用左子树代替被删除的节点，并将右子树接到左子树的最右节点上
        BiTree LC = current->lchild;//左子树
        BiTree RC = current->rchild;//右子树
        BiTree rightmost = LC;
        while (rightmost->rchild) {
            rightmost = rightmost->rchild;
        }
        rightmost->rchild = RC;
        if (!parent) {
            free(current);
            T = LC;
        }
        else {
            if (parent->lchild == current) {
                free(parent->lchild);
                parent->lchild = LC;
            }
            else {
                free(parent->rchild);
                parent->rchild = LC;
            }
        }
        return OK;
    }
    return OK;
}
status PreOrderTraverse(BiTree T, void (*visit)(BiTree))
//先序遍历二叉树T
{
    if (T) {
        BiTree p = T;
        stack<BiTree>s;
        while (p != NULL || !s.empty())
        {
            while (p != NULL)
            {
                s.push(p);
                visit(p);
                p = p->lchild;
            }
            if (!s.empty())
            {
                p = s.top();
                s.pop();
                p = p->rchild;
            }
        }
        return OK;
    }
    else return INFEASIBLE;
}
status InOrderTraverse(BiTree T, void (*visit)(BiTree))
{
    if (T)
    {
        InOrderTraverse(T->lchild, visit);
        visit(T);
        InOrderTraverse(T->rchild, visit);
        return OK;
    }
    else return INFEASIBLE;
}
status PostOrderTraverse(BiTree T, void (*visit)(BiTree))
//后序遍历二叉树T
{

    if (T)
    {
        PostOrderTraverse(T->lchild, visit);
        PostOrderTraverse(T->rchild, visit);
        visit(T);
        return OK;
    }
    else return INFEASIBLE;
}
status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
//按层遍历二叉树T
{
    queue < BiTree > q;
    if (T != NULL)
        q.push(T);   //根节点进队列
    while (!q.empty())  //队列不为空
    {
        BiTree node = q.front();
        visit(node);
        if (node->lchild != NULL)   //如果有左孩子，入队
            q.push(node->lchild);
        if (node->rchild != NULL)   //如果有右孩子，入队
            q.push(node->rchild);
        q.pop();  //已经遍历过的节点出队列
    }
}
void PreOrder(BiTree T, int position, FILE* fp)
{
    if (T)
    {
        fprintf(fp, "%d %d %s ", position, T->data.key, T->data.others);
        PreOrder(T->lchild, position * 2, fp);
        PreOrder(T->rchild, position * 2 + 1, fp);

    }
}
status CreateBiTree2(BiTree& T, DEF definition[])
{
    int i = 0, j;
    static BiTNode* p[100];
    while (j = definition[i].pos)
    {
        p[j] = (BiTNode*)malloc(sizeof(BiTNode));
        p[j]->data = definition[i].data;
        p[j]->lchild = NULL;
        p[j]->rchild = NULL;
        if (j != 1)
            if (j % 2)   p[j / 2]->rchild = p[j];
            else      p[j / 2]->lchild = p[j];
        i++;
    }
    T = p[1];
    return OK;
}
status SaveBiTree(BiTree T, char FileName[])
//将二叉树的结点数据写入到文件FileName中
{
    if (!T) return INFEASIBLE;
    FILE* fp = fopen(FileName, "w");
    PreOrder(T, 1, fp);
    fprintf(fp, "0 0 null");
    fclose(fp);
    return OK;
}
status LoadBiTree(BiTree& T, char FileName[])
//读入文件FileName的结点数据，创建二叉树
{
    FILE* fp = fopen(FileName, "r");
    DEF LoadDefinition[100];
    int i = 0;
    do {
        fscanf(fp, "%d%d%s", &LoadDefinition[i].pos, &LoadDefinition[i].data.key,
            LoadDefinition[i].data.others);
    } while (LoadDefinition[i++].pos);
    fclose(fp);
    if (CreateBiTree2(T, LoadDefinition) == OK)  return OK;
    else return ERROR;
}
