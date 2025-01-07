#define _CRT_SECURE_NO_WARNINGS
#include"def.h"
int index = 0;
int max(int a, int b)
{
    return a >= b ? a : b;
}

/***************************************************************
*函数名称：KeyExistInTree
*函数功能：检测关键字是否存在于树中
*注释：Assign的辅助函数，用于检测树中是否存在重复关键字
*返回值类型：int类型
****************************************************************/

int KeyExistInTree(BiTree T, KeyType key) {
    if (!T) {
        return 0;
    }
    if (T->data.key == key) {
        return 1;
    }
    return KeyExistInTree(T->lchild, key) || KeyExistInTree(T->rchild, key);
}

/***************************************************************
*函数名称：KeyExistInDef
*函数功能：检测关键字是否存在
*注释：CreateBiTree的辅助函数，用于检测序列中是否存在重复关键字
*返回值类型：int类型
****************************************************************/

int KeyExistInDef(TElemType definition[], int index, KeyType key)
{
    for (int i = 0; i < index; i++)
    {
        if (definition[i].key == key) return 1;
    }
    return 0;
}

/***************************************************************
*函数名称：CreateBiTree
*函数功能：创建二叉树
*注释：根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，
将根节点指针赋值给T并返回OK，如果有相同的关键字，返回ERROR。 
*返回值类型：status类型
****************************************************************/

