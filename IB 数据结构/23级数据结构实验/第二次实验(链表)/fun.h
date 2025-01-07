#define _CRT_SECURE_NO_WARNINGS
#include"def.h"

/***************************************************************
*函数名称：InitList
*函数功能：创建线性表
*注释： 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBL
*返回值类型：status类型
****************************************************************/

status InitList(LinkList& L)
{
    if (L != NULL) return INFEASIBLE;
    else {
        L = ((LinkList)malloc(sizeof(LNode)));
        L->next = NULL;
        L->data = 0;
        return OK;
    }
}

/***************************************************************
*函数名称：DestroyList
*函数功能：销毁线性表
*注释：如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status DestroyList(LinkList& L)
{
    if (L != NULL)
    {
        LinkList p = L;
        LinkList q = NULL;
        while (p)
        {
            q = p->next;
            free(p);
            p = q;
        }
        L = NULL;
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：ClearList
*函数功能：清空线性表
*注释：如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ClearList(LinkList& L)
{
    if (L != NULL)
    {
        LinkList p = L->next;
        LinkList q = NULL;
        while (p)
        {
            q = p->next;
            free(p);
            p = q;
        }
        L->next = NULL;
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：ListEmpty
*函数功能：清空线性表
*注释：如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE
*返回值类型：status类型
****************************************************************/

