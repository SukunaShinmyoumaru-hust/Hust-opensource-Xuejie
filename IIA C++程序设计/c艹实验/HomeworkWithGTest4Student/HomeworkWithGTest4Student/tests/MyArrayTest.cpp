#include <gtest/gtest.h>

#include "../include/ch4_homework/MyArray.h"
#include <iostream>
using namespace std;


/*
	在实现好MyArray类基础上，编写下面的代码来测试验证MyArray的功能
*/
/*
    请构建构建大小为size的MyArray对象a，要求元素的值分别为1 2 3 4 ... size
    并返回该对象 ，用于测试
*/
 MyArray buildArrayInstace(int size){
    MyArray a(size);
    for (int i = 0; i < size; ++i) {
        a.get(i) = i + 1;  // 将数组元素设置为1, 2, 3, ..., size
    }
    return a;
 }

/*
    测试MyArray的实例化以及toString函数
*/
TEST(TEST_MYARRY, MyArray_Test_instantiate_toString){
    //given
    MyArray a = buildArrayInstace(5);
    
    //when
    //得到对象a的字符串表示
    std::string content = a.toString();

    //then
    std::string target("1 2 3 4 5");
    ASSERT_EQ(target,content);
}

/*
    测试MyArray的拷贝构造函数
*/
TEST(TEST_MYARRY, MyArray_Test_CopyConstructor){
    //given
    MyArray a = buildArrayInstace(10);

    //检测内容是否相等
    //when
    MyArray b(a);
    //then
    ASSERT_EQ(true,a.equals(b));

    //检查是否为深拷贝
    //when
    a.~MyArray();  //析构对象a
    //then
    std::string target("1 2 3 4 5 6 7 8 9 10");
    ASSERT_EQ(target,b.toString()); //这时对象b的内容应该还在，深拷贝
}


/*
    测试MyArray的copy assignment（拷贝=）
*/
TEST(TEST_MYARRY, MyArray_Test_CopyAssignment){
    //given
    MyArray a = buildArrayInstace(10);
    MyArray b = buildArrayInstace(5);


    //检测内容是否相等
    //when
    a = b;
    //then
    ASSERT_EQ(true,a.equals(b));

    //检查是否为深拷贝
    //when
    b.~MyArray();
    //then
    std::string target("1 2 3 4 5");
    ASSERT_EQ(target,a.toString()); 
}


/*
    测试MyArray的移动构造函数
*/
TEST(TEST_MYARRY, MyArray_Test_MoveConstructor){
    //given
    MyArray a = buildArrayInstace(10);
    const int * const pa = a.getP();  //保留a的内部指针

    
    //when
    MyArray b(std::move(a));

    //检测内容是否相等
    std::string target("1 2 3 4 5 6 7 8 9 10");
    //then
    ASSERT_EQ(target,b.toString());

    //检查是否为浅拷贝
    //then
    ASSERT_EQ(pa,b.getP());

    //检查a对象是不是为可以安全析构
    //then
    ASSERT_EQ(0,a.length());
    ASSERT_EQ(nullptr,a.getP());
}

/*
    测试MyArray的move assignment（移动=）
*/
TEST(TEST_MYARRY, MyArray_Test_MoveAssignment){
    //given
    MyArray a = buildArrayInstace(5);
    MyArray b = buildArrayInstace(10);
    const int * const pa = a.getP();  //保留a的内部指针

    //when
    b = std::move(a);

    //检测内容是否相等
    std::string target("1 2 3 4 5");
    //then
    ASSERT_EQ(target,b.toString());

    //检查是否为浅拷贝
    //then
    ASSERT_EQ(pa,b.getP());
    
    //检查a对象是不是为可以安全析构
    //then
    ASSERT_EQ(0,a.length());
    ASSERT_EQ(nullptr,a.getP());
}

/*
    测试MyArray的析构函数
*/
TEST(TEST_MYARRY, MyArray_Test_Deconstructor){
    //given
    MyArray a = buildArrayInstace(5);
    int rtn = 0;

    //when
    a.~MyArray();
    a.~MyArray();
    rtn = 1;    //如果析构函数实现正确，会执行到rtn = 1; 

    //then
    ASSERT_EQ(0,a.length());
    ASSERT_EQ(nullptr,a.getP());
    ASSERT_EQ(1, rtn);
}