status CreateBiTree(BiTree& T, TElemType definition[])
{
    // 用于记录当前处理的数组下标
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
/***************************************************************
*函数名称：DestroyBiTree
*函数功能：摧毁二叉树
*注释：将二叉树设置成空，并删除所有结点，释放结点空间
*返回值类型：status类型
****************************************************************/

status DestroyBiTree(BiTree& T)
{
    if (!T) {
        return OK; // 如果当前节点为空，直接返回
    }
    else
    {
        DestroyBiTree(T->lchild);
        DestroyBiTree(T->rchild);

    }
    free(T);
    T = NULL;
    return OK;
}

/***************************************************************
*函数名称：ClearBiTree
*函数功能：清空二叉树
*注释：将二叉树设置成空，但不释放内存空间
*返回值类型：status类型
****************************************************************/

status ClearBiTree(BiTree T)
{
    if (T)
    {
        T->lchild = NULL;
        T->rchild = NULL;
        T->data.key = -1;//清空二叉树,左右子树指针指向空，关键字设置为-1表示空树
    }
    return OK;
}
/***************************************************************
*函数名称：BiTreeEmpty
*函数功能：二叉树判空
*注释：关键字为-1表示空树,返回true,否则返回false
*返回值类型：status类型
****************************************************************/

status BiTreeEmpty(BiTree T)
{
    if (T && T->data.key == -1) return true;
    else return false;
}

/***************************************************************
*函数名称：BiTreeDepth
*函数功能：求二叉树深度
*注释：递归求深度，借助了max函数
*返回值类型：int类型
****************************************************************/

int BiTreeDepth(BiTree T)
{
    if (!T) return 0;
    else if (T->data.key == -1) return -1;
    else return 1 + max(BiTreeDepth(T->lchild), BiTreeDepth(T->rchild));
}

/***************************************************************
*函数名称：LocateNode
*函数功能：查找结点
*注释：递归查找，若找到目标节点，返回该结点的指针，否则返回NULL
*返回值类型：BiTNode*类型
****************************************************************/

BiTNode* LocateNode(BiTree T, KeyType e)
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

/***************************************************************
*函数名称：Assign
*函数功能：实现结点赋值
*注释：调用LocateNode查找节点，并进行关键字判重
*返回值类型：status类型
****************************************************************/

status Assign(BiTree& T, KeyType e, TElemType value)
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

/***************************************************************
*函数名称：GetSibling
*函数功能：实现获得兄弟结点
*注释：递归查找，若找到，返回其兄弟结点的指针；若没找到，返回NULL
*返回值类型：BiTNode*类型
****************************************************************/

BiTNode* GetSibling(BiTree T, KeyType e)
{
    if (T) {
        if (T->lchild && T->lchild->data.key == e && T->rchild) return T->rchild;//判断key要先检查指针是否为空!
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

/***************************************************************
*函数名称：InsertNode
*函数功能：插入结点
*注释：根据LR为0或者1，插入结点c到T中，作为关键字为e的结点的左或
右孩子结点，结点e的原有左子树或右子树则为结点c的右子树
*返回值类型：status类型
****************************************************************/

status InsertNode(BiTree& T, KeyType e, int LR, TElemType c)

{
    if (KeyExistInTree(T, c.key)) return INFEASIBLE;//C.key已存在
    if (LR == -1)
    {
        BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
        NewNode->data = c;
        NewNode->rchild = T;
        T = NewNode;
        return OK;
    }
    else {
        BiTree t = LocateNode(T, e);//未找到关键字为e的结点
        if (!t) return ERROR;
        else {
            if (LR == 1)
            {
                BiTree tr = t->rchild;
                BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
                NewNode->data = c;
                t->rchild = NewNode;
                NewNode->rchild = tr;
                NewNode->lchild = NULL;
            }
            else if (LR == 0)
            {
                BiTree tl = t->lchild;
                BiTree NewNode = (BiTree)malloc(sizeof(BiTNode));
                NewNode->data = c;
                t->lchild = NewNode;
                NewNode->rchild = tl;
                NewNode->lchild = NULL;
            }
            return OK;

        }
    }
}

/***************************************************************
*函数名称：findParent
*函数功能：查找父结点
*注释：DeleteNode的辅助函数，通过找到被删除结点的父结点，从而改变其
孩子，查找过程采用了递归
*返回值类型：BiTNode*类型
****************************************************************/

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

/***************************************************************
*函数名称：DeNode
*函数功能：计算结点的度
*注释：DeleteNode的辅助函数，根据结点的度执行不同操作
*返回值类型：int类型
****************************************************************/

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

/***************************************************************
*函数名称：DeleteNode
*函数功能：删除结点
*注释：如果关键字为e的结点度为0，删除即可；如关键字为e的结点度为1，
用关键字为e的结点孩子代替被删除的e位置；如关键字为e的结点度为2，用
e的左孩子代替被删除的e位置，e的右子树作为e的左子树中最右结点的右子树
*返回值类型：status类型
****************************************************************/

status DeleteNode(BiTree& T, KeyType e)
{
    if (!T) return ERROR; // 树为空，返回错误
    BiTree current = LocateNode(T, e);
    if (!current) return INFEASIBLE;//找不到目标结点，返回错误
    int d = DeNode(current); // 获取节点的度
    BiTree parent = findParent(T, e);//找到父结点

    if (d == 0)
    {
        // 关键字为e的节点度为0，直接删除 
        if (current == T)
        {
            free(T);
            T = NULL;
            return OK;
        }
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

/***************************************************************
*函数名称：PreOrderTraverse
*函数功能：先序遍历二叉树
*注释：采用栈来实现先序遍历的非递归算法。创建栈，并初始化。遍历结点，
若结点存在，则入栈，并输出结点的值，指向其左孩子；否则出栈，访问结点
，指向其右孩子。如果结点不存在或者栈为空，则遍历结束
*返回值类型：status类型
****************************************************************/

status PreOrderTraverse(BiTree T)

{
    if (T) {
        BiTree p = T;
        stack<BiTree>s;
        while (p != NULL || !s.empty())
        {
            while (p != NULL)
            {
                s.push(p);
                printf(" %d,%s", p->data.key, p->data.others);
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

/***************************************************************
*函数名称：InOrderTraverse
*函数功能：中序遍历二叉树
*注释：采用递归的方式
*返回值类型：status类型
****************************************************************/

status InOrderTraverse(BiTree T)
{
    if (T)
    {
        InOrderTraverse(T->lchild);
        printf(" %d,%s", T->data.key, T->data.others);
        InOrderTraverse(T->rchild);
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：PostOrderTraverse
*函数功能：后序遍历二叉树
*注释：采用递归的方式
*返回值类型：status类型
****************************************************************/

status PostOrderTraverse(BiTree T)
{

    if (T)
    {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        printf(" %d,%s", T->data.key, T->data.others);
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：LevelOrderTraverse
*函数功能：层序遍历二叉树
*注释：使用队列保存每一层的所有节点，把队列里的所有节点出队列，
然后把这些出去节点各自的子节点入队列。以此来完成对每层的遍历。
*返回值类型：status类型
****************************************************************/

status LevelOrderTraverse(BiTree T)
{
    queue < BiTree > q;
    if (T != NULL)
        q.push(T);   //根节点进队列
    while (!q.empty())  //队列不为空
    {
        BiTree node = q.front();
        printf(" %d,%s", node->data.key, node->data.others);
        if (node->lchild != NULL)   //如果有左孩子，入队
            q.push(node->lchild);
        if (node->rchild != NULL)   //如果有右孩子，入队
            q.push(node->rchild);
        q.pop();  //已经遍历过的节点出队列
    }
    return OK;
}

/***************************************************************
*函数名称：MaxPathSum
*函数功能：计算二叉树中从根节点到任意节点路径上的最大带权路径和
*注释：采用递归方式实现
*返回值类型：int
****************************************************************/

int MaxPathSum(BiTree T) {
    if (!T) return 0;

    // 递归计算左子树和右子树的最大路径和
    int leftSum = MaxPathSum(T->lchild);
    int rightSum = MaxPathSum(T->rchild);

    // 返回左右子树中的较大路径和，加上当前节点的值
    return max(leftSum, rightSum) + T->data.key;
}

/***************************************************************
*函数名称：LowestCommonAncestor
*函数功能：找到二叉树中两个节点的最近公共祖先
*注释：假设二叉树节点结构为BiTree，节点数据类型为KeyType，节点中
含有指向左右子节点的指针lchild和rchild
*返回值类型：BiTree
****************************************************************/

BiTree LowestCommonAncestor(BiTree T, KeyType e1, KeyType e2) {
    // 如果根节点为空，直接返回空指针
    if (!T) return NULL;

    // 如果第一个关键字存在于左子树中
    if (LocateNode(T->lchild, e1)) {
        // 如果第二个关键字也存在于左子树中，递归在左子树中查找最近公共祖先
        if (LocateNode(T->lchild, e2))
            return LowestCommonAncestor(T->lchild, e1, e2);
        // 如果第二个关键字不在左子树中，当前节点即为最近公共祖先
        return T;
    }
    // 如果第一个关键字不在左子树中，但在右子树中
    else {
        // 如果第二个关键字存在于右子树中，递归在右子树中查找最近公共祖先
        if (LocateNode(T->rchild, e2))
            return LowestCommonAncestor(T->rchild, e1, e2);
        // 如果第二个关键字不在右子树中，当前节点即为最近公共祖先
        return T;
    }
}

void printBinaryTree(BiTree root) {
    if (!root) return;

    std::queue<BiTNode*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size(); // 当前层的节点数
        std::vector<TElemType> levelNodes; // 用于存储当前层的节点值

        // 遍历当前层的节点，并将其子节点加入队列
        for (int i = 0; i < size; ++i) {
            BiTNode* node = q.front();
            q.pop();
            levelNodes.push_back(node->data); // 记录当前节点值

            if (node->lchild) q.push(node->lchild);
            if (node->rchild) q.push(node->rchild);
        }

        // 打印当前层节点值
        for (const auto& nodeData : levelNodes) {
            std::cout << std::setw(4) << nodeData.key << " "; // 控制节点值输出格式
        }
        std::cout << std::endl;
    }
}

/***************************************************************
*函数名称：InvertTree
*函数功能：翻转二叉树，即交换每个节点的左右子树。
*注释：交换左右子树，递归实现
*返回值类型：status
****************************************************************/

status InvertTree(BiTree& T) {
    // 如果当前节点为空，直接返回操作成功
    if (!T)
        return OK;

    // 交换当前节点的左右子树
    BiTNode* temp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = temp;

    // 递归翻转左右子树
    InvertTree(T->lchild);
    InvertTree(T->rchild);

    // 返回操作成功
    return OK;
}

/***************************************************************
*函数名称：PreOrderPrint
*函数功能：先序遍历二叉树
*注释：SaveBiTree的辅助函数，以特定格式记录二叉树结点值
*返回值类型：status
****************************************************************/

status PreOrderPrint(BiTree T, FILE* fp) {
    if (T)
    {
        fprintf(fp, "%d %s ", T->data.key, T->data.others);
        PreOrderPrint(T->lchild, fp);
        if (!T->lchild)   fprintf(fp, "0 null ");
        PreOrderPrint(T->rchild, fp);
        if (!T->rchild)   fprintf(fp, "0 null ");
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：SaveBiTree
*函数功能：将二叉树的结点数据写入到文件FileName中
*注释：打印带空枝的先序序列，并以-1 null作为结束标志
*返回值类型：status
****************************************************************/

status SaveBiTree(BiTree T, char FileName[]) {
    // 将二叉树的结点数据写入到文件FileName中
    FILE* fp = fopen(FileName, "w");
    if (!fp) // 检查文件是否成功打开
        return ERROR;
    // 调用先序遍历打印函数
    PreOrderPrint(T, fp);
    // 打印结束标志
    fprintf(fp, "-1 null");
    fclose(fp); // 关闭文件
    return OK;
}

/***************************************************************
*函数名称：LoadBiTree
*函数功能：从文件中读取二叉树信息，创建二叉树
*注释：读取带空枝的先序序列，调用CreateBiTree函数
*返回值类型：status
****************************************************************/

status LoadBiTree(BiTree& T, char FileName[]) {
    FILE* fp = fopen(FileName, "r");
    if (!fp) // 检查文件是否成功打开
        return ERROR;
    TElemType definition[100];
    int i = 0;
    index = 0;
    do {
        fscanf(fp, "%d%s", &definition[i].key, definition[i].others);
    } while (definition[i++].key != -1);
    if (CreateBiTree(T, definition)) return OK;
    else return ERROR;

}
