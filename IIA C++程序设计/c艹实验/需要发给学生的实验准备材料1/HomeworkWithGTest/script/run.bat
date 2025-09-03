rem 自动执行bin目录下的exe文件（如果有多个exe文件，会自动执行最后找到的exe文件）
@echo off
pushd .

if not exist bin (
    echo "bin directory not exists!"
    popd
    exit(1)
)

set EXE_FILE=""
for /r ./bin %%f in (*.exe) do (
    set EXE_FILE=%%f
)
echo %EXE_FILE%

if EXE_FILE!=="" (
    echo "exe file not exists in bin directory!"
    popd
    exit(1)
)

%EXE_FILE%

popd