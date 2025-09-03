#pragma once
/*
	第4章 编程题1
	一维整型数组MyArry的定义如下，请实现相应的函数成员
*/
class MyArray
{
private:
	int size;			// 数组大小
	int *const p ; // 指向动态分配的内存，保存数组的内容
	
public:
	MyArray(int size = 10);					// 构造函数，参数size指定数组大小
	MyArray(const MyArray &old);			// 拷贝构造函数，要求实现深拷贝
	// MyArray &operator=(const MyArray &rhs); // 重载=，要求实现深拷贝
	// MyArray(MyArray &&old) noexcept;		// 移动拷贝
	// MyArray &operator=(MyArray &&rhs);		// 移动=
	// ~MyArray();								// 析构函数，要求能防止反复释放资源
	// int length();							// 返回数组大小
	// int &get(int index);						// 返回下标为index的元素，不考虑越界
};