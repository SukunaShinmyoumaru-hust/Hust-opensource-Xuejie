cmake_minimum_required(VERSION 3.0)

PROJECT("HomeworkWithGTest4Student")

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -Wall -Werror -Wnon-virtual-dtor -Woverloaded-virtual")

#可执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#构建的库的输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

#构建test 目标
ENABLE_TESTING()
#向当前项目添加一个子目录，并且使得 CMake 在构建过程中进入该子目录继续处理其中的 CMakeLists.txt 文件。
#这个命令通常用于管理项目中的多个模块或子项目。
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
