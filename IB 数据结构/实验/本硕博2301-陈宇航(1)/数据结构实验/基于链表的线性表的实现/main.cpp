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

// 初始链表的最大长度
#define LIST_INIT_SIZE 100
// 每次新增的长度
#define LISTINCREMENT 10

// 定义数据元素类型
typedef int ElemType;
typedef int status;

// 定义单链表（链式结构）结点的结构体
typedef struct LNode{
    ElemType data; // 结点的数据元素
    struct LNode *next; // 指向下一个结点的指针
}LNode, *LinkList;

// 定义链表集合的结构体
typedef struct {
    struct {
        char name[30]; // 集合的名称，最多可以有 30 个字符
        LinkList L; // 指向链表头结点的指针
    }elem[30]; // 集合中最多包含 30 个链表
    int length; // 集合中包含的链表数目
}LISTS;

LISTS Lists;  //链表集合实例化为Lists对象

/*---- 函数申明 ----*/

//基础功能
void fun(LinkList &L );  //封装的处理单个线性表的处理函数
status InitList(LinkList &L);  //新建
status DestroyList(LinkList &L);  //销毁
status ClearList(LinkList &L);  //清空
status ListEmpty(LinkList L);   //判空
status ListLength(LinkList L);  //求长度
status GetElem(LinkList L,int i,ElemType &e);  //获取元素
status LocateElem(LinkList L,ElemType e,int (*vis)(int ,int ));  //判断位置
status PriorElem(LinkList L,ElemType e,ElemType &pre);   //前驱
status NextElem(LinkList L,ElemType e,ElemType &next);   //后继
status ListInsert(LinkList &L,int i,int num);   //插入
status ListDelete(LinkList &L,int i,ElemType &e);  //删除
status ListTraverse(LinkList L,void (*vi)(int ));  //遍历

//额外功能
void reverseList(LinkList L);   //翻转线性表
void RemoveNthFromEnd(LinkList L,int n);  //删除倒数元素
void sortList(LinkList L);  //排序
void savetofile(LinkList L,char name[]);  //保存到文件
void getfromfile(LinkList L,char name[]);  //读取文件

//文件操作
status AddList(LISTS &Lists,char ListName[]);//增加线性表
status RemoveList(LISTS &Lists,char ListName[]);//除去线性表
status LocateList(LISTS Lists,char ListName[]);//定位线性表
void SearchList(LISTS Lists);   //展示已经创建的线性表

//辅助函数
status  compare(int a,int b);   //判断位置函数时候调用的比较函数
void visit(int x);   //遍历函数时候调用的输出函数

//菜单
void menu();  //管理多个线性表的菜单
void show_normal();  //单个线性表的菜单
void Menuofinsert();  //插入的菜单



