#include "MyArray.h"
#include <stdexcept>
#include <sstream>

// 构造函数，指定数组大小
MyArray::MyArray(int size) : size(size), p(new int[size]) {
    for (int i = 0; i < size; i++) {
        p[i] = 0;
    }
}

// 拷贝构造函数，深拷贝
MyArray::MyArray(const MyArray &old) : size(old.size), p(new int[old.size]) {
    for (int i = 0; i < size; i++) {
        p[i] = old.p[i];
    }
}

// 移动构造函数
MyArray::MyArray(MyArray &&old) noexcept : size(old.size), p(old.p) {
    *(const_cast<int**>(&old.p)) = nullptr;  // 将旧对象的指针设为nullptr
    old.size = 0;
}

// 拷贝赋值运算符，深拷贝
MyArray& MyArray::operator=(const MyArray &rhs) {
    if (this != &rhs) {
        delete[] p;  // 释放已有的内存
        *(const_cast<int**>(&p)) = new int[rhs.size];  // 分配新内存
        size = rhs.size;
        for (int i = 0; i < size; i++) {
            p[i] = rhs.p[i];
        }
    }
    return *this;
}

// 移动赋值运算符
MyArray& MyArray::operator=(MyArray &&rhs) noexcept {
    if (this != &rhs) {
        delete[] p;  // 释放已有的内存
        *(const_cast<int**>(&p)) = rhs.p;  // 转移资源
        size = rhs.size;
        *(const_cast<int**>(&rhs.p)) = nullptr;  // 旧对象指针设为nullptr
        rhs.size = 0;
    }
    return *this;
}

// 析构函数，释放动态分配的内存
MyArray::~MyArray() {
    delete[] p;
}

// 返回数组长度
int MyArray::length() const {
    return size;
}

// 返回指定下标的元素
int& MyArray::get(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("索引越界");
    }
    return p[index];
}

// 判断数组是否为空
bool MyArray::isEmpty() const {
    return size == 0 || p == nullptr;
}

// 判断两个MyArray对象是否相等
bool MyArray::equals(const MyArray &other) const {
    if (this == &other) return true;
    if (isEmpty() || other.isEmpty()) return false;
    if (size != other.size) return false;
    for (int i = 0; i < size; i++) {
        if (p[i] != other.p[i]) return false;
    }
    return true;
}

// 将数组转换为字符串
std::string MyArray::toString() const {
    if (isEmpty()) return "";
    std::ostringstream oss;
    for (int i = 0; i < size; i++) {
        if (i > 0) oss << " ";
        oss << p[i];
    }
    return oss.str();
}

// 返回指针，仅用于测试
const int* const MyArray::getP() const {
    return p;
}
