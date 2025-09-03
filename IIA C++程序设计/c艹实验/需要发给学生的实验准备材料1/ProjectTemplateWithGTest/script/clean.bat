rem 会清除build和bin目录里所有内容（会递归删除子目录及其内容）
rem 请确保build和bin子目录里都是自动生成的文件，以免误删除用户自己创建的文件
@echo off
pushd .

if not exist build (
    mkdir build
)

pushd .
cd build
del /f /s /q *.*
for /d %%p in (*) do rmdir "%%p" /s /q
popd


if not exist bin (
    mkdir bin
)
cd bin
del /f /s /q *.*
for /d %%p in (*) do rmdir "%%p" /s /q

popd