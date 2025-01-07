#include"GenerateSudoku.h"
#include "definition.h"
#include"vector.hpp"

// col mask
// +-------+-------+-------+
// | 1 2 3 | 4 5 6 | 7 8 9 |
// | 1 2 3 | 4 5 6 | 7 8 9 |
// | 1 2 3 | 4 5 6 | 7 8 9 |
// +-------+-------+-------+
// | 1 2 3 | 4 5 6 | 7 8 9 |
// | 1 2 3 | 4 5 6 | 7 8 9 |
// | 1 2 3 | 4 5 6 | 7 8 9 |
// +-------+-------+-------+
// | 1 2 3 | 4 5 6 | 7 8 9 |
// | 1 2 3 | 4 5 6 | 7 8 9 |
// | 1 2 3 | 4 5 6 | 7 8 9 |
// +-------+-------+-------+

enum {
    COL_MASK_NONE = 0,
    COL_MASK_1 = 1 << 0,  // 0x001
    COL_MASK_2 = 1 << 1,  // 0x002
    COL_MASK_3 = 1 << 2,  // 0x004
    COL_MASK_4 = 1 << 3,  // 0x008
    COL_MASK_5 = 1 << 4,  // 0x010
    COL_MASK_6 = 1 << 5,  // 0x020
    COL_MASK_7 = 1 << 6,  // 0x040
    COL_MASK_8 = 1 << 7,  // 0x080
    COL_MASK_9 = 1 << 8,  // 0x100
    COL_MASK_123 = (1 << 0) | (1 << 1) | (1 << 2),  // 0x007
    COL_MASK_456 = (1 << 3) | (1 << 4) | (1 << 5),  // 0x038
    COL_MASK_789 = (1 << 6) | (1 << 7) | (1 << 8),  // 0x1C0
    COL_MASK_ALL = (1 << 9) - 1  // 0x1FF
};
//根据BIT位定义每一列，是否填充了数字；方便对不同行进行位或操作，计算列是否已经填充。
//初始化
void InitSodoku(Sodoku* sodoku)
{
    if (sodoku == nullptr) {
        return;
    }

    std::memset(sodoku, 0, sizeof(Sodoku));
	sodoku->long_row = -1; //初始化为-1，表示还未开始搜索
}
//判断数独是否完成
bool isSodokuInProgress(const Sodoku* sodoku) {
    if (sodoku == nullptr) {
        return false;
    }

    return sodoku->long_row <= 80;
}
//跳到下一行
void moveNextRow(Sodoku* sodoku)
{
	if (sodoku == nullptr) //如果数独为空
    {
        return;
    }

	sodoku->long_row++; //行数加1
	if (sodoku->long_row > 80)//表示数独已经完成
	{
		return;
	}
 
	sodoku->col_index[sodoku->long_row].base_col = calcRandCol();//计算起始搜索列为随机列
	sodoku->col_index[sodoku->long_row].curr_col = -1; //初始化当前列为-1表示还未开始搜索

	//计算并更新当前行的列掩码
    calculateColumnMaskForRow(sodoku, sodoku->long_row);
}
//回滚到上一行
void rollback_row(Sodoku* sodoku)
{
    if (sodoku == nullptr)
        return;

    sodoku->long_row--;
  
    // 重新计算
    calculateColumnMaskForRow(sodoku, sodoku->long_row);
}
//计算随机列
inline int calcRandCol(void)
{ 
	int col = rand() % 9; // 0-8
    return col;
}
//计算下一列
 int calNextCol(int col)
{
	 return (col + 1) % 9; // 0-8 -> 1-9
}
 //计算列掩码
 void calculateColumnMaskForRow(Sodoku* sodoku, int long_row)
{
  
     
    int row;
    unsigned int mask = 0;
	//1表示已被放置，0表示未被放置
	// 遍历前面数字在该行已经填充的列，计算列掩码
    for (row = long_row % 9; row < long_row; row += 9)
    {
        mask |= sodoku->col_index[row].col_mask;
    }

	//遍历当前数字已填的列，计算列掩码
    for (row = long_row - long_row % 9; row < long_row; row++)
    {
        mask |= sodoku->col_index[row].col_mask;
    }

	//九宫格内的列掩码
	for (row = long_row - long_row % 3; row < long_row; row++)//从起始行遍历到当前行
	{
		if (sodoku->col_index[row].col_mask & COL_MASK_123)
            mask |= COL_MASK_123;
        else if (sodoku->col_index[row].col_mask & COL_MASK_456)
            mask |= COL_MASK_456;
        else //789
            mask |= COL_MASK_789;
    }
	////主对角线的列掩码
 //  for (row = long_row - long_row % 9; row < long_row; row++)
 //   {
 //      
 //       if (sodoku->col_index[row].col_mask & (1 << (row % 9)))
 //       {
	//		mask |= (1 << (long_row % 9));
 //           break;
 //       }
 //   }
 //  //副对角线的列掩码
 //  for (row = long_row - long_row % 9; row < long_row; row++)
 //  {

 //      if (sodoku->col_index[row].col_mask & (1 << (8-row % 9)))
 //      {
 //          mask |= (1 << (8-long_row % 9));
 //          break;
 //      }
 //  }

    sodoku->col_index[long_row].col_mask = ~mask; // 取反！
}
 //跳到下一列

 bool moveNextCol(Sodoku* sodoku)
 {
     bool success = false; // 指示操作是否成功
     int base_col; // 当前行的基准列索引
     int curr_col; // 当前行的当前列索引
     int next_col; // 要找到的下一列索引
     bool found = false; // 指示是否找到有效的下一列

     if (sodoku == nullptr)
         return success; // 如果数独为空，返回false

     base_col = sodoku->col_index[sodoku->long_row].base_col;
     curr_col = sodoku->col_index[sodoku->long_row].curr_col;

     if (curr_col == -1)
     {
         // 如果当前列是-1，从base_col开始找到第一个有效列
         next_col = base_col;

         // 检查base_col是否有效
         if (sodoku->col_index[sodoku->long_row].col_mask & (1 << base_col))
         {
             found = true;
         }
         else
         {
             // 如果base_col无效，找到下一个有效列
             next_col = calNextCol(base_col);
         }
     }
     else
     {
         // 从当前列找到下一个有效列
         next_col = calNextCol(curr_col);
     }

     if (!found)
     {
         // 继续搜索直到找到有效列或循环回到base_col
         while (next_col != base_col)
         {
             // 检查next_col是否有效
             if (sodoku->col_index[sodoku->long_row].col_mask & (1 << next_col))
             {
                 found = true;
                 break;
             }

             // 找到下一列
             next_col = calNextCol(next_col);
         }
     }

     if (!found)
         return success; // 如果没有找到有效列，返回false

     // 设置当前列为找到的下一列
     sodoku->col_index[sodoku->long_row].curr_col = next_col;
     // 更新列掩码以指示选中的列
	 sodoku->col_index[sodoku->long_row].col_mask = (1 << next_col);//更新列掩码

     success = true;
     return success; // 返回true指示成功
 }

 //保存数独到数组