int main()
{
    system("color ob");
    menu(); //调用菜单
    int a; //定义一个整型变量
    printf("请输入一个命令\n"); //输出提示信息
    scanf("%d", &a); //读取一个整型变量

    //通过while循环来进行命令处理
    while (a) //当a不为0时执行循环体
    {
        fflush(stdin); //清空输入流，防止上一次操作影响本次操作

        switch (a) {
            //如果a等于1，执行以下代码
            case 1:
                printf("现在进行创建线性表\n"); //输出提示信息
                printf("请输入你想创建的线性表的名字\n"); //输出提示信息
                char name1[30]; //定义一个名字为name1且长度最大为30的字符数组
                scanf("%s", name1); //读取字符串

                int u; //定义一个整型变量
                u = AddList(Lists, name1); //向一个线性表数组中插入一个新的空线性表
                if(u == OK) //如果插入成功
                {
                    printf("创建成功啦\n"); //输出提示信息
                }
                if(u == INFEASIBLE) //如果插入失败
                {
                    system("pause"); //暂停程序运行
                }
                break;

                //如果a等于2，执行以下代码
            case 2:
                printf("现在进行删除线性表\n"); //输出提示信息
                printf("请输入你想创建的线性表的名字\n"); //输出提示信息
                char name2[30]; //定义一个名字为name2且长度最大为30的字符数组
                scanf("%s", name2); //读取字符串
                RemoveList(Lists, name2); //从线性表数组中删除指定的线性表
                break;

                //如果a等于3，执行以下代码
            case 3:
                printf("现在进行查询创建了哪些线性表\n"); //输出提示信息
                SearchList(Lists); //查询线性表数组中所有线性表的名字并输出
                break;

                //如果a等于4，执行以下代码
            case 4:
                printf("现在进行线性表的查找和操作\n"); //输出提示信息
                printf("请输入你想查找和操作的线性表的名字\n"); //输出提示信息
                char name3[30]; //定义一个名字为name3且长度最大为30的字符数组
                scanf("%s", name3); //读取字符串
                int judge; //定义一个整型变量
                judge=LocateList(Lists, name3); //查找线性表数组中指定名字的线性表，并返回其下标

                if(judge ==-1) //如果查找失败
                {
                    printf("不存在这个线性表\n"); //输出提示信息
                    system("pause"); //暂停程序运行
                }
                else { //如果查找成功
                    fun(Lists.elem[judge-1].L); //执行另一个函数
                }
                break;

                //如果a不等于1、2、3、4，执行以下代码
            default:
                printf("输入的命令错误，请再次输入"); //输出提示信息
        }

        printf("请输入下一个命令\n"); //输出提示信息
        scanf("%d", &a); //读取一个整型变量
        system("cls"); //清屏
        menu(); //再次调用另一个函数
    }
}
/*----- 函数定义  在主函数里面输出提示----*/
//（1）初始化表：函数名称是InitList(L)；初始条件是线性表L不存在；操作结果是构造一个空的线性表；
status InitList(LinkList &L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
// 如果线性表L已存在，则返回 INFEASIBLE
    if(L)
        return INFEASIBLE;
// 分配一个新的节点作为线性表头结点
    L = (LinkList)malloc(sizeof(LNode));
// 将头结点的指针域置为空
    L->next = NULL;
// 返回 OK
    return OK;
}

