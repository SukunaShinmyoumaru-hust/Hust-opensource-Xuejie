# cmake step by step 入门



环境准备：

+ cmake环境已安装就位
+ git安装就位
+ mingw64安装就位
+ git安装目录下Git\usr\bin路径加入到用户Path变量中，如git安装在C:\Program Files\Git，则把C:\Program Files\Git\usr\bin添加到用户Path变量中



## step1

1、新建一个t1目录,用于练习

```shell
mkdir t1
cd t1
```

2、在t1 目录中，新建源文件**main.cpp**，内容如下

```cpp
#include <iostream>
using namespace std;

int main()
{
    cout << "Hello World from t1 Main!" << endl;
    return 0;
}
```

3、在t1 目录中，新建**CMakeLists.txt**文件，内容如下

```tex
PROJECT (HELLO)
SET(SRC_LIST main.cpp)
ADD_EXECUTABLE(hello ${SRC_LIST})
```

4、生成makefile文件，cmake利用CmakeLists.txt，t1目录中有Makefile。t1目录下执行：

```shell
cmake.exe -G"MinGW Makefiles"
```

5、生成hello.exe，mingw32-make.exe利用makefile，完成编译和链接，t1目录中有hello.exe

```shell
mingw32-make.exe
```

6、执行hello.exe，输出main.cpp的字符串

```shell
hello.exe
```

Hello World from t1 Main!



## step2

1、在t1目录删除除main.cpp和CMakeLists.txt外的所有内容，然后建立build 目录

```shell
mkdir build
cd build
```

2、在t1的build目录下，执行cmake.exe -G"MinGW Makefiles" .. 
   ，可在build目录下看到makefile文件

```shell
cmake.exe -G"MinGW Makefiles" .. 
```

3、在t1的build目录下，执行mingw32-make.exe，可在在build目录下看到hello.exe文件

```shell
mingw32-make.exe
```



## step3

目标：
1、在t1目录下增加一个子目录src，用来放置工程源代码
2、将构建后的目标文件放入构建目录的bin子目录

准备工作：
1、在t1目录下添加子目录src：将main.cpp 移动到src

```shell
mkdir src
mv main.cpp src/
```

2、进入子目录src：

```shell
cd src
```

编写CMakeLists.txt如下:

```tex
ADD_EXECUTABLE(hello main.cpp)
```

3、将t1目录CMakeLists.txt修改为：

```txt
PROJECT(HELLO)
ADD_SUBDIRECTORY(src bin)
```

4、进入到build目录，删除目录下所有内容，然后执行如下命令进行外部编译

```shell
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe 
```

5、确认目标文件hello.exe位于build/bin目录
t1\build\bin>hello
Hello World from t1 Main!



## step4

目标：
增加一个func.cpp源文件编译

具体执行步骤：
1、在t1目录下创建include目录，并且创建func.hpp文件

```shell
mkdir include
touch include/func.hpp
```



2、在src目录下增加func.cpp

```shell
touch src/func.cpp
```



3、编辑src\CMakeLists.txt,修改为如下

```tex
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
ADD_EXECUTABLE(hello main.cpp;func.cpp)
```



4、编辑func.hpp，敲入如下代码

```cpp
#include <iostream>
using namespace std;

void func();
```



5、编辑func.cpp，敲入如下代码

```cpp
#include "func.hpp"

void func()
{
    cout << "You are enter func!" << endl;
}

```

6、编辑main.cpp ,增加如下代码

```cpp
#include <iostream>
#include "func.hpp"
using namespace std;

int main()
{
    cout << "Hello World from t1 Main!" << endl;
    func();
    return 0;
}

```



7、进入到build目录，删除目录下所有内容，然后执行如下命令进行外部编译

