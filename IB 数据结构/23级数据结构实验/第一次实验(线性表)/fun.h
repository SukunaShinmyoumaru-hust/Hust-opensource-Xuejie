#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<algorithm>
using namespace std;
#define TRUE 1                //有关常量定义
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int status;
typedef int ElemType; //数据元素类型定义
#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef int ElemType;
typedef struct {  //顺序表（顺序结构）的定义
    ElemType* elem;
    int length;
    int listsize;
}SqList;
typedef struct {  //线性表的集合类型定义
    struct {
        char name[30];
        SqList L;
    } elem[10];
    int length;
}LISTS;
LISTS Lists;      //线性表集合的定义Lists

status InitList(SqList& L);
status DestroyList(SqList& L);
status ClearList(SqList& L);
status ListEmpty(SqList L);
status ListLength(SqList L);
status GetElem(SqList L, int i, ElemType& e);
int LocateElem(SqList L, ElemType e);
status PriorElem(SqList L, ElemType e, ElemType& pre);
status NextElem(SqList L, ElemType e, ElemType& next);
status ListInsert(SqList& L, int i, ElemType e);
status ListTraverse(SqList L);
status ListDelete(SqList& L, int i, ElemType& e);
status  SaveList(SqList L, char FileName[]);
status  LoadList(SqList& L, char FileName[]);
status AddList(LISTS& Lists, char ListName[]);
status RemoveList(LISTS& Lists, char ListName[]);
int LocateList(LISTS Lists, char ListName[]);
ElemType MaxSubArray(SqList L);
int SubArrayNum(SqList L, int k);
status SortList(SqList);

ElemType max(ElemType a, ElemType b)
{
    return a > b ? a : b;
}

/***************************************************************
*函数名称：InitList
*函数功能：创建线性表
*注释： 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBL
*返回值类型：status类型
****************************************************************/

status InitList(SqList& L)
{
    if (!L.elem)//如果线性表为空
    {
        L.elem = (ElemType*)malloc(sizeof(ElemType) * LIST_INIT_SIZE);//内存分配
        L.length = 0;//长度初始化为0
        L.listsize = LIST_INIT_SIZE;//初始化空间大小
        return OK;
    }
    else {
        return INFEASIBLE;
    }
}

/***************************************************************
*函数名称：DestroyList
*函数功能：销毁线性表
*注释：如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status DestroyList(SqList& L)
{
    if (L.elem)//如果线性表存在
    {
        free(L.elem);//释放空间
        L.elem = NULL;
        L.length = 0;
        L.listsize = 0;
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

status ClearList(SqList& L)
{
    if (L.elem)//如果线性表存在
    {
        L.length = 0;//长度置为0，等价于删除所有元素
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

status ListEmpty(SqList L)
{
    if (!L.elem)//如果线性表不存在
    {
        return INFEASIBLE;
    }
    else
    {
        if (L.length == 0) return TRUE;
        else return FALSE;
    }

}

/***************************************************************
*函数名称：ListLength
*函数功能：线性表长度
*注释：如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE
*返回值类型：status类型
****************************************************************/

status ListLength(SqList L)
{
    /********** Begin *********/
    if (!L.elem) return INFEASIBLE;
    else return L.length;

    /********** End **********/
}

/***************************************************************
*函数名称：GetElem
*函数功能：获取元素
*注释：如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；
如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status GetElem(SqList L, int i, ElemType& e)
{
    if (!L.elem) return INFEASIBLE;
    else {
        if (i<1 || i>L.length) return ERROR;
        else {
            e = L.elem[i - 1];
            return OK;
        }
    }

}

/***************************************************************
*函数名称：LocateElem
*函数功能：查找元素
*注释： 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；
如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE（即-1）。
*返回值类型：int类型
****************************************************************/

int LocateElem(SqList L, ElemType e)
{
    if (!L.elem) return INFEASIBLE;
    else
    {
        int flag = -1;//表示尚未找到
        for (int i = 0; i < L.length; i++)
        {
            if (L.elem[i] == e)
            {
                flag = i; break;
            }
        }
        if (flag == -1) return ERROR;
        else return flag + 1;
    }

}