status DestroyList(LinkList &L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回 OK，否则返回 INFEASIBLE。
{
// 如果线性表L不存在，则返回 INFEASIBLE
    if(!L)
    {
        return INFEASIBLE;
    }
// 定义指针 p 指向当前结点，q 指向下一个结点
    LinkList p = L, q;
// 如果当前结点不为空，则继续循环
    while(p)
    {
// 将 q 指向当前结点的下一个结点
        q = p->next;
// 释放当前结点的空间
        free(p);
// 将指针 p 指向 q，继续循环
        p = q;
    }
// 返回 OK
    return OK;
}

//（3）清空表：函数名称是ClearList(L)；初始条件是线性表L已存在；操作结果是将L重置为空表；
status ClearList(LinkList &L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
    //不存在单链表 不用操作
    if(!L)
    {
        printf("单链表不存在或者未初始化，无法清空");
        return INFEASIBLE;
    }
    //空的 不用操作
    if(L->next==NULL)
    {
        printf("已经是空的了，还清啥空");
        return INFEASIBLE;
    }
    else{
        LinkList p = L->next;
        while(L->next){//如果没空
            L->next=p->next;//要删除的节点首先提出，把下一个节点接入，删除当前节点
            free(p);//删除当前节点
            p=L->next;//指针指向下一个节点
        }
    }
    L->next=NULL;//指向NULL；
    return OK;
}

//（4）判定空表：函数名称是ListEmpty(L)；初始条件是线性表L已存在；操作结果是若L为空表则返回TRUE,否则返回FALSE；
status ListEmpty(LinkList L)
{
    // 如果线性表L不存在，返回INFEASIBLE
    if(!L)
    {
        printf("线性表不存在或未初始化，无法进行清空\n");
        return INFEASIBLE;
    }
    // 如果够了L的第一个元素为空，表明线性表为空，返回TRUE
    if(L->next == NULL)
    {
        printf("线性表是空的\n");
        return TRUE;
    }
    // 线性表不为空，返回FALSE
    printf("线性表不是空的\n");
    return FALSE;
}

//（5）求表长：函数名称是ListLength(L)；初始条件是线性表已存在；操作结果是返回L中数据元素的个数；
int ListLength(LinkList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
    // 首先要检查线性表是否存在
    if (!L) {
        printf("线性表不存在或未初始化，无法进行清空\n");
        return INFEASIBLE;
    }

    L = L->next;    // 不要把头节点考虑
    int number = 0;   //用来 记录长度
    while (L) {     // 遍历链表，计算数据元素的个数
        number++;
        L = L->next;
    }
    return number;
}

//（6）获得元素：函数名称是GetElem(L,i,e)；初始条件是线性表已存在，1≤i≤ListLength(L)；操作结果是用e返回L中第i个数据元素的值；
status GetElem(LinkList L, int i, ElemType &e)
// 获取线性表L中第i个元素，将其存储在e中
{
    if(!L) // 若L为空，表示线性表不存在或未初始化
    {
        printf("线性表不存在或未初始化，无法进行清空\n");
        return INFEASIBLE; // 返回INFEASIBLE
    }
    int number = 0; // 记录当前遍历到的节点数，从0开始
    LinkList p = L; // 定义p指针，指向L
    p = L->next; // 跳过头节点，从第一个存储数据的节点开始遍历
    while(p)
    {
        number++; // 遍历到一个节点，number加1
        if(number == i) // 找到第i个节点
        {
            e = p->data; // 将找到的节点的数据存储在e中
            return OK; // 返回OK
        }
        p = p->next; // 指针p指向下一个节点
    }
    printf("i的值不合法，无法操作\n");
    return ERROR; // 遍历完整个线性表，未找到第i个节点，返回ERROR
}

//（7）查找元素：函数名称是LocateElem(L,e,compare())；初始条件是线性表已存在；
// 操作结果是返回L中第1个与e满足关系compare（）关系的数据元素的位序，若这样的数据元素不存在，则返回值为0；
status LocateElem(LinkList L,ElemType e,int (*vis)(int ,int ))
// 查找元素e在线性表L中的位置序号
// 当e存在时，返回其在线性表中的位置序号
// 当e不存在时，返回ERROR
// 当线性表L不存在时，返回INFEASIBLE
{
    // 当线性表L不存在时，返回INFEASIBLE
    if(!L)
    {
        printf("线性表不存在或未初始化，无法进行查找\n");
        return INFEASIBLE;
    }

    // 从头结点的下一个结点开始向后遍历
    LinkList p = L->next;

    // 记录当前位置序号
    int number = 0;

    // 遍历链表，查找元素e
    while(p)
    {
        number++;

        if(vis(p->data,e) == 1) // 如果找到元素e，返回其位置序号
        {
            return number;
        }
        p = p->next;
    }
    // 如果遍历完整个线性表仍未找到元素e，返回ERROR
    printf("没有所要查询的元素\n");
    return ERROR;
}

//（8）获得前驱：函数名称是PriorElem(L,cur_e,pre_e)；
// 初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，否则操作失败，pre_e无定义；
status PriorElem(LinkList L,ElemType e,ElemType &pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L)    // 如果链表L不存在，则返回INFEASIBLE，表示不可行
    {
        printf("线性表不存在或未初始化，无法进行清空\n");
        return INFEASIBLE;
    }
    if(L->next == NULL)  // 如果链表L为空，则返回ERROR，表示出错
    {
        printf("线性表里面没有元素\n");
        return ERROR;
    }
    LinkList p = L->next;
    if(p->data == e )  // 如果所要查找的元素e是第一个元素，不存在前驱，返回ERROR
    {
        printf("所要查找的元素是第一个元素，没有前驱\n");
        return ERROR;
    }
    while(p->next)   // 从第二个元素开始往后遍历整个链表，找到要查找的元素e
    {
        L = p->next;
        if(L->data == e )   // 如果找到要查找的元素e，则将该元素的前驱保存在pre中，返回OK
        {
            pre = p->data;
            return OK;
        }
        p = L;
    }
    printf("所要查找的元素不存在线性表里面,无法操作\n"); // 如果整个链表中都没有找到要查找的元素e，则返回ERROR，表示出错
    return ERROR;
}

