#这是位于src下面的CMakeLists.txt，用于将src下面的所有代码构建成静态库

#${CMAKE_SOURCE_DIR}是项目的根目录
message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
#将项目根目录下的include和src子目录都加到变量INCLUDE里
SET(INCLUDE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
message("INCLUDE: " ${INCLUDE})

#查找src下面所有cpp文件（遇到src的子目录也会递归查找），查找结果放在变量SOURCE里
FILE(GLOB_RECURSE SOURCE "*.cpp")
message("SOURCE: " ${SOURCE})

#CMAKE_CURRENT_SOURCE_DIR是当前正在处理的 CMakeLists.txt 所在的源代码目录路径，即:项目根目录\src
message("CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
#变量SOURCE里面已经是src下面所有cpp文件，现在将 项目根目录\src\Main.cpp和main.cpp从SOURCE里面删除
#因为src的内容最终不是构建成exe文件，而是库，所以要删除main函数
LIST(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)
LIST(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)


#显示所有警告。
ADD_COMPILE_OPTIONS("-Wall")
#将所有的编译器警告视为错误
ADD_COMPILE_OPTIONS("-Werror")
#将${SOURCE}里面包含的所有cpp文件编译构建成一个库（lib），类型是static库
#库的文件名为：lib${PROJECT_NAME}.a，其中${PROJECT_NAME}是项目根目录下的CMakeLists.txt里面定义的：PROJECT("项目名称")
ADD_LIBRARY(${PROJECT_NAME} ${SOURCE})


#Specify include directories to use when compiling a given target.（为构建目标添加头文件搜索目录）
#The named <target> must have been created by a command such as add_executable() or add_library() and must not be an ALIAS target.
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC "${INCLUDE}")
