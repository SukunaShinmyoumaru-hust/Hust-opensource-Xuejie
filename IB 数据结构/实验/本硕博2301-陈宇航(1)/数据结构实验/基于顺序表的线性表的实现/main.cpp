/* Linear Table On Sequence Structure*/
/*---- 头文件的申明 ----*/
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
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
typedef int status;

// 顺序表中数据元素的类型
typedef int ElemType;

// 定义顺序表的初始长度和每次扩展的长度
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

// 定义顺序表类型
typedef struct{
    ElemType * elem; // 存储数据元素的数组指针
    int length; // 当前长度
    int listsize; // 当前可容纳的最大长度
}SqList;

// 定义线性表集合中的每个线性表的类型
typedef struct{
    char name[30]; // 线性表的名称
    SqList L; // 线性表本身
}LIST_ELEM;

// 定义线性表集合类型
typedef struct{
    LIST_ELEM elem[10]; // 存储线性表的数组
    int length; // 当前集合长度
}LISTS;
LISTS Lists; // 声明线性表集合的变量名为Lists

/*---- 函数申明 ----*/

status InitList(SqList& L); //新建
status DestroyList(SqList& L); //销毁
status ClearList(SqList& L); //清空
status ListEmpty(SqList L);  //判空
status ListLength(SqList L);  //求长度
status GetElem(SqList L,int i,ElemType &e);  //获取元素
status LocateElem(SqList L,ElemType e,int (*p)(int ,int ));  //判断位置
status compare(int a ,int b);  //判断位置函数中调用的compare函数
status PriorElem(SqList L,ElemType e,ElemType &pre);  //获得前驱
status NextElem(SqList L,ElemType e,ElemType &next);  //获得后继
status ListInsert(SqList &L,int i,ElemType e);  //插入元素
status ListDelete(SqList &L,int i,ElemType &e);  //删除元素
status ListTraverse(SqList L,void (*visit)(int)); //遍历输出
void visit(int elem);  //遍历输出时候调用的visit函数
status MaxSubArray(SqList L);  //最大连续子数组
status SubArrayNum(SqList L , int u); //和为k的子数组个数
status sort(SqList &L);  //顺序表排序
void show();  //单个线性表的菜单
void show1();  //多个线性表的菜单
void menu();  //多个线性表的入口菜单
int savetofile(SqList L);  //线性表保存到文件
int getfromfile(SqList &L);  //从文件中读取线性表
status AddList(LISTS &Lists,char ListName[]);  //在Lists中增加一个空线性表
status RemoveList(LISTS &Lists,char ListName[]);  // Lists中删除一个线性表
status LocateList(LISTS Lists,char ListName[]);  // 在Lists中查找线性表
void funtion();  //多个线性表管理的封装函数
void showplus();  //附加功能的菜单