//（9）获得后继：函数名称是NextElem(L,cur_e,next_e)；
// 初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，否则操作失败，next_e无定义；
status NextElem(LinkList L,ElemType e,ElemType &next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(!L)   // 如果线性表L不存在
    {
        printf("线性表不存在或未初始化，无法进行查询\n");
        return INFEASIBLE;
    }
    if(L->next==NULL)   // 如果线性表L是空表
    {
        printf("这个线性表是空的\n");
        return ERROR;
    }
    LinkList p = L->next;   // 设p为第一个结点，p做前驱结点
    L =  p ->next;   // 设L为p的后继结点，L做当前结点
    while(p)   // 如果p不为空
    {
        if( !L)   // 如果L为空，表示已经没有后继结点了
        {
            printf("查询不到后继结点\n");
            return ERROR;
        }
        if(p->data == e)   // 如果p的数据等于给定数据e
        {
            next = L->data;   // 将L的数据赋给next
            return OK;   // 返回操作成功
        }
        p = L ;   // 将p移到L的位置，作为新的前驱结点
        L = p->next;   // 将L移到下一个结点位置，作为新的当前结点
    }
    return ERROR;   // 如果循环结束时仍未查询到，返回操作失败
}

//需要注意 这里是改进版本 e改成了num 可以一次性插入大量数据 方便快速输入
//（10）插入元素：函数名称是ListInsert(L,i,e)；初始条件是线性表L已存在，1≤i≤ListLength(L)+1；操作结果是在L的第i个位置之前插入新的数据元素e；
status ListInsert(LinkList &L,int i,int num)
{
    // 先进行特判，如果线性表不存在，返回 INFEASIBLE
    if(!L)
    {
        printf("线性表不存在或未初始化，无法进行插入\n");
        return INFEASIBLE;
    }
    int e;

    // 用两个指针 p 和 next 分别指向当前遍历到的节点和下一个节点
    LinkList p = L, next = L->next;
    int number = 1;  //来记录当前位置

    // 遍历链表，找到要插入的位置
    printf("请输入元素：\n");
    while(next)
    {
        if(number == i)
        {
            // 当找到插入位置时，使用一个循环插入 num 个元素
            while (num)
            {
                // 创建新的节点，获取用户输入的数据
                LinkList insert = (LinkList)malloc(sizeof(LNode));
                scanf("%d",&e);
                insert->data = e;

                // 修改链表指针指向，完成插入操作
                p->next = insert;
                insert->next = next;
                p = insert;
                num--;
            }

            return OK;
        }

        // 继续向下遍历
        number++;
        p = next;
        next = p->next;
    }

    // 如果插入位置为最后一个位置，则在链表尾部插入
    if( number == i)
    {
        LinkList insert;

        // 使用循环将 num 个数据插入到尾部
        while (num) {
            scanf("%d",&e);
            insert = (LinkList)malloc(sizeof(LNode));
            insert->data = e;
            insert->next = NULL;
            p->next = insert;
            p = insert;
            num--;
        }
        return OK;
    }

    // 如果插入位置不正确，返回错误
    printf("插入的位置不对\n");
    return ERROR;
}


//（11）删除元素：函数名称是ListDelete(L,i,e)；初始条件是线性表L已存在且非空，1≤i≤ListLength(L)；操作结果：删除L的第i个数据元素，用e返回其值；
status ListDelete(LinkList &L,int i,ElemType &e)  // 删除线性表L的第i个元素，并保存在e中，返回OK或ERROR或INFEASIBLE
{
    if(!L)  // 如果线性表L不存在，返回INFEASIBLE
    {
        printf("线性表不存在或未初始化，无法进行清空\n");
        return INFEASIBLE;
    }
    int number = 0;  // 用于记数，记录当前扫描到的元素的位置

    LinkList pre = L, next = L->next;  // pre用于记录当前扫描到的元素的前一个元素，next用于记录当前扫描到的元素

    while(next)  // 遍历线性表，直到到达表尾
    {
        number++;  // 计数器加1，记录当前扫描到的元素的位置

        if(number == i)  // 如果找到第i个元素
        {
            e = next->data;  // 将该元素的值保存在e中
            pre->next = next->next;  // 将当前元素的前一个元素的指针指向当前元素的后一个元素，实现删除操作
            free(next);  // 释放内存
            return OK;  // 返回执行成功
        }
        pre = next;  // 当前元素保存到前一个元素变量pre中
        next = pre->next;  // 后一个元素保存到当前元素变量next中，实现遍历
    }
    printf("想要删除的位置存在问题\n");  // 如果遍历到表尾仍未找到第i个元素，则输出提示
    return ERROR;  // 返回执行失败
}

