#这个CMakeLists.txt位于test目录下
#用于构建tests目录下的测试代码，最终构建成exe文件

#构建系统时禁用GMock
#设置缓存变量BUILD_GMOCK为OFF，变量类型为BOOL
#FORCE是指如果缓存里已经有该变量BUILD_GMOCK，则强制用新的值覆盖
#""是docstring，用于在cmake-gui里显示该命令的介绍
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

##添加googletest子目录，并且使得 CMake 在构建过程中进入该子目录继续处理其中的 CMakeLists.txt 文件。
ADD_SUBDIRECTORY(googletest EXCLUDE_FROM_ALL)


#搜索当前目录下所有源文件，并将它们存储在 SOURCE 变量中
AUX_SOURCE_DIRECTORY(. SOURCE)
#然后，这个 SOURCE 变量被用作 add_executable 命令的参数，以构建名为 ${PROJECT_NAME}-main 的可执行文件。
ADD_EXECUTABLE(${PROJECT_NAME}-main ${SOURCE})

#构建名为 ${PROJECT_NAME}-main 的可执行文件时，需要依赖的库
#   一个是名为${PROJECT_NAME}的库（在src\CMakeLists.txt里面构建）
#   一个是名为gtest_main的库，这个库是在tests\googletest\googletest\CMakeLists.txt里构建的（120行）
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-main ${PROJECT_NAME} gtest_main)

ADD_TEST(NAME ${PROJECT_NAME}-main COMMAND ${PROJECT_NAME}-main)