/*---- main主函数 ----*/
int  main()
{
    //修改控制台输出颜色
    system("color 0b");
    //显示功能菜单
    show();
    //输入操作编号
    int order;
    //声明一个线性表
    SqList L;
    scanf("%d",&order);
    while(order)
    {
        //清除屏幕上的内容
        system("cls");
        //根据操作编号执行相应的功能
        switch(order){
            case 1:
                //显示功能菜单
                show();
                //初始化线性表
                if(InitList(L)==OK) printf("线性表创建成功！\n");
                else printf("线性表已经存在，创建失败！\n");
                getchar();
                break;
            case 2:
                //显示功能菜单
                show();
                //销毁线性表
                DestroyList(L);
                getchar();
                break;
            case 3:
                //显示功能菜单
                show();
                //清空线性表
                ClearList(L);
                getchar();
                break;
            case 4:
                //显示功能菜单
                show();
                //判断线性表是否为空
                ListEmpty(L);
                getchar();
                break;
            case 5:
                //显示功能菜单
                show();
                //获取线性表长度
                int getdata ;
                getdata = ListLength(L);
                if(getdata !=INFEASIBLE )
                {
                    printf("线性表的长度是%d",getdata);
                }
                getchar();
                break;
            case 6:
                //显示功能菜单
                show();
                //获取指定位置的元素
                printf("请输入你想获取第几个元素\n");
                int i ;
                scanf("%d",&i);
                int n ; int getdata1 ;
                n = GetElem(L,i,getdata1);

                if(n == OK)
                {
                    printf("成功获取到第%d个元素的值：%d\n",i,getdata1);
                }
                getchar();
                break;
            case 7:
                //显示功能菜单
                show();
                //查找指定元素
                int u ;
                printf("请输入一个数值e\n");
                int e ; scanf("%d",&e);
                printf("你想在表里查找一个比e大还是小的数据，大请输入1，小请输入0\n");
                int getorder ;  scanf("%d",&getorder);
                int q;
                if(getorder == 1)
                {
                    printf("你想要这个数据比e大多少\n");

                    scanf("%d",&q);
                    if(LocateElem(L,e+q,compare) > 0)
                    {
                        printf("你要查找到数据的下标的是%d\n",LocateElem(L,e+q,compare));
                    }
                }
                else{
                    printf("你想要这个数据比e小多少\n");

                    scanf("%d",&q);
                    if(LocateElem(L,e+q,compare) > 0)
                    {
                        printf("你要查找到数据的下标的是%d\n",LocateElem(L,e+q,compare));
                    }
                }
                getchar();
                break;
            case 8:
                //显示功能菜单
                show();
                //获取指定元素的前驱
                printf("请问你想获得那个元素的前驱\n");
                int v ;
                scanf("%d",&v);
                int pre_e;
                if(PriorElem(L,v,pre_e)==OK)
                {
                    printf("成功获得前驱，是%d\n",pre_e);
                }
                getchar();
                break;
            case 9:
                //显示功能菜单
                show();
                //获取指定元素的后继
                printf("请问你想获得哪个元素的后驱\n");
                int p ;
                scanf("%d",&p);
                int next_e;
                if(NextElem(L,p,next_e) == OK)
                {
                    printf("成功获取后驱，是%d\n",next_e);
                }
                getchar();
                break;
            case 10:
                //显示功能菜单
                show();
                //在指定位置之前插入元素
                printf("请问你想在第几个位置之前插入元素\n");
                int r ;
                scanf("%d",&r);
                printf("插入的元素的值为\n");
                int a ;
                scanf("%d",&a);
                if(ListInsert(L,r,a) == OK)
                {
                    printf("插入成功\n");
                }
                getchar();
                break;
            case 11:
                //显示功能菜单
                show();
                //删除指定位置的元素
                printf("请问你想删除第几个数据元素\n");
                int b ;
                scanf("%d",&b);
                int ee;
                if(ListDelete(L,b,ee) == OK)
                {
                    printf("删除的数据元素是%d\n",ee);
                }
                getchar();
                break;
            case 12:
                //显示功能菜单
                show();
                //遍历线性表
                if(ListTraverse(L,visit) )
                {
                    printf("\n成功遍历\n");
                }
                getchar();
                break;
            case 13:
                //显示函数列表
                funtion();
                //清除屏幕上的内容
                system("cls");
                //显示功能菜单
                show();
                break;
            case 0:
                break;
        }//end of switch
        //再次输入操作编号
        scanf("%d",&order);
    }//end of while
    //退出程序
    printf("欢迎下次再使用本系统！\n");
    system("pause");
    return 0;
}








// 1. 初始化表：函数名称是InitList(L)；初始条件是线性表L不存在；操作结果是构造一个空的线性表；
status InitList(SqList& L)//线性表L不存在，构造一个空的线性表；返回OK，否则返回INFEASIBLE。
{
// 请在这里补充代码，完成本关任务
/********** Begin *********/
    if(L.elem)
        return INFEASIBLE;//线性表已存在，返回线性表不存在的错误代码

    L.elem = (ElemType *)malloc(sizeof(ElemType)*LIST_INIT_SIZE);//为线性表分配内存空间
    L.length = 0;//将线性表的长度设置为0
    L.listsize = LIST_INIT_SIZE;//设置线性表的容量

    return OK;//返回操作成功的代码

/********** End **********/
}

// 2. 销毁表：函数名称是DestroyList(L)；初始条件是线性表L已存在；操作结果是销毁线性表L；
status DestroyList(SqList& L)//如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
// 请在这里补充代码，完成本关任务
/********** Begin *********/
    if(!L.elem)//如果线性表不存在，返回线性表不存在的错误代码
    {
        printf("线性表不存在\n");
        return INFEASIBLE;
    }

    free(L.elem);//释放线性表中元素的内存空间
    L.elem = NULL;//将线性表指针置为空指针
    L.length = 0;//将线性表长度置为0
    L.listsize = 0;//将线性表容量置为0
    printf("成功销毁线性表\n");
    return OK;//返回操作成功的代码

