#include <gtest/gtest.h>

#include "../include/ch4_homework/MyArray.h"
#include <iostream>
using namespace std;

int myarray_test(){
    cout << "Hello World" << endl;
    MyArray a;
    return 0;
}

// 下面是基于gtest编写的测试
TEST(myarray_test_suit, test_case1)
{
    EXPECT_EQ(0, myarray_test());
}


//也可以不用gtest，编写自己的main函数来启动程序
// int main(){
//     cout << "Hello World" << endl;
//     MyArray a;
//     return 0;
// }