void SaveSodokutoBoard(Sodoku* sodoku,vector<int>& Board)
{
    if (sodoku == nullptr) {
        return;
    }
//存储数独的每个数字

    for (int long_row = 0; long_row < sodoku->long_row; long_row++) {
        
        int digit = long_row / 9 + 1;//第几个数字
		int row = long_row % 9;//数字digit的行
		int col = sodoku->col_index[long_row].curr_col;//当前行的实际列
        
        Board[row * 9 + col] = digit;
    }
}
void PrintBoard(const vector<int>& Board)
{
    cout << "   数 独 棋 盘" << endl;

    for (int row = 0; row < 9; row++) {
        if (row % 3 == 0) {
            cout << "+-----+-----+-----+" << endl;//每三行打印一次
        }
        cout << "|";//每行开始打印
        for (int col = 0; col < 9; col++) {
            if (Board[row * 9 + col] != 0)
            {
				cout << Board[row * 9 + col];//打印数字
           }
            else
            {
				cout << "_";//打印空格
            }
            if (col % 3 == 2) {
                cout << "|";//每三列打印一次
            }
            else {
                cout << " ";//每列之间打印一个空格
            }
        }
        cout << endl;
    }
    cout << "+-----+-----+-----+" << endl;
}
//开始数独生成
void StartSodokuGenerate(Sodoku* sodoku, vector<int>& Board)
{
    // 初始化数独
    InitSodoku(sodoku);
    // 跳到下一行
    moveNextRow(sodoku);

    while (isSodokuInProgress(sodoku))//判断数独是否完成
    {
        if (moveNextCol(sodoku))
        {
            moveNextRow(sodoku);//跳到下一行
        }
        else
        {
            rollback_row(sodoku);//回滚到上一行
        }
    }
	SaveSodokutoBoard(sodoku, Board);//保存数独到数组
}
void shuffleArray(vector<int>&array) {
    for (int i = 80; i > 0; --i) {
        int j = rand()%i;
        swap(array[i], array[j]);
    }
}
int generateGameBoard(const vector<int>& normalBoard, vector<int>& gameBoard) {
	
    // 随机挖去的数字数量
    int numToRemove = 35 + rand() % 13;

    // 复制 normalBoard 到 gameBoard
    gameBoard = normalBoard;

    // 创建索引数组
    vector<int> indices(81,0);
    for (int i = 0; i <81; ++i) {
        indices[i] = i;
    }
   
    // 打乱索引数组
    shuffleArray(indices);

    // 挖去数字
    for (int i = 0; i < numToRemove; ++i) {
        gameBoard[indices[i]] = 0; // 0 表示空白
    }
    return numToRemove;
}