/********** End **********/
}
// 3.清空表：函数名称是ClearList(L)；初始条件是线性表L已存在；操作结果是将L重置为空表；
status ClearList(SqList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
// 判断线性表是否存在或是否为空
    if(!L.length || !L.elem )
    {
        printf("笨蛋，线性表不存在或者没有元素\n");
        return INFEASIBLE;

    }

    L.length = 0;   // 将线性表长度设为0，相当于清空了线性表
    printf("成功删除线性表里面的元素啦\n");
    return OK;
}

//4. 判定空表：函数名称是ListEmpty(L)；初始条件是线性表L已存在；操作结果是若L为空表则返回TRUE,否则返回FALSE；
status ListEmpty(SqList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
// 判断线性表是否存在
    if(!L.elem)
    {
        printf("猪头，线性表不存在\n");
        return INFEASIBLE;
    }

// 判断线性表是否为空
    if(L.length==0)
    {
        printf("线性表是空的\n");
        return TRUE;
    }
    printf("线性表不是空的\n");
    return FALSE;
}

// 5.求表长：函数名称是ListLength(L)；初始条件是线性表已存在；操作结果是返回L中数据元素的个数；
status ListLength(SqList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
// 判断线性表是否存在
    if(L.elem==NULL)
    {
        printf("线性表不存在\n");
        return INFEASIBLE;
    }

// 返回线性表长度
    else {
        return L.length;
    }
}

// 6.获得元素：函数名称是GetElem(L,i,e)；初始条件是线性表已存在，1≤i≤ListLength(L)；操作结果是用e返回L中第i个数据元素的值；
status GetElem(SqList L,int i,ElemType &e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
// 判断线性表是否存在
    if(!L.elem)
    {
        printf("线性表不存在\n");
        return INFEASIBLE;
    }

// 判断线性表序号i的合法性
    if(i<1 || i>L.length)
    {
        printf("获取的元素i不合法\n");
        return ERROR;
    }

// 获取线性表第i个元素
    e = L.elem[i-1];
    {
        return OK;
    }
}

// 7.查找元素：函数名称是LocateElem(L,e,compare())；初始条件是线性表已存在；操作结果是返回L中第1个与e满足关系compare（）关系的数据元素的位序，若这样的数据元素不存在，则返回值为0；
int LocateElem(SqList L,ElemType e,int (*p)(int ,int ))
// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE。
{
// 判断线性表是否存在
    if(!L.elem)
    {
        printf("笨蛋，线性表不存在\n");
        return INFEASIBLE;
    }

// 在线性表中查找元素，若找到返回位置序号
    for(int k=0;k<L.length;k++)
    {
        if(compare(L.elem[k],e))
        {
            return k+1;
        }
    }

// 没有找到符合的元素
    printf("抱歉，没找到符合的元素");
    return ERROR;
}
// 8. 获得前驱：函数名称是PriorElem(L,cur_e,pre_e)；初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，否则操作失败，pre_e无定义
status PriorElem(SqList L, ElemType cur_e, ElemType &pre_e) {
    // 如果线性表L为空，返回INFEASIBLE
    if (!L.elem) {
        printf("Error: 该线性表不存在。\n");
        return INFEASIBLE;
    }

    // 如果要获取前驱的元素是第一个，返回ERROR
    if (L.elem[0] == cur_e) {
        printf("Error: 要获取前驱的元素是第一个，不存在前驱。\n");
        return ERROR;
    }

    // 查询要获取前驱的元素在表中的位置
    for (int k = 1; k < L.length; k++) {
        if (L.elem[k] == cur_e ) {
            pre_e = L.elem[k-1];
            return OK;
        }
    }

    // 如果要获取前驱的元素不存在，返回ERROR
    printf("Error: 该元素不存在于该线性表中。\n");
    return ERROR;
}

