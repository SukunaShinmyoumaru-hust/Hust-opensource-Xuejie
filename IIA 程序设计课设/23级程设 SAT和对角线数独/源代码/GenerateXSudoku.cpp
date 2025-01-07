#include"GenerateXSudoku.h"
/***********************************************
*函数名称：isSafe
* 函数功能：检查在给定的行、列和数字的情况下，数字是否可以安全地放置在数独棋盘上。
* 注释：用于生成数独时，检查数字是否可以放置在数独棋盘上。
* 返回值：如果数字可以安全地放置在数独棋盘上，则返回true，否则返回false。
************************************************/
bool isSafe(const vector<int>& board, int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        // 检查行
        if (board[row * SIZE + x] == num)
            return false;
        // 检查列
        if (board[x * SIZE + col] == num)
            return false;
    }

    // 检查九宫格
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[(startRow + i) * SIZE + (startCol + j)] == num)
                return false;
        }
    }

    // 检查主对角线
    if (row == col) {
        for (int i = 0; i < SIZE; i++) {
            if (board[i * SIZE + i] == num)
                return false;
        }
    }

    // 检查副对角线
    if (row + col == SIZE - 1) {
        for (int i = 0; i < SIZE; i++) {
            if (board[i * SIZE + (SIZE - 1 - i)] == num)
                return false;
        }
    }

    return true;
}

/***********************************************
*函数名称：solveSudoku
* 函数功能：随机生成数独棋盘
* 注释：利用递归回溯的思想，生成数独棋盘。
* 返回值：bool类型，如果成功生成数独棋盘，则返回true，否则返回false。
************************************************/
bool solveSudoku(vector<int>& board) 
{
    for (int row = 1; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row * SIZE + col] == 0) { //如果当前位置为空
                for (int num = 1; num <= SIZE; num++) {
                    if (isSafe(board, row, col, num)) {
                        board[row * SIZE + col] = num; // 放置数字

                        if (solveSudoku(board))
                            return true; // 递归

                        // Backtrack
                        board[row * SIZE + col] = 0; //回溯
                    }
                }
                return false; // 递归失败
            }
        }
    }
	return true; // 生成成功
}
/***********************************************
*函数名称：GenerateFirstLine
* 函数功能：随机产生数独棋盘的第一行
* 注释：solveSudoku函数的辅助函数，用于生成数独棋盘的第一行，这决定了后续数独棋盘的生成。
* 返回值：void
************************************************/
void GenerateFirstLine(vector<int>& a) {

    for (int i = 0; i < SIZE; ++i) {
        a[i] = rand() % SIZE + 1;
        int j = 0;
        while (j < i) {
			if (a[i] == a[j]) {//如果生成的数字重复
                a[i] = rand() % SIZE + 1;//重新生成
				j = 0;//重新检查
            }
            else {
                j++;
            }
        }
    }
}

/***********************************************
*函数名称：generateDiagonalSudoku
* 函数功能：随机生成对角线数独棋盘
* 注释：传入board数组，生成对角线数独棋盘。
* 返回值：void
************************************************/
void generateDiagonalSudoku(vector<int>& board) {
    for (int i = 0; i < 81; i++)
    {
        board[i] = 0;
    }
    GenerateFirstLine(board);
    solveSudoku(board);
   
}
