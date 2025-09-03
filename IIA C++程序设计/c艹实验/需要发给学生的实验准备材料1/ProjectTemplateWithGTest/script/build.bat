rem 首先配置cmake，然后make编译程序，生成的exe文件放在bin目录下
@echo off
pushd .

if not exist build (
     mkdir build
)

cd build
cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
mingw32-make.exe

popd