// 9. 获得后继：函数名称是NextElem(L,cur_e,next_e)；初始条件是线性表L已存在；操作结果是若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继，否则操作失败，next_e无定义；
status NextElem(SqList L, ElemType cur_e, ElemType &next_e) {
    // 如果线性表L为空，返回INFEASIBLE
    if (!L.elem) {
        printf("Error: 该线性表不存在。\n");
        return INFEASIBLE;
    }

    // 如果要获取后继的元素是最后一个，返回ERROR
    if (L.elem[L.length-1] == cur_e) {
        printf("Error: 要获取后继的元素是最后一个，不存在后继。\n");
        return ERROR;
    }

    // 查询要获取后继的元素在表中的位置
    for (int k = 0; k < L.length-1; k++) {
        if (L.elem[k] == cur_e) {
            next_e = L.elem[k+1];
            return OK;
        }
    }

    // 如果要获取后继的元素不存在，返回ERROR
    printf("Error: 该元素不存在于该线性表中。\n");
    return ERROR;
}

// 10.插入元素，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
status ListInsert(SqList &L, int i, ElemType e)
{
    if (!L.elem) {
// 线性表为空
        printf("线性表为空\n");
        return INFEASIBLE;
    }

    if (i < 1 || i > L.length + 1) {
        // 插入位置不正确
        printf("插入位置不正确\n");
        return ERROR;
    }

    int k = i - 1;
    if (L.length == L.listsize) {
        // 顺序表已满，需要重新分配空间
        ElemType * newbase = (ElemType *)realloc(L.elem, sizeof(ElemType) * (L.listsize + LISTINCREMENT));
        if (!newbase)
            return ERROR;
        L.elem = newbase;
        L.listsize += LISTINCREMENT;
    }

// 将位置k及其后面的元素后移一位
    for (int p = L.length - 1; p >= k; p--) {
        L.elem[p + 1] = L.elem[p];
    }

// 插入元素e
    L.length++;
    L.elem[k] = e;
    return OK;
}

// 11.删除元素，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
status ListDelete(SqList &L, int i, ElemType &e)
{
    if (!L.elem) {
// 线性表为空
        printf("线性表为空\n");
        return INFEASIBLE;
    }

    if (i < 1 || i > L.length) {
        // 删除位置不正确
        printf("删除位置不正确\n");
        return ERROR;
    }

    e = L.elem[i - 1];
// 将位置i后面的元素前移一位
    for (int k = i - 1; k < L.length - 1; k++) {
        L.elem[k] = L.elem[k + 1];
    }
    L.length--;
    return OK;
}

// 12.遍历表，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
status ListTraverse(SqList L, void (*visit)(int))
{
    if (!L.elem) {
// 线性表为空
        printf("线性表为空\n");
        return INFEASIBLE;
    }

    for (int k = 0; k < L.length; k++) {
        // 调用visit对每个元素进行操作
        visit(L.elem[k]);
        if (k != L.length - 1) {
            putchar(' ');
        }
    }
    return OK;
}

int compare(int a ,int b)
{
    if(a == b)
    {
        return 1;
    }
    return 0;
}
//遍历输出时候调用的visit函数
void visit(int elem)
{
    printf("%d",elem);
}
//求最大连续子数组
int MaxSubArray(SqList L)
{
    if(!L.elem) //如果线性表不存在
    {
        printf("笨蛋，线性表不存在");
        return INFEASIBLE;
    }
    int max ,current ; //定义max表示最大值，current表示当前值
    max = current = L.elem[0]; //初始化max和current为线性表L的第一个元素
    for(int k = 1;k<L.length;k++) //遍历线性表L
    {
        if(current <= 0) //如果当前值小于等于0
        {
            current = L.elem[k]; //将当前值置为下一个元素的值
        } else{
            current+=L.elem[k]; //将当前值做累加操作
        }
        if(max < current) //如果最大值小于当前值
        {
            max = current; //更新最大值为当前值
        }
    }
    return max ; //返回最终最大值
}
//计算线性表L中和为u的子数组个数
int SubArrayNum(SqList L, int u)
{
    if(!L.elem)
    {
        printf("笨蛋，线性表不存在");
        return INFEASIBLE; //返回线性表不存在的错误代码
    }
    int count = 0; //子数组个数计数器
    for(int k = 0; k < L.length; k++)
    {
        int sum = 0; //子数组元素之和
        for(int i = k; i < L.length; i++)
        {
            sum += L.elem[i]; //累加元素值
            if(sum == u)
            {
                count ++ ; //子数组和为u，计数器加一
            }
        }
    }
    return count ;
}//冒泡排序实现顺序表排序
int sort(SqList &L)
{
    for(int k = 0; k < L.length - 1; k++)//控制轮数
    {
        for(int i = 0; i < L.length - 1 - k; i++)//每轮比较相邻元素
        {
            if(L.elem[i] > L.elem[i+1])//如果左边元素大于右边元素
            {
                int tmp = L.elem[i];//交换两个元素的位置
                L.elem[i] = L.elem[i+1];
                L.elem[i+1] = tmp;
            }
        }
    }
}

