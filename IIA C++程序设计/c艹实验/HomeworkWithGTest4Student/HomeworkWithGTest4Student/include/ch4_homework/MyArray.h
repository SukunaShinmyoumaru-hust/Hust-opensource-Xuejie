#pragma once
#include <string>

class MyArray {
private:
    int size;                 // 数组大小
    int* const p;             // 指向数组的指针，声明为const，防止指针被重新赋值

public:
    // 构造函数
    MyArray(int size = 10);                     // 默认构造函数
    MyArray(const MyArray &old);                // 拷贝构造函数
    MyArray(MyArray &&old) noexcept;            // 移动构造函数

    // 重载运算符
    MyArray& operator=(const MyArray &rhs);     // 拷贝赋值运算符
    MyArray& operator=(MyArray &&rhs) noexcept; // 移动赋值运算符

    // 析构函数
    ~MyArray();                                 // 析构函数

    // 成员函数
    int length() const;                         // 返回数组长度
    int& get(int index);                        // 获取指定索引的元素
    bool isEmpty() const;                       // 判断是否为空
    bool equals(const MyArray &other) const;    // 判断两个MyArray对象是否相等
    std::string toString() const;               // 将数组内容转换为字符串
    const int* const getP() const;              // 返回指针，仅用于测试
};