//（12）遍历表：函数名称是ListTraverse(L,visit())，初始条件是线性表L已存在；
// 操作结果是依次对L的每个数据元素调用函数visit( )。
status ListTraverse(LinkList L,void (*vi)(int ))
// 遍历线性表 L 中的元素，依次使用函数指针 vi 处理每个元素。
// 如果线性表 L 不存在，返回 INFEASIBLE，否则返回 OK。
{
    if (!L) // 如果线性表 L 不存在
    {
        printf("线性表不存在或未初始化，无法进行操作\n");
        return INFEASIBLE; // 返回 INFEASIBLE
    }
    LinkList p = L->next; // 从 L 中第一个元素开始遍历
    while (p) // 只要当前节点不是尾节点
    {
        vi(p->data); // 对当前节点的元素使用函数指针 vi 进行处理
        p = p->next; // 指向下一个节点
        if (p) // 如果当前不是最后一个节点
        {
            putchar(' '); // 输出一个空格，与下一个元素分隔开来
        }
    }
    return OK; // 遍历结束，返回 OK
}

// 在Lists中增加一个名称为ListName的空线性表
// Lists: 线性表集合，包含多个线性表
// ListName: 待添加的线性表名称
// 返回值：操作状态，成功为OK，否则为INFEASIBLE
status AddList(LISTS &Lists,char ListName[])
{
    // 循环查找是否已经存在同名线性表
    for(int i = 0; i<Lists.length ;i++)
    {
        if(strcmp(ListName,Lists.elem[i].name) == 0) // 判断线性表名称是否相同
        {
            printf("这个名字的线性表已经存在了"); // 输出提示信息
            return INFEASIBLE; // 返回INFEASIBLE表示操作失败
        }
    }
    // 未找到同名线性表，可以继续添加
    Lists.length++; // 线性表集合长度+1
    int n = 0;
    // 将新线性表的名称和数据初始化
    strcpy(Lists.elem[Lists.length-1].name ,ListName); // 将线性表名称赋值
    Lists.elem[Lists.length-1].L = NULL; // 将存储数据的指针初始化为NULL
    return OK; // 返回OK表示操作成功
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    // 遍历线性表数组找到需要删除的线性表
    for(int k = 0;k<Lists.length;k++)
    {
        if(strcmp(Lists.elem[k].name,ListName)==0)  // 逐一判断线性表名称是否与要删除的名称相同
        {
            DestroyList(Lists.elem[k].L);  // 先销毁这个线性表本身的空间
            // 指针和名称逐一向前移动
            for(int i = k;i<Lists.length-1;i++)
            {
                strcpy(Lists.elem[i].name, Lists.elem[i+1].name);   // 逐一将后面的线性表的名称复制到前面
                Lists.elem[i].L =Lists.elem[i+1].L;   // 将后面线性表的指针复制到前面
            }
            // 线性表数组的长度减 1
            Lists.length--;
            return OK;  // 删除成功
        }
    }
    return ERROR;   // 没有找到要删除的线性表，删除失败
}

int LocateList(LISTS Lists,char ListName[])
// 查找一个名称为ListName的线性表在Lists中的位置，成功返回逻辑序号，否则返回-1
{
    // 开始遍历线性表
    for(int k = 0 ;k< Lists.length;k++)
    {
        // 比较线性表名称是否匹配
        if(strcmp(Lists.elem[k].name,ListName)==0)
        {
            return k+1;  // 返回序号（序号从 1 开始）
        }
    }
    return -1;  // 查找失败，返回 -1
}

void SearchList(LISTS Lists)   //这个函数负责展示已经创建的线性表
{
    int i =0;
    printf("已经存在的线性表有：\n");
    for( ;i<Lists.length;i++)
    {
        printf("序号 %d)  线性表的名称:%s\n",(i+1),Lists.elem[i].name);
    }
}

void reverseList(LinkList L)
{
    //判断线性表是否存在或者初始化
    if(!L)
    {
        printf("线性表不存在");
        return;
    }
    int len = ListLength(L);
    int *arr=(int *)malloc(sizeof(int)*len);
    //遍历链表 把元素一个个放入到数组中
    LinkList p1 = L->next;
    for(int k=0;k<len;k++)
    {
        arr[k]=p1->data;
        p1=p1->next;
    }
    //从头放入元素 倒置
    LinkList p2 = L->next;
    for(int i = len-1;i>=0;i--)
    {
        p2->data=arr[i];
        p2=p2->next;
    }
    printf("成功反转了");
    return ;
}

void RemoveNthFromEnd(LinkList L,int n) // RemoveNthFromEnd函数的定义，参数为一个单链表和要删除的节点位置
{
    if(!L) // 如果单链表为空或未初始化
    {
        printf("线性表不存在或着未初始化\n"); // 打印错误信息
        return; // 退出函数
    }

    int len = ListLength(L); // 获取单链表的长度
    int e; // 用来存储被删除节点的数据元素
    int feedback; // 存储ListDelete函数的返回值，即删除操作的结果

    feedback = ListDelete(L,len-n+1,e); // 调用ListDelete函数删除第(len-n+1)个节点，并将被删除节点的值存入e中

    if(feedback == OK) // 如果删除成功
    {
        printf("成功删除，删除的元素是 :%d\n",e); // 打印成功信息及被删除节点的值
    }
}

//排序
void sortList(LinkList L)
{
    //判断线性表是否存在或者未初始化
    if(!L)
    {
        printf("线性表不存在或者未初始化\n");
        return;
    }
    //获取线性表长度
    int len = ListLength(L);
    //申请动态内存，用于存放线性表数据
    int *arr =(int *) malloc(sizeof(int)*len);
    //遍历线性表，将数据存放到arr数组中
    LinkList p1 = L->next;
    for(int k=0;k<len;k++)
    {
        arr[k] = p1->data;
        p1 = p1->next;
    }
    //对arr数组进行冒泡排序
    for(int k=0;k<len-1;k++)
    {
        for(int i=0;i<len-1-k;i++)
        {
            if(arr[i] > arr[i+1])
            {
                int tmp = arr[i];  //中间变量tmp
                arr[i] = arr[i+1];
                arr[i+1] = tmp;
            }
        }
    }
    //将排序后的数据写回线性表中
    LinkList p2 = L->next;
    for(int k =0;k<len;k++)
    {
        p2->data = arr[k];
        p2 = p2->next;
    }
}

// 从线性表L中将数据保存到文件name中
void savetofile(LinkList L,char name[])
{
    if(!L) // 如果L不存在或未初始化，无法进行操作
    {
        printf("线性表不存在或未初始化\n");
        return;
    }
    FILE *fp = fopen(name,"w"); // 打开文件，以写的方式
    if(fp == NULL) // 如果无法找到文件，报错
    {
        printf("打开文件失败\n");
        return;
    }
    LinkList current = L->next; // 指向第一个节点
    while (current != NULL) // 循环遍历线性表中的每个节点
    {
        fprintf(fp, "%d ", current->data); // 将节点的数据写入文件中
        current = current->next;
    }
    fclose(fp); // 关闭文件
    printf("成功保存到文件了\n"); // 提示信息，表明操作成功
    return;
}

// 从文件name中读取数据，保存到线性表L中
void getfromfile(LinkList L,char name[])
{
    if(L->next) // 如果L不是空的，说明已经有数据了，无法进行操作
    {
        printf("这不是一个空的线性表，读取数据会导致原来的数据被覆盖，无法操作\n");
        return;
    }
    FILE *fp = fopen(name,"r"); // 打开文件，以读的方式
    if(fp == NULL) // 如果无法找到文件，报错
    {
        printf("打开文件失败\n");
        return;
    }
    LinkList p = L; // 用于遍历线性表
    LinkList insert= (LinkList) malloc(sizeof(LNode)); // 动态分配内存，用于存放从文件中读取的数据
    while (fscanf(fp,"%d ",&insert->data) != EOF) // 循环读取文件中的数据
    {
        p->next = insert; // 插入到线性表中
        p = insert; // 指向刚插入的节点
        p->next = NULL; // 该节点的下一个节点为空
        insert= (LinkList) malloc(sizeof(LNode)); // 为下一个节点动态分配内存
    }
    fclose(fp); // 关闭文件
    return;
}