/* 输出程序的菜单 */
void show()
{
    for(int k = 0; k<= 119 ;k++) // 打印分割线
    {
        putchar('-');
    }
    printf("           Menu for Linear Table On Sequence Structure \n");
    printf("    	  1. InitList                       7. LocateElem\n");
    printf("    	  2. DestroyList                    8. PriorElem\n");
    printf("    	  3. ClearList                      9. NextElem \n");
    printf("    	  4. ListEmpty                      10. ListInsert\n");
    printf("    	  5. ListLength                     11. ListDelete\n");
    printf("    	  6. GetElem                        12. ListTrabverse\n");
    printf("    	  13. plusfunction                        \n");
    printf("    	  0.exit                        \n");
    for(int k = 0; k<= 119 ;k++) // 打印分割线
    {
        putchar('-');
    }
    putchar('\n');
// 下面是一个带有动态图像的输出，可以略过
    printf(" 请选择你的操作[0~13]:");
    putchar('\n');
    for(int k = 0; k<= 119 ;k++)
    {
        putchar('-');
    }

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
}

void show1()
{
    // 打印菜单前的分隔符
    printf("这是多个线性表管理中的菜单\n");
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    // 打印菜单内容
    printf("           Menu for Linear Table On Sequence Structure \n");
    printf("    	  1. InitList                       7. LocateElem\n");
    printf("    	  2. DestroyList                    8. PriorElem\n");
    printf("    	  3. ClearList                      9. NextElem \n");
    printf("    	  4. ListEmpty                      10. ListInsert\n");
    printf("    	  5. ListLength                     11. ListDelete\n");
    printf("    	  6. GetElem                        12. ListTrabverse\n");
    printf("       0.exit\n");
    // 打印菜单后的分隔符
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
    // 打印额外的一些菜单内容
    printf("    请选择你的操作[0~1]:");
    putchar('\n');
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }
    // 打印一段 ASCII 艺术
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

}

void menu()
{
    printf("1.创建一个线性表\n");
    printf("2.删除一个线性表\n");
    printf("3.查找一个线性表和进行操作\n");
    printf("0.退出线性表的管理\n");

}

// 函数名：savetofile
// 功能：将线性表 L 中的元素保存到文件中
// 输入参数：线性表 L
// 返回值：操作成功返回 OK，否则返回 INFEASIBLE
int savetofile(SqList L)
{
    printf("请输入你想保存到的文件名\n"); // 提示输入要保存的文件名
    char arr[30];
    scanf("%s",arr); // 获取用户输入的文件名

    FILE *fp;
    fp = fopen(arr,"w"); // 以写入方式打开文件
    if(fp == NULL){ // 如果文件打开失败
        printf("error"); // 输出错误信息
        return INFEASIBLE; // 返回错误代码
    }

    int i ;
    for( i =0; i< L.length; i++) // 遍历线性表中的元素
    {
        fprintf(fp,"%d ",L.elem[i]); // 将元素写入文件中
    }

    fclose(fp); // 关闭文件
    return OK; // 返回操作成功代码
}

// 函数名：getfromfile
// 功能：从文件中读取线性表 L 的元素
// 输入参数：线性表 L 的地址
// 返回值：操作成功返回 OK，否则返回 INFEASIBLE
int getfromfile(SqList &L)
{
    if(!L.elem) // 如果线性表不存在
    {
        printf("笨蛋，线性表不存在\n"); // 输出错误信息
        return INFEASIBLE; // 返回错误代码
    }

    printf("请输入你要读取的文件名:\n"); // 提示输入要读取的文件名
    char name[30] = {'\0'};
    scanf("%s",name); // 获取用户输入的文件名

    FILE *fp = fopen(name,"r"); // 以只读方式打开文件
    if(fp == NULL){ // 如果文件打开失败
        printf("error"); // 输出错误信息
        return INFEASIBLE; // 返回错误代码
    }

    int j;
    while(fscanf(fp,"%d ",&j) !=EOF) // 读取文件中的元素
    {
        L.elem[L.length++] = j; // 将元素添加到线性表中
    }

    fclose(fp); // 关闭文件
    return OK; // 返回操作成功代码
}