/***************************************************************
*函数名称：PriorElem
*函数功能：获得前驱
*注释：如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；
如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status PriorElem(SqList L, ElemType e, ElemType& pre)
{

    if (!L.elem) return INFEASIBLE;
    else {
        for (int i = 1; i < L.length; i++)
        {
            if (L.elem[i] == e) {
                pre = L.elem[i - 1];
                return OK;
            }
        }
        return ERROR;
    }

}

/***************************************************************
*函数名称：NextElem
*函数功能：获取后继
*注释：如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；
如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status NextElem(SqList L, ElemType e, ElemType& next)

{

    if (!L.elem) return INFEASIBLE;
    else {
        for (int i = 0; i < L.length - 1; i++)
        {
            if (L.elem[i] == e) {
                next = L.elem[i + 1];
                return OK;
            }
        }
        return ERROR;
    }

}

/***************************************************************
*函数名称：ListInsert
*函数功能：插入元素
*注释：如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；
当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ListInsert(SqList& L, int i, ElemType e)
{
    if (!L.elem) return INFEASIBLE;
    else {

        if (i<1 || i>L.length + 1) return ERROR;

        else {

            L.length++;
            if (L.length >= L.listsize) //记得扩容！
            {
                L.elem = (ElemType*)realloc(L.elem, sizeof(ElemType*) * (L.listsize + LISTINCREMENT));
            }
            for (int j = L.length - 1; j >= i; j--)
            {
                L.elem[j] = L.elem[j - 1];
            }
            L.elem[i - 1] = e;
            return OK;
        }
    }

}

/***************************************************************
*函数名称：ListTraverse
*函数功能：遍历表
*注释： 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；
如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ListTraverse(SqList L)
{

    if (!L.elem) return INFEASIBLE;
    else {

        for (int i = 0; i < L.length; i++)
        {
            printf("%d", L.elem[i]);
            if (i != L.length - 1) printf(" ");
        }
        return OK;
    }

}

/***************************************************************
*函数名称：ListDelete
*函数功能：删除元素
*注释：如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；
当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status ListDelete(SqList& L, int i, ElemType& e)

{

    if (!L.elem) return INFEASIBLE;
    else {
        if (i <= 0 || i >= L.length + 1) return ERROR;//判断i的位置是否合法
        else {
            if (i == L.length)//如果是最后一个元素，删除即可
            {
                e = L.elem[i - 1];
                L.length--;
                return OK;
            }
            else            //否则需要将其他元素向前移动
            {
                e = L.elem[i - 1];
                for (int j = i - 1; j <= L.length - 2; j++)
                {
                    L.elem[j] = L.elem[j + 1];
                }
                L.length--;
                return OK;
            }
        }
    }

}

/***************************************************************
*函数名称：SaveList
*函数功能：保存线性表到文件中
*注释：如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status  SaveList(SqList L, char FileName[])
{
    FILE* fp;
    fp = fopen(FileName, "w");
    if (!L.elem) return INFEASIBLE;
    else {
        for (int i = 0; i < L.length; i++) 
        {
            fprintf(fp, "%d", L.elem[i]);//将数据打印到文件中
            if (i != L.length - 1) {
                fprintf(fp, " ");  //格式控制
            }
        }
        fclose(fp);
        return OK;

    }
}

/***************************************************************
*函数名称：LoadList
*函数功能：从文件中读入数据到线性表
*注释： 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
*返回值类型：status类型
****************************************************************/

status  LoadList(SqList& L, char FileName[])
{
    FILE* fp;
    fp = fopen(FileName, "r");
    if (L.elem) return INFEASIBLE;
    else
    {
        L.elem = (ElemType*)malloc(sizeof(ElemType) * LIST_INIT_SIZE);//创建新表
        L.length = 0;
        L.listsize = LIST_INIT_SIZE;

        ElemType data;
        while (fscanf(fp, "%d", &data) != EOF)  //从文件中读取数据
        {
            if (L.length >= L.listsize)//若线性表已满，进行扩容
            {
                L.elem = (ElemType*)realloc(L.elem, sizeof(ElemType) * ( 
                    L.listsize + LISTINCREMENT));
                L.listsize += LISTINCREMENT;
            }
            L.elem[L.length++] = data;
        }

        fclose(fp);
        return OK;
    }
}

