#include"DPLL-Solver.h"
#include"Literal.h"
#include"Clause.h"
#include"CNF.h"
#include"CNFParser.h"
#include"vector.hpp"
#include"definition.h"
#include"GenerateXSudoku.h"
#include"CheckSudokuBoard.hpp"
bool SudokuToCnf(const vector<int>& board, int empty);//将数独转化为CNF
bool XSudokuToCnf(const vector<int>& board, int empty);//将对角线数独转化为CNF

int Count = 0;//递归次数
int numVars = 0, numClauses = 0;//变元数和子句数

int main() {
	int choice = 1, choice2 = 1, choice3 = 1, choice4 = 1, way = 1;//选择
	int isRead = 0;//是否读取CNF文件
	std::string filename;//文件名
	CNF Cnf;	//CNF
	clock_t start, end;//计时
	double time1 = 0, time2 = 0;//时间比较
	int* assignments = nullptr;//布尔变元的赋值状态

    Sodoku* sodoku = new Sodoku();
    vector<int> NormalBoard(81, 0);//数独完整棋盘
    vector<int> GameBoard(81, 0);//挖洞生成的数独游戏棋盘
	vector<int> GamePlayBoard(81, 0);//玩家填写的数独游戏棋盘
	int empty = 0;//挖洞数目
	int x = 0, y = 0, num = 0;//玩家输入的坐标和数字

	srand(static_cast<unsigned int>(time(nullptr)));//随机数种子

    while (true) {
        cout << endl << endl;
		cout << "\t  菜  单" << endl;
        cout << "**********************************" << endl;
        cout << endl;
		cout << "1.SAT求解器\t  2.数独游戏"<< endl;
        cout << endl;
        cout << "0. Exit" << endl;
        cout << "**********************************" << endl;
        cout << "请输入您的选择：";
        cin >> choice;
		system("cls");
        switch (choice) {
        case 1:
            choice2 = 1;
            while (choice2)
            {
                way = 1;
                cout << endl;
                cout << "*****************SAT求解********************" << endl << endl;
				cout << "\t1.读取CNF文件     2.打印CNF文件" << endl<<endl;
				cout << "\t3.求解CNF文件     0.返回" << endl;
                cout << "********************************************" << endl<<endl;
                cout << "请输入您的选择：";
				cin >> choice2;
                switch (choice2)
                {
                case 1:
                    cout << "请输入CNF文件路径：";
                    cin >> filename;
                    if (CNFParser::parse(Cnf, filename, numVars, numClauses))
                    {
                        isRead = 1;
                        if (assignments != nullptr) delete[] assignments;
						assignments = new int[numVars + 1];
			            
                        cout << "读取成功" << endl;
                    }
                    //按任意键继续
                    system("pause");
                    system("cls");
                    break;
                case 2:
					if (Cnf.isEmpty()) cout << "CNF为空" << endl;
                    else Cnf.print();
					//按任意键继续
					system("pause");
                    system("cls");
                    break;
                case 3:
                    while (isRead&&way) {
                        Count = 0;
                        cout << endl;
						cout << "----------变元选择策略------------" << endl;
                        cout << "---1.随机选择变元" << endl;
                        cout << "---2.选择第一个变元" << endl;
                        cout << "---3.选择出现次数最多的文字" << endl;
                        cout << "---4.选择权重最大的文字" << endl;
						cout << "---5.选择权重最大的文字（区分正负）" << endl;
                        cout << "---0.返回" << endl;
                        cout << endl;
                        cout << "请输入变元选择策略:";
                        cin >> way;
                        if (way == 0) { system("cls"); break; }
                        //计算执行时间

                        start = clock();
                        bool result = DPLL(Cnf, way, assignments);
                        if (result)
                        {
                            cout << "可满足" << endl;
							printAssignments(assignments,1);
                        }
                        else cout << "不可满足" << endl;
                        end = clock();
                        
						if (way == 2) time1 = (double)(end - start);
                        time2 = (double)(end - start);
                        saveResultToFile(filename, result, assignments, time2);
                        cout << "执行时间：" << time2 << "ms" << endl;
						if (time1 == 0 || time1 == time2 || time2 == 0) cout << "时间优化率：0%" << endl;
						else cout << "时间优化率：" << (time1 - time2) / time1 * 100 << "%" << endl;

                        system("pause");
                        system("cls");
                    }
                    if (isRead == 0)
                    {
						cout << "请先读取CNF文件" << endl;
                        system("pause");
                        system("cls");
                    }
                    break;
				case 0:
					system("cls");
                   
					break;
                default:
                    break;
                }

            }		         
            break;
        case 2:
            choice3 = 1;
            while (choice3)
            {
               
                cout << endl;
                cout << "*****************数独游戏*******************" << endl << endl;
                cout << "\t1.普通数独     2.对角线数独" << endl << endl;;
                cout << "\t0.返回" << endl;
                cout << "********************************************" << endl << endl;;
                cout << "请输入您的选择：";
                cin >> choice3;
                switch (choice3)
                {
                case 1:
					system("cls");
                    StartSodokuGenerate(sodoku, NormalBoard);                 
					empty=generateGameBoard(NormalBoard, GameBoard);
					GamePlayBoard = GameBoard;					
                    choice4 = 1;
                    while (choice4)
                    {
						system("cls");
						cout << endl;
						cout << "-----------------数独游戏-----------------" << endl << endl;
						cout << "\t1.填写数字     2.查看答案" << endl << endl;
                        cout << "\t3.重新开始     4.清屏" << endl << endl;
                        cout << "\t5.DPLL求解     0.返回" << endl;
						cout << "-------------------------------------------" << endl << endl;
                        PrintBoard(GamePlayBoard);
						cout << "请输入您的选择：";
						cin >> choice4;
						switch (choice4)
						{
						case 1:
							cout << "请输入坐标（x,y）和数字：";
							cin >> x >> y >> num;
							if (x < 1 || x>9 || y < 1 || y>9 || num < 0 || num>9)
							{
								cout << "输入错误，请重新输入" << endl;
								system("pause");
								break;
							}
							
							if (GameBoard[(x - 1) * 9 + y - 1] != 0)
							{
								cout << "该位置不可填写，请重新输入" << endl;
								system("pause");
								break;
							}
							//检测当前局面是否合法
							if (CheckBoard(GamePlayBoard, x, y, num) == false)
							{
								cout << "填写错误，请重新输入" << endl;
								system("pause");
								break;
							}
							GamePlayBoard[(x - 1) * 9 + y - 1] = num;
                            cout << endl;
							PrintBoard(GamePlayBoard);
							break;
						case 2:
                            cout << endl;
							PrintBoard(NormalBoard);
							system("pause");
							break;
						case 3:
							GamePlayBoard = GameBoard;
							system("cls");
							PrintBoard(GamePlayBoard);
							break;
						case 4:
							system("cls");
							break;
                        case 5:
							SudokuToCnf(GamePlayBoard,empty);
							if (CNFParser::parse(Cnf, "NormalSudoku.cnf", numVars, numClauses))
							{
								isRead = 1;
                                numVars = 999;
								if (assignments != nullptr) delete[] assignments;
								assignments = new int[numVars + 1];
								cout << "读取成功" << endl;
							}
							if (DPLL(Cnf, 2, assignments))
							{
								cout << "数独可解" << endl;
								printAssignments(assignments,0);
							}
							else cout << "数独不可解" << endl;
							system("pause");
							break;
						case 0:
							system("cls");
							break;
						default:
							break;
						}
                    }
                    system("pause");
                    break;
                case 2:
                    system("cls");
                    generateDiagonalSudoku(NormalBoard);
					empty = generateGameBoard(NormalBoard, GameBoard);
					GamePlayBoard = GameBoard;
                    
					choice4 = 1;
					while (choice4)
					{
						system("cls");
						cout << endl;
						cout << "-----------------数独游戏-----------------" << endl << endl;
						cout << "\t1.填写数字     2.查看答案" << endl << endl;
						cout << "\t3.重新开始     4.清屏" << endl << endl;
						cout << "\t5.DPLL求解     0.返回" << endl;
						cout << "-------------------------------------------" << endl << endl;
						PrintBoard(GamePlayBoard);
						cout << "请输入您的选择：";
						cin >> choice4;
						switch (choice4)
						{
						case 1:
							cout << "请输入坐标（x,y）和数字：";
							cin >> x >> y >> num;
							if (x < 1 || x>9 || y < 1 || y>9 || num < 0 || num>9)
							{
								cout << "输入错误，请重新输入" << endl;
								system("pause");
								break;
							}
							
							if (GameBoard[(x - 1) * 9 + y - 1] != 0)
							{
								cout << "该位置不可填写，请重新输入" << endl;
								system("pause");
								break;
							}
							//检测当前局面是否合法
							if (CheckBoard(GamePlayBoard, x, y, num) == false)
							{
								cout << "填写错误，请重新输入" << endl;
								system("pause");
								break;
							}

							GamePlayBoard[(x - 1) * 9 + y - 1] = num;
							cout << endl;
							PrintBoard(GamePlayBoard);
							break;
						case 2:
							cout << endl;
							PrintBoard(NormalBoard);
							system("pause");
							break;
						case 3:
							GamePlayBoard = GameBoard;
							system("cls");
							PrintBoard(GamePlayBoard);
							break;
						case 4:
							system("cls");
							break;
						case 5:
							XSudokuToCnf(GamePlayBoard, empty);
							if (CNFParser::parse(Cnf, "XSudoku.cnf", numVars, numClauses))
							{
								isRead = 1;
								numVars = 999;
								if (assignments != nullptr) delete[] assignments;
								assignments = new int[numVars + 1];
								cout << "读取成功" << endl;
							}
							if (DPLL(Cnf, 2, assignments))
							{
								cout << "数独可解" << endl;
								printAssignments(assignments,0);
							}
							else cout << "数独不可解" << endl;
							system("pause");
							break;
						case 0:
							system("cls");
							break;
						default:
							break;
						}
					}
					system("pause");
					break;
                case 0:
                    system("cls");
                    break;
                default:
                    break;

                }
       

            }
            break;
        case 0:
			cout << "退出程序" << endl;
            return 0;
        default:
            cout << "无效的选择，请重新尝试\n";
        }
    }

    return 0;
}