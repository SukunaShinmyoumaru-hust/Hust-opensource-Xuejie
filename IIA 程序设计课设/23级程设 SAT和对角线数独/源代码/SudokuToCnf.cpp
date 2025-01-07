#pragma once
#include"definition.h"
#include"vector.hpp"

/***********************************************
*函数名称：sudokuToCnf
* 函数功能：：将数独棋盘转换为 CNF 格式，并将其写入文件。
* 注释：- 从源文件 "SudokuBase.cnf" 复制基础约束内容。
        - 添加数独棋盘上已有数字的约束。
        - 写入主对角线和副对角线不能重复的规则。
        - 写入 CNF 文件头。
* 返回值：bool类型，如果成功生成 CNF 文件，则返回 true，否则返回 false。
************************************************/
bool SudokuToCnf(const vector<int>& board,int empty)
{
    string sourceFilename = "SudokuBase.cnf";
    string destinationFilename = "NormalSudoku.cnf";

    // 打开源文件进行读取
    ifstream sourceFile(sourceFilename, ios::binary);
    if (!sourceFile) {
        cerr << "无法打开源文件 " << sourceFilename << endl;
        return 0;
    }

    // 打开目标文件进行写入，如果文件不存在则新建
    ofstream destinationFile(destinationFilename, std::ios::out | std::ios::trunc);
    if (!destinationFile) {
        cerr << "无法打开目标文件 " << destinationFilename << endl;
        return 0;
    }
    destinationFile << "p" << " " << "cnf" << " " << 729 << " " << 8829 + 81 - empty << endl;
    // 从源文件读取内容并写入到目标文件
   
    for (int i = 1; i <=9; ++i) {
        for (int j = 1; j <=9; ++j) {
            if (board[(i- 1) * 9 + j - 1]) {
				destinationFile << i << j << board[(i - 1) * 9 + j - 1] << " 0" << endl;
            }
        }
    }
    char buffer[1024];
    while (sourceFile.read(buffer, sizeof(buffer))) {
        destinationFile.write(buffer, sourceFile.gcount());
    }

    // 写入剩余的内容（如果有的话）
    if (sourceFile.gcount() > 0) {
        destinationFile.write(buffer, sourceFile.gcount());
    }

    // 关闭文件
    sourceFile.close();
    destinationFile.close();

    //cout << "文件内容复制完成。" << endl;

}

/***********************************************
*函数名称：XsudokuToCnf
* 函数功能：：将数独棋盘(X数独)转换为 CNF 格式，并将其写入文件。
* 注释：- 从源文件 "SudokuBase.cnf" 复制基础约束内容。
        - 添加数独棋盘上已有数字的约束。
        - 写入主对角线和副对角线不能重复的规则。
        - 写入 CNF 文件头。
* 返回值：bool类型，如果成功生成 CNF 文件，则返回 true，否则返回 false。
************************************************/
bool XSudokuToCnf(const vector<int>& board, int empty)
{
    string sourceFilename = "SudokuBase.cnf";
    string destinationFilename = "XSudoku.cnf";

    // 打开源文件进行读取
    ifstream sourceFile(sourceFilename, ios::binary);
    if (!sourceFile) {
        cerr << "无法打开源文件 " << sourceFilename << endl;
        return 0;
    }

    // 打开目标文件进行写入，如果文件不存在则新建
    ofstream destinationFile(destinationFilename, std::ios::out | std::ios::trunc);
    if (!destinationFile) {
        cerr << "无法打开目标文件 " << destinationFilename << endl;
        return 0;
    }

	// 写入文件头
    destinationFile << "p" << " " << "cnf" << " " << 729 << " " << 8829 +648+81 - empty << endl;

    // 主对角线不能重复的规则
    int mainDiagonal[9] = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };
    for (int z = 1; z <= 9; ++z) {
        for (int i = 0; i < 9; ++i) {
            for (int j = i + 1; j < 9; ++j) {
                destinationFile << 0 - (mainDiagonal[i] * 10 + z) << " " << 0 - (mainDiagonal[j] * 10 + z) << " " << 0 << endl;
            }
        }
    }

    // 副对角线不能重复的规则
    int antiDiagonal[9] = { 19, 28, 37, 46, 55, 64, 73, 82, 91 };
    for (int z = 1; z <= 9; ++z) {
        for (int i = 0; i < 9; ++i) {
            for (int j = i + 1; j < 9; ++j) {
                destinationFile << 0 - (antiDiagonal[i] * 10 + z) << " " << 0 - (antiDiagonal[j] * 10 + z) << " " << 0 << endl;
            }
        }
    }
//棋盘上已有数字产生的约束----------------------------------------
    for (int i = 1; i <= 9; ++i) {
        for (int j = 1; j <= 9; ++j) {
            if (board[(i - 1) * 9 + j - 1]) {
                destinationFile << i << j << board[(i - 1) * 9 + j - 1] << " 0" << endl;
            }
        }
    }
//棋盘上已有数字产生的约束---------------------------------------
//复制基础约束内容-----------------------------------------------
    char buffer[1024];
    while (sourceFile.read(buffer, sizeof(buffer))) {
        destinationFile.write(buffer, sourceFile.gcount());
    }
    // 写入剩余的内容（如果有的话）
    if (sourceFile.gcount() > 0) {
        destinationFile.write(buffer, sourceFile.gcount());
    }
//复制基础约束内容-----------------------------------------------

    // 关闭文件
    sourceFile.close();
    destinationFile.close();
}