/***************************************************************
*函数名称：AddList
*函数功能：添加新表
*注释：如果线性表集合已满，返回ERROR，否则创建一个新线性表并分配内存，返回OK。
*返回值类型：status类型
****************************************************************/

status AddList(LISTS& Lists, char ListName[])

{
    if (Lists.length > 9) return ERROR;
    else {
        strcpy(Lists.elem[Lists.length].name, ListName);
        Lists.elem[Lists.length].L.elem = NULL;
        InitList(Lists.elem[Lists.length].L);
        Lists.length++;
        return OK;
    }
    /********** End **********/
}

/***************************************************************
*函数名称：RemoveList
*函数功能：删除指定表
*注释：如果找到该线性表，删除并返回OK，否则返回ERROR
*返回值类型：status类型
****************************************************************/

status RemoveList(LISTS& Lists, char ListName[])
{
    int flag = -1;//判断是否找到目标
    for (int i = 0; i < Lists.length; i++)
    {
        if (strcmp(Lists.elem[i].name, ListName) == 0)//依次比较
        {
            flag = i;
            break;//找到后退出循环
        }
    }
    if (flag != -1)
    {
        for (int i = flag; i < Lists.length - 1; i++) {
            Lists.elem[i] = Lists.elem[i + 1];
        }
        Lists.length--;
        return OK;
    }
    else return ERROR;
}

/***************************************************************
*函数名称：LocateList
*函数功能：查找指定表
*注释：如果找到该线性表，返回其逻辑序号，否则返回0.
*返回值类型：int类型
****************************************************************/

int LocateList(LISTS Lists, char ListName[])
// 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
{

    int flag = -1;
    for (int i = 0; i < Lists.length; i++)
    {
        if (strcmp(Lists.elem[i].name, ListName) == 0)
        {
            flag = i + 1;
            break;
        }
    }
    if (flag == -1) return 0;
    else return flag;
}

/***************************************************************
*函数名称：MaxSubArray
*函数功能：计算最大连续子数组和
*注释：利用贪心思想保留有益结果
*返回值类型：ElemType类型
****************************************************************/

ElemType MaxSubArray(SqList L)
{
    ElemType ans = L.elem[0];
    ElemType sum = 0;
    for (int i = 0; i < L.length; i++)
    {
        if (sum >= 0) sum += L.elem[i];
        else sum = L.elem[i];
        ans = max(ans, sum);
    }
    return ans;

}

/***************************************************************
*函数名称：SubArrayNum
*函数功能：计算和为K的子数组个数
*注释：利用前缀和和枚举算法比较每一种情况
*返回值类型：int类型
****************************************************************/

int SubArrayNum(SqList L, int k) {
    int count = 0;

    // 计算前缀和数组
    int* prefixSum = (int*)malloc(sizeof(int) * (L.length + 1));
    prefixSum[0] = 0;
    for (int i = 0; i < L.length; i++) {
        prefixSum[i + 1] = prefixSum[i] + L.elem[i];
    }

    // 遍历所有子数组并比较和是否为k
    for (int i = 0; i < L.length; i++) {
        for (int j = i + 1; j <= L.length; j++) {
            if (prefixSum[j] - prefixSum[i] == k) {
                count++;
            }
        }
    }
    return count;
}

/***************************************************************
*函数名称：SortList
*函数功能：线性表排序
*注释：如果线性表不存在，返回INFEASIBLE，如果线性表为空，返回ERROR。
*返回值类型：status类型
****************************************************************/

status SortList(SqList L)
{
    if (!L.elem) return INFEASIBLE;
    else if (L.length == 0) return ERROR;
    else
    {
        sort(L.elem, L.elem + L.length);//调用库函数从小到大排序
        return OK;
    }
}