```shell
cd build
rm -rf ./*
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



8、确认目标文件hello.exe,位于build/bin
t1\build\bin>hello
Hello World from t1 Main!
You are enter func!



## step5

目标：
不用每增加一个文件就改一改配置文件

具体执行步骤：

1、编辑src\CMakeLists.txt,修改为如下

```text
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
FILE(GLOB SOURCE "*.cpp")
ADD_EXECUTABLE(hello main.cpp;${SOURCE})
```



2、在include目录下新增new_func.hpp并编辑，敲入如下代码

```cpp
#include <iostream>
using namespace std;

void NewFunc();

```



3、在src目录下新增new_func.cpp并编辑，敲入如下代码

```cpp
#include "new_func.hpp"

void NewFunc()
{
    cout << "You are enter new func!" << endl;
}

```



4、编辑main.cpp ,增加函数调用，修改为如下代码

```cpp
#include <iostream>
#include "func.hpp"
#include "new_func.hpp"
using namespace std;

int main()
{
    cout << "Hello World from t1 Main!" << endl;
    func();
    NewFunc();
    return 0;
}

```



5、进入到build目录，删除目录下所有内容，然后执行如下命令进行外部编译

```shell
cd build
rm -rf ./*
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



6、确认目标文件hello.exe,位于build/bin
t1\build\bin>hello
Hello World from t1 Main!
You are enter func!
You are enter new func!



## step6

目标：
静态库构建

具体执行步骤：
步骤1：t1目录下创建lib目录，并进入到lib目录

```shell
mkdir lib
cd lib
```



步骤2：创建funclib.hpp文件，敲入如下代码

```cpp
#include <iostream>
using namespace std;

void FuncLib();

```



创建funclib.cpp文件，并敲入如下内容：

```cpp
#include "funclib.hpp"

void FuncLib()
{}

```



步骤3：在t1\lib目录，新建CMakeLists.txt

```tex
SET(LIBHELLO_SRC funclib.cpp)
ADD_LIBRARY(funclib STATIC ${LIBHELLO_SRC})
```

用于cmake产生makefile文件

步骤5：创建lib\build目录，进入lib\build目录，然后执行cmake

```shell
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



--- 在lib\build目录看到libfundlib.a 静态库



## step7

使用外部共享静态库和头文件
步骤1：在t1/lib目录下创建include目录，且将lib目录下funclib.hpp文件移动到lib\include目录下

步骤2：修改lib\CMakeLists.txt

```tex
SET(LIBHELLO_SRC funclib.cpp)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/include)
ADD_LIBRARY(funclib STATIC ${LIBHELLO_SRC})
```



步骤3：修改src\CMakeLists.txt

```tex
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/include)

FILE(GLOB SOURCE "*.cpp")

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/build)
ADD_EXECUTABLE(hello main.cpp;${SOURCE})
TARGET_LINK_LIBRARIES(hello funclib)

```



步骤4：修改maic.cpp

```cpp
#include <iostream>
#include "func.hpp"
#include "new_func.hpp"
#include "funclib.hpp"
using namespace std;

int main()
{
    cout << "Hello World from t1 Main!" << endl;
    func();
    NewFunc();
    FuncLib();
    return 0;
}

```



步骤5：进入build目录，删除build目录下所有文件，在build目录执行cmake

```shell
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



步骤6：执行程序
t1\build\bin>hello.exe



## step8

加入测试

步骤1：添加子目录tests：新建文件main.cpp，添加代码如下

```cpp
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

```



步骤2：在tests 目录中，新建源文件test.cpp，内容如下

```cpp
#include <gtest/gtest.h> 

#include "sut.hpp"

TEST(Hello, should_get_world)
{
    int actual = SutAdd(1, 1);
    EXPECT_EQ(actual, 2);
}

```



步骤3：在include 目录中，新建头文件sut.hpp，代码如下

```cpp
#ifndef SUT_H
#define SUT_H

int SutAdd(int a, int b);

#endif /* SUT_H */

```




步骤4：在src 目录中，添加sut.cpp ，内容如下

```cpp
#include "sut.hpp"

int SutAdd(int a, int b){
    return a+b;
}