// 本函数的功能为在线性表管理系统Lists中增加一个名为ListName的空线性表
// 如果已经存在名为ListName的线性表，则返回INFEASIBLE，否则返回OK
status AddList(LISTS &Lists,char ListName[])
{
// 遍历已有线性表，查找是否已经存在名为ListName的线性表
    for(int i = 0; i<Lists.length ;i++)
    {
        if(strcmp(ListName,Lists.elem[i].name) == 0)
        {
            printf("这个名字的线性表已经存在了");
            return INFEASIBLE;
        }
    }

// 如果不存在名为ListName的线性表，则在Lists中添加一个新的线性表
    Lists.length++; // 在线性表管理系统中增加一个线性表
    int n = 0; // 初始化线性表的长度n为0，即线性表为空

// 将新线性表的名字设置为ListName
    strcpy(Lists.elem[Lists.length-1].name ,ListName);

// 将新线性表的数据初始化为空，即没有元素
    Lists.elem[Lists.length-1].L.elem=NULL;

// 注释掉的InitList不需要调用，因为L.elem已设置为NULL
// InitList(Lists.elem[Lists.length-1].L);

// 返回添加线性表操作执行成功
    return OK;
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    // 遍历线性表
    for(int k = 0;k<Lists.length;k++)
    {
        // 找到名称为ListName的线性表
        if(strcmp(Lists.elem[k].name,ListName)==0)
        {
            // 销毁线性表
            DestroyList(Lists.elem[k].L);
            // 将后面的线性表前移，覆盖当前位置
            for(int i = k;i<Lists.length-1;i++)
            {
                strcpy(Lists.elem[i].name, Lists.elem[i+1].name);   //注意要名字和结构体一起移动，我感觉没有捷径
                Lists.elem[i].L.elem =Lists.elem[i+1].L.elem;
                Lists.elem[i].L.length = Lists.elem[i+1].L.length;
                Lists.elem[i].L.listsize =Lists.elem[i+1].L.listsize;
            }
            Lists.length--; // 线性表数量减一
            return OK;
        }
    }
    // 没有找到目标线性表，返回错误
    return ERROR;

    /********** End **********/
}

int LocateList(LISTS Lists,char ListName[])
// 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回-1
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    // 循环遍历线性表数组
    for(int k = 0 ;k< Lists.length;k++)
    {
        // 如果找到了指定名称的线性表
        if(strcmp(Lists.elem[k].name,ListName)==0)
        {
            // 返回该线性表的逻辑序号（索引位置+1）
            return k+1;
        }
    }
    // 如果未找到指定名称的线性表，则返回-1
    return -1;

    /********** End **********/
}