void show_normal()   //单个线性表的菜单
{
    // 输出横线
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    putchar('\n');

    // 输出菜单标题
    printf("           Menu for Linear Table On Sequence Structure \n");

    // 输出菜单选项
    printf("    	  1. InitList                       7. LocateElem\n");
    printf("    	  2. DestroyList                    8. PriorElem\n");
    printf("    	  3. ClearList                      9. NextElem \n");
    printf("    	  4. ListEmpty                      10. ListInsert\n");
    printf("    	  5. ListLength                     11. ListDelete\n");
    printf("    	  6. GetElem                        12. ListTrabverse\n");

    // 输出横线
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }

    putchar('\n');

    // 输出额外的菜单选项
    printf("    	  13.reverseList                    14.RemoveNthFromEnd\n");
    printf("    	  15.sortList                       16.saveFile\n");
    printf("    	  17.getFile                        \n");
    printf("    	  0.exit                        \n");

    // 输出横线
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }

    putchar('\n');

    // 输出小猫咪
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

    putchar('\n');

    // 输出横线
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }

    putchar('\n');

    // 输出提示文字
    printf("    请选择你的操作[0~13]:");

    putchar('\n');

    // 输出横线
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }

    putchar('\n');
}

void menu()
{
    for(int k = 0; k<=  119 ;k++) // 打印分隔线，共120个 '-'
    {
        putchar('-');
    }
    putchar('\n'); // 打印换行符

    printf("1.创建一个线性表\n"); // 打印操作1的描述
    printf("2.删除一个线性表\n"); // 打印操作2的描述
    printf("3.查询已经创建的线性表\n"); // 打印操作3的描述
    printf("4.查找一个线性表和进行操作\n"); // 打印操作4的描述
    printf("0.退出线性表的管理\n"); // 打印操作0的描述

    //小猫保佑
    printf("   /\\_/\\\n");
    printf("  ( o.o )\n");
    printf("  > ^ <\n");
    printf(" /  -  \\\n");
    printf("/         \\\n");
    //猫猫冲击

    for(int k = 0; k<=  119 ;k++) // 其他同上，打印分隔线
    {
        putchar('-');
    }
    putchar('\n'); // 最后再打印一行空行，方便视觉上的分离
}

// 下方是一个插入操作的菜单，类似于前面的 menu()函数
void Menuofinsert()
{
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    putchar('\n');

    printf("1.插入多个元素\n");
    printf("2.定点插入元素\n");

    //小猫保佑
    printf("   /\\_/\\\n");
    printf("  ( o.o )\n");
    printf("  > ^ <\n");
    printf(" /  -  \\\n");
    printf("/         \\\n");
    //猫猫冲击

    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    putchar('\n');
}

int  compare(int a,int b)
{
    if(a == b)
    {
        return 1;
    }
    return 0;
}

void visit(int x)
{
    printf("%d",x);
}

void fun(LinkList &L)       //这个函数进行每个线性表的详细功能实现
{
    system("cls");  //清空命令行窗口
    printf("线性表存在鸭鸭\n");
    printf("现在对这个线性表进行操作\n");
    printf("别忘记初始化这个线性表鸭\n");

    //接下来会进行各种操作，要求用户输入命令
    int order;
    show_normal();  //显示命令列表
    scanf("%d",&order);  //读取用户输入的命令序号
    while (order)   //如果用户输入的命令不是0（退出），就继续循环
    {
        fflush(stdin); //清空输入缓冲区，防止影响下一次输入
        int feedback;   //来接收函数返回值
        switch (order) {    //根据命令序号进行相应的操作
            case 1:
                //创建线性表
                if (InitList(L) == OK)
                    printf("线性表创建成功！\n");
                else printf("线性表已经存在，创建失败！\n");
                break;
            case 2:
                //销毁线性表
                printf("现在进行线性表的销毁\n");
                feedback = DestroyList(L);
                if(feedback == OK)
                {
                    printf("成功销毁了\n");
                }
                else if(feedback == INFEASIBLE)
                {
                    printf("这个线性表不存在或未初始化,无法销毁\n");
                }
                break;
            case 3:
                //清空线性表
                printf("现在进行线性表的清空操作\n");
                feedback = ClearList(L);
                if(feedback == OK)
                {
                    printf("成功清空线性表\n");
                }
                break;
            case 4:
                //判断线性表是否为空
                printf("现在对线性表进行判空操作\n");
                ListEmpty(L);
                break;
            case 5:
                //求线性表的长度
                printf("现在进行求线性表的长度\n");
                feedback = ListLength(L);
                if(feedback != INFEASIBLE)
                {
                    printf("线性表的长度为:%d",feedback);
                }
                break;
            case 6:
                //获取线性表中指定位置的元素
                printf("现在进行元素获取操作\n");
                int e; int i;      //用e来接收元素的值，i是所要获取元素的位置
                printf("请输入你想获取第几个元素的值\n");
                scanf("%d",&i);
                feedback =  GetElem(L,i,e);
                if(feedback == OK)
                {
                    printf("成功获取，第%d个元素的值为：%d",i,e);
                }
                break;
            case 7:
                //查找线性表中指定值的元素
                printf("现在进行查找元素的操作\n");
                int ee;  //接收所要查找的元素
                printf("请输入你想查找的元素\n");
                scanf("%d",&ee);
                feedback =  LocateElem(L,ee,compare);
                if(feedback >0)
                {
                    printf("所要查找的元素是第%d个\n",feedback);
                }
                break;
            case 8:
                //查找指定元素的前驱
                printf("现在进行查找前驱的操作\n");
                int cur_e,pre_e;   //接收元素，并存储前驱
                printf("请输入你想查找哪个元素的前驱\n");
                scanf("%d",&cur_e);
                feedback =  PriorElem(L,cur_e,pre_e);
                if(feedback == OK)
                {
                    printf("你所要查找的前驱是:  %d\n",pre_e);
                }
                break;
            case 9:
                //查找指定元素的后驱
                printf("现在进行查找后驱的操作\n");
                int cur,next_e;   //接收元素，并存储后继
                printf("请输入你想查找哪个元素的后驱\n");
                scanf("%d",&cur);
                feedback = NextElem(L,cur,next_e);
                if(feedback == OK)
                {
                    printf("你所要查找的元素的后驱是:   %d\n",next_e);
                }
                break;
            case 10:
                //在指定位置插入元素
                printf("现在进行插入元素的操作\n");
                printf("请问你想在第几个位置插入元素\n");
                int position;   //接收元素的位置
                scanf("%d",&position);
                printf("请问你想插入元素的个数\n");
                int number;
                scanf("%d",&number);
                feedback =  ListInsert(L,position,number);
                if(feedback == OK)
                {
                    printf("插入成功\n");
                }
                break;
            case 11:
                //删除指定位置的元素
                printf("现在进行删除元素的操作\n");
                printf("请问你想删除第几个位置的元素\n");
                int position01;
                int e1;
                scanf("%d",&position01);
                feedback =  ListDelete(L,position01,e1);
                if(feedback == OK)
                {
                    printf("删除成功，删除的元素是:  %d\n",e1);
                }
                break;
            case 12:
                //遍历线性表
                printf("现在进行线性表的遍历\n");
                ListTraverse(L,visit);
                break;
            case 13:
                //翻转线性表
                printf("现在进行线性表的翻转\n");
                reverseList(L);
                break;
            case 14:
                //删除倒数第n个元素
                printf("现在进行删除链表倒数元素的操作\n");
                printf("你想删除链表倒数第几个节点\n");
                int positon2;
                scanf("%d",&positon2);
                RemoveNthFromEnd(L,positon2);
                break;
            case 15:
                //排序线性表
                printf("现在进行链表的排序\n");
                sortList(L);
                printf("搞定力\n");
                break;
            case 16:
                //将线性表保存到文件
                printf("现在进行线性表的文件保存\n");
                printf("请问你想保存到哪一个文件\n");
                char name1[30];
                scanf("%s",name1);
                savetofile(L,name1);
                break;
            case 17:
                //从文件中读取线性表
                printf("现在进行线性表的读取操作\n");
                printf("你想读取哪一个文件的资料\n");
                char name2[30];
                scanf("%s",name2);
                getfromfile(L,name2);
                break;
            default:
                //输入错误命令序号
                printf("命令输入有问题\n");
        }
        putchar('\n');
        printf("请输入下一个命令\n");
        scanf("%d",&order);
        system("cls");  //清空命令行窗口
        if(order != 0)
        {
            show_normal();  //显示命令列表
        }
        else{
            menu(); //返回主菜单
        }

    }
}