status ListEmpty(LinkList L)
{
    if (L != NULL)
    {
        if (L->next == NULL) return TRUE;
        else return FALSE;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：ListLength
*函数功能：线性表长度
*注释：如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE
*返回值类型：status类型
****************************************************************/

status ListLength(LinkList L)
{
    if (L != NULL)
    {
        int length = 0;
        LinkList p = L->next;
        while (p)
        {
            length++;
            p = p->next;
        }
        return length;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：GetElem
*函数功能：获取元素
*注释：如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；
如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status GetElem(LinkList L, int i, ElemType& e)
{
    if (L != NULL)
    {
        if (i < 1) return ERROR;
        int count = 0;
        LinkList p = L;
        while (p && count < i)
        {
            p = p->next;
            count++;
        }
        if (count != i || !p) return ERROR;
        else {
            e = p->data;
            return OK;
        }
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：LocateElem
*函数功能：查找元素
*注释： 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；
如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE（即-1）。
*返回值类型：int类型
****************************************************************/

status LocateElem(LinkList L, ElemType e)
{
    if (L != NULL)
    {
        int i = 0;
        LinkList p = L;
        while (p)
        {
            p = p->next;
            i++;
            while (p)
            {
                p = p->next;
                i++;
                if (p && p->data == e) return i;
            }
            return ERROR;
        }
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：PriorElem
*函数功能：获得前驱
*注释：如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；
如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status PriorElem(LinkList L, ElemType e, ElemType& pre)
{
    if (L != NULL)
    {
        LinkList p = L, q=p->next;
        while (q)
        {
            q = p->next;
            if (q && p != L && q->data == e)
            {
                pre = p->data;
                return OK;
            }
            p = q;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：NextElem
*函数功能：获取后继
*注释：如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；
如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status NextElem(LinkList L, ElemType e, ElemType& next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (L != NULL)
    {
        LinkList p = L->next, q;
        while (p)
        {
            q = p->next;
            if (q && p->data == e)
            {
                next = q->data;
                return OK;
            }
            p = q;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：ListInsert
*函数功能：插入元素
*注释：如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；
当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ListInsert(LinkList& L, int i, ElemType e)
{
    if (L != NULL)
    {
        int count = 1;
        LinkList p = L, q = L->next;
        while (p)
        {
            if (i < 1) break;
            if (count == i)
            {
                LinkList NewNode = (LinkList)malloc(sizeof(LNode));;
                p->next = NewNode;
                NewNode->next = q;
                NewNode->data = e;
                return OK;
            }
            p = q;
            if (p == NULL) break;
            q = q->next;
            count++;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：ListDelete
*函数功能：删除元素
*注释：如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；
当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ListDelete(LinkList& L, int i, ElemType& e)
{
    if (L != NULL)
    {
        int count = 1;
        LinkList p = L, q = L->next;
        while (q)
        {
            if (i < 1) break;
            if (count == i)
            {
                p->next = q->next;
                e = q->data;
                free(q);
                return OK;
            }
            p = q;
            q = q->next;
            count++;
        }
        return ERROR;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：ListTraverse
*函数功能：遍历表
*注释： 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；
如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ListTraverse(LinkList L)
{
    if (L != NULL)
    {
        LinkList p = L->next;
        while (p)
        {
            printf("%d ", p->data);
            p = p->next;
        }
        if (L->next == NULL) printf("线性表为空!\n");
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：SaveList
*函数功能：保存线性表到文件中
*注释：如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status SaveList(LinkList L, char FileName[])
{
    if (L != NULL)
    {
        FILE* fp = fopen(FileName, "w");
        LinkList p = L->next;
        while (p)
        {
            fprintf(fp, "%d ", p->data);
            p = p->next;
        }
        fclose(fp);
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：LoadList
*函数功能：从文件中读入数据到线性表
*注释： 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status LoadList(LinkList& L, char FileName[])
{
    if (L == NULL)
    {
        L = ((LinkList)malloc(sizeof(LNode)));
        L->next = NULL;
        FILE* fp = fopen(FileName, "r");
        LinkList p = L;
        ElemType e;
        while (fscanf(fp, "%d", &e) != EOF)
        {
            LinkList newNode = (LinkList)malloc(sizeof(LNode));
            newNode->data = e;
            newNode->next = NULL;
            p->next = newNode;
            p = newNode; // 移动到新节点
        }
        fclose(fp);
        return OK;
    }
    else return INFEASIBLE;
}

/***************************************************************
*函数名称：reverseList
*函数功能：链表逆置
*注释： 利用p，q指针的移动通过一次遍历逆置链表，每次更新头指针指向的next为p，若线性表为空
，返回INFEASIBLE，否则逆置链表后返回OK。
*时间复杂度:0(n)
*返回值类型：status类型
****************************************************************/

status reverseList(LinkList& L)
{
    if (!L) return INFEASIBLE;
    else
    {
        LinkList p, q;
        p = L->next;
        L->next = NULL;
        while (p)
        {
            q = p->next;
            p->next = L->next;
            L->next = p;
            p = q;
        }
        return OK;
    }
}

/***************************************************************
*函数名称：RemoveNthFromEnd
*函数功能：删除线性表倒数第n个结点
*注释：先求出线性表总长度，转化为删除正数第
*时间复杂度:0(n)
*返回值类型：status类型
****************************************************************/

status RemoveNthFromEnd(LinkList& L, int n,ElemType& e)
{
    if (L == NULL) return INFEASIBLE;
    else if (L->next == NULL) return ERROR;
    else {
        int x = ListLength(L) - n + 1;
        ListDelete(L, x, e);
        return OK;
    }
}

/***************************************************************
*函数名称：sortList
*函数功能：线性表排序
*注释：冒泡排序改进版，当线性表不存在时，返回INFEASIBLE，当线性表为空时，返回ERROR，
此外进行冒泡排序，返回OK。
*时间复杂度:0(n^2)
*返回值类型：status类型
****************************************************************/

status sortList(LinkList L) {
   
    if (L == NULL) return INFEASIBLE;
    else if (L->next == NULL) return ERROR;
    else {
        int swap = 1;      // 用于检测是否发生交换
        ElemType t;  // 用于交换节点
        LinkList end = NULL;   // 指向已排序部分的末尾

        while (swap) {//若没有发生交换，即swap为0，则退出循环
            swap = 0;          // 在每次外部循环开始时重置交换标志
            LinkList current = L->next;  // 从链表头开始遍历

            while (current->next != end) {
                LinkList next = current->next;
      
                if (current->data > next->data)
                {
                    t = current->data;
                    current->data = next->data;
                    next->data = t;
                    swap = 1;  // 标记发生了交换
                }
                else {
                    current = current->next;
                }
            }
            end = current;  // 每轮大循环结束时，已排序部分的末尾为当前节点
        }
        return OK;
    }
}	// 如果链表为空或只有一个节点，无需排序