void funtion()
{
    showplus();  // 显示菜单页面头部
    SqList S ;  // 用于操作的线性表，S代表单个线性表

    int order ;
    scanf("%d",&order);   // 获取用户输入的命令
    while(order) {    // 如果用户输入了命令，即非0，则进入循环

        system("cls");  // 清空控制台屏幕
        showplus();    // 重新显示菜单页面头部

        switch (order) {   // 根据用户命令进行相应操作
            case 1:
                // 创建线性表

                S.elem = (ElemType *) malloc(sizeof(ElemType) * LIST_INIT_SIZE);   // 为线性表分配内存空间
                S.length = 0;   // 初始化线性表长度为0
                S.listsize = LIST_INIT_SIZE;   // 初始化线性表可用大小为LIST_INIT_SIZE


                printf("请输入一串数据，数据之间用空格隔开,最后一个数据输入完直接回车\n");
                int number;
                scanf("%d", &number);
                char c;
                c = getchar();
                while (c != '\n') {   // 当输入未结束时，循环获取数据并添加到线性表中
                    S.elem[S.length++] = number;
                    scanf("%d", &number);
                    c = getchar();
                }
                S.elem[S.length++] = number;  // 将最后一个数据添加到线性表中
                break;
            case 2:
                // 寻找最大子序列
                int r;

                r = MaxSubArray(S);   // 调用MaxSubArray函数计算最大子序列之和
                if (r != -1) {
                    printf("最大连续子序列之和是%d\n", r);
                }
                // printf("请输入下一个命令\n");
                break;
            case 3:
                // 寻找和为K的连续子序列
                printf("这个函数来寻找一个和为K的连续子序列\n");
                int k;
                printf("请输入一个K值\n");
                scanf("%d", &k);
                if (SubArrayNum(S, k) != -1) {   // 调用SubArrayNum函数在线性表中查找和为K的连续子序列
                    printf("线性表中和为k的连续子列数量为%d\n", SubArrayNum(S, k));
                }
                break;
            case 4:
                // 对线性表进行排序
                sort(S);    // 调用sort函数对线性表进行排序
                printf("排完序后的线性表如下:\n");
                ListTraverse(S, visit);   // 使用ListTraverse函数遍历并输出线性表中的数据
                putchar('\n');
                break;
            case 5:
                // 将线性表保存到文件中
                printf("现在进行线性表的文件形式保存\n");
                int message;
                message =  savetofile(S);  // 调用savetofile函数将线性表保存到文件中
                if(message == OK)
                {
                    printf("存储成功\n");
                    //printf("请输入下一个命令\n");
                }
                break;
            case 6:
                // 从文件中读取线性表数据
                printf("现在进行文件的读取\n");
                SqList p;
                p.elem = NULL;
                InitList(p);   //用这个线性表来存储读取的数据
                if(getfromfile(p) == OK)   // 调用getfromfile函数从文件中读取数据并保存到p中
                {
                    printf("读取成功\n读取的数据如下:\n");
                    ListTraverse(p,visit);  // 使用ListTraverse函数遍历并输出线性表中的数据
                }
                else {
                    printf("读取失败");
                }
                //printf("请输入下一个命令\n");
                break;
                // 进行多个线性表的管理
            case 7:
                printf("现在进行多个线性表的管理\n");
                menu(); // 显示菜单
                int a;
                scanf("%d", &a);
                while (a) {
                    switch (a) {
                        // 创建线性表
                        case 1:
                            printf("现在进行创建线性表\n");
                            printf("请输入你想创建的线性表的名字\n");
                            char name1[30];
                            scanf("%s", name1);
                            int u;
                            u = AddList(Lists, name1); // 添加线性表
                            if (u == OK) {
                                printf("创建成功啦\n");
                            }
                            if (u == INFEASIBLE) {
                                system("pause");
                            }
                            break;
                            // 删除线性表
                        case 2:
                            printf("现在进行删除线性表\n");
                            printf("请输入你想创建的线性表的名字\n");
                            char name2[30];
                            scanf("%s", name2);
                            RemoveList(Lists, name2); // 删除线性表
                            break;
                            // 查找和操作线性表
                        case 3:
                            printf("现在进行线性表的查找和操作\n");
                            printf("请输入你想查找和操作的线性表的名字\n");
                            char name3[30];
                            scanf("%s", name3);
                            int judge;
                            judge = LocateList(Lists, name3); // 查找线性表
                            if (judge == -1) {
                                printf("不存在这个线性表\n");
                                system("pause");
                            } else {
                                printf("线性表存在鸭鸭\n");
                                printf("现在对这个线性表进行操作\n");
                                int order;
                                show(); // 显示线性表操作菜单
                                scanf("%d", &order);
                                while (order) {
                                    switch (order) {
                                        // 初始化线性表
                                        case 1:
                                            show1(); // 显示操作提示
                                            if (InitList(Lists.elem[judge - 1].L) == OK)
                                                printf("线性表创建成功！\n");
                                            else printf("线性表已经存在，创建失败！\n");
                                            getchar();
                                            break;
                                            // 删除线性表
                                        case 2:
                                            show1();
                                            DestroyList(Lists.elem[judge - 1].L); // 销毁线性表
                                            getchar();
                                            break;
                                            // 清空线性表
                                        case 3:
                                            show1();
                                            ClearList(Lists.elem[judge - 1].L); //清空线性表
                                            getchar();
                                            break;
                                            // 判断线性表是否为空
                                        case 4:
                                            show1();
                                            ListEmpty(Lists.elem[judge - 1].L);
                                            getchar();
                                            break;
                                            // 获取线性表的长度
                                        case 5:
                                            show1();
                                            int getdata;
                                            getdata = ListLength(Lists.elem[judge - 1].L);
                                            if (getdata != INFEASIBLE) {
                                                printf("线性表的长度是%d", getdata);
                                            }
                                            getchar();
                                            break;
                                            // 获取线性表中的元素
                                        case 6:
                                            show1();
                                            printf("请输入你想获取第几个元素\n");
                                            int i;
                                            scanf("%d", &i);
                                            int n;
                                            int getdata1;
                                            n = GetElem(Lists.elem[judge - 1].L, i, getdata1);
                                            if (n == OK) {
                                                printf("成功获取到第%d个元素的值：%d\n", i, getdata1);
                                            }
                                            getchar();
                                            break;
                                            // 查找线性表中的元素
                                        case 7:
                                            show1();
                                            int u;
                                            printf("请输入一个数值e\n");
                                            int e;
                                            scanf("%d", &e);
                                            printf("你想在表里查找一个比e大还是小的数据，大请输入1，小请输入0\n");
                                            int getorder;
                                            scanf("%d", &getorder);
                                            int q;
                                            if (getorder == 1) {
                                                printf("你想要这个数据比e大多少\n");
                                                scanf("%d", &q);
                                                if (LocateElem(Lists.elem[order - 1].L, e + q, compare) > 0) {
                                                    printf("你要查找到数据的下标的是%d\n", LocateElem(Lists.elem[judge - 1].L, e + q, compare));
                                                }
                                            } else {
                                                printf("你想要这个数据比e小多少\n");
                                                scanf("%d", &q);
                                                if (LocateElem(Lists.elem[judge - 1].L, e + q, compare) > 0) {
                                                    printf("你要查找到数据的下标的是%d\n", LocateElem(Lists.elem[judge - 1].L, e + q, compare));
                                                }
                                            }
                                            getchar();
                                            break;
                                            // 获取线性表中某个元素的前驱
                                        case 8:
                                            show1();
                                            printf("请问你想获得那个元素的前驱\n");
                                            int v;
                                            scanf("%d", &v);
                                            int pre_e;
                                            if (PriorElem(Lists.elem[judge - 1].L, v, pre_e) == OK) {
                                                printf("成功获得前驱，是%d\n", pre_e);
                                            }
                                            getchar();
                                            break;
                                            // 获取线性表中某个元素的后驱
                                        case 9:
                                            show1();
                                            printf("请问你想获得哪个元素的后驱\n");
                                            int p;
                                            scanf("%d", &p);
                                            int next_e;
                                            if (NextElem(Lists.elem[judge - 1].L, p, next_e) == OK) {
                                                printf("成功获取后驱，是%d\n", next_e);
                                            }
                                            getchar();
                                            break;
                                            // 在线性表中插入元素
                                        case 10:
                                            show1();
                                            printf("请问你想在第几个位置之前插入元素\n");
                                            int r;
                                            scanf("%d", &r);
                                            printf("插入的元素的值为\n");
                                            int a;
                                            scanf("%d", &a);
                                            if (ListInsert(Lists.elem[judge - 1].L, r, a) == OK) {
                                                printf("插入成功\n");
                                            }
                                            getchar();
                                            break;
                                            // 在线性表中删除元素
                                        case 11:
                                            show1();
                                            printf("请问你想删除第几个数据元素\n");
                                            int b;
                                            scanf("%d", &b);
                                            int ee;
                                            if (ListDelete(Lists.elem[judge - 1].L, b, ee) == OK) {
                                                printf("删除的数据元素是%d\n", ee);
                                            }
                                            getchar();
                                            break;
                                            // 遍历线性表
                                        case 12:
                                            show1();
                                            if (ListTraverse(Lists.elem[judge - 1].L, visit)) {
                                                printf("\n成功遍历\n");
                                            }
                                            getchar();
                                            break;
                                        case 0:
                                            break;
                                    }
                                    scanf("%d", &order);
                                }
                            }
                            break;
                        case 0:
                            break;
                    }
                    system("cls");
                    menu(); // 显示菜单
                    printf("请输入下一个命令\n");
                    scanf("%d", &a);
                }
                break;
            default:
                printf("输入的命令不正确，请再次输入\n");
                break;
        }
        printf("请输入下一个命令\n");
        scanf("%d", &order);
    }
}

void showplus()
{
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
    printf("这是一些附加的功能，和之前的线性表不兼容\n");
    printf("    	  1.初始化线性表\n");
    printf("    	  2.maxSubarray                     3.subarrayNum\n");
    printf("    	  4.sortList                        5.savetofile\n");
    printf("    	  6.getfromfile                     7.managelist\n");
    printf("    	  0. Exit\n");
    for(int k = 0; k<=  119 ;k++)
    {
        putchar('-');
    }  putchar('\n');
}