```



步骤5：修改t1\CMakeLists.txt ，配置测试如下：

```tex
PROJECT (HELLO)
SET(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
ENABLE_TESTING()

ADD_SUBDIRECTORY(src bin)
ADD_SUBDIRECTORY(lib lib)
ADD_SUBDIRECTORY(tests)

```



步骤6：修改t1\tests\CMakeLists.txt ，配置测试如下：

```tex
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(googletest EXCLUDE_FROM_ALL)
AUX_SOURCE_DIRECTORY(. SOURCE)
ADD_EXECUTABLE(helloTest ${SOURCE})
TARGET_LINK_LIBRARIES(helloTest hello gtest_main)

ADD_TEST(NAME helloTest COMMAND helloTest)

```




步骤7：把googletest拷贝到tests文件夹下

步骤8：把src\CMakeList.txt 中把main之外的输出为静态库

```tex
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/include)

SET(INCLUDE ${CMAKE_SOURCE_DIR}/include)

FILE(GLOB SOURCE "*.cpp")
LIST(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

ADD_LIBRARY(hello ${SOURCE})
TARGET_INCLUDE_DIRECTORIES(hello PUBLIC "${INCLUDE}")

ADD_EXECUTABLE(helloExe ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
TARGET_LINK_LIBRARIES(helloExe hello funclib)

```



步骤9：进入build目录，删除build下所有文件，在build目录执行cmake

```shell
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



## step10

目标：
动态库构建

具体执行步骤：
步骤1、新建一个t2目录,用于练习
```shell
mkdir t2
cd t2
```



步骤2：在t2目录下新建lib目录

```shell
mkdir lib
```



步骤3：t2\lib目录下新建funclib.hpp敲入如下代码

```cpp
#pragma once

#if WIN32
#ifdef funclib_EXPORTS 
#define API declspec(dllexport)
#else
#define API declspec(dllimport)
#endif
#endif

extern "C" void FuncLib();

```

t2\lib目录下新建funclib.cpp敲入如下代码

```cpp
#include "funclib.hpp"

void FuncLib()
{
    cout << "You are enter func dynamic lib!" << endl;
}

```



步骤4：添加t2\CMakeLists.txt,新增加lib目录的遍历

```tex
PROJECT(HELLO)
ADD_SUBDIRECTORY(lib lib)
```



步骤5：在t2\lib目录，新建CMakeLists.txt，用于cmake产生makefile文件

```tex
SET(LIBHELLO_SRC funclib.cpp)
ADD_LIBRARY(funclib SHARED ${LIBHELLO_SRC})
```



步骤6：在t2目录下创建build目录，并且在build目录执行cmake

```shell
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



--- 在build\lib目录看到libfundlib.dll动态库



## step11

目标：
动态库应用

具体执行步骤：
步骤1：在t2目录下新建src目录

```shell
mkdir src
```



步骤2：t2\src目录下新建main.cpp，敲入如下代码

```cpp
#include "funclib.hpp"
#include <iostream>
using namespace std;

int main()
{
    cout << "main start" << endl;

    FuncLib();
    cout << "main end" << endl;
    return 0;

}

```



步骤3：修改t2\CMakeLists.txt,新增加lib目录的遍历

```tex
PROJECT(HELLO)
ADD_SUBDIRECTORY(lib lib)
ADD_SUBDIRECTORY(src bin)
```



步骤4：在t2\src目录，新建CMakeLists.txt，用于cmake产生makefile文件

```tex
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

FILE(GLOB SOURCE "*.cpp")

LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/lib)
ADD_EXECUTABLE(hello main.cpp)
TARGET_LINK_LIBRARIES(hello funclib)

```



步骤5：进入build目录，删除build目录下所有文件，在build目录执行cmake

```shell
cmake.exe -G"MinGW Makefiles" ..
mingw32-make.exe
```



步骤6：将build\lib目录下的libfunclib.dll拷贝到build\bin目录下

步骤7：在build\bin目录下执行可执行程序
t2\build>bin\hello.exe
main start
You are enter func dynamic lib!
main end