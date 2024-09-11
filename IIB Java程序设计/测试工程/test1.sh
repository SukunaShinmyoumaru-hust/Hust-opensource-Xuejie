#!/bin/bash
#设置JAVA_HOME
JAVA_HOME=/Libary/Java/JavaVirtualMachines/jdk-13.0.2.jdk/Contents/Home

#设置运行TestNG的依赖Ja包所在目录(当前目录的lib子目录)
LIB_DIR=./lib

#设置学生代码的目录（顶级包目录的父目录）
TO_BE_TEST_CLASSPAH=/Uses/xupengzhu/Desktop/Java实验作业/Experiment1Tes4_4Linux/betset

#设置测试类的目录,为当前目录的test子目录
TEST_SUITE_CLASSPATH=./test

#将运行TestNG的依赖Ja包放进CLASSPATH
DEP_JAR=
fo jarfile in $(find $LIB_DIR | grep '\.jar$')
do
	if [ ! -n "$DEP_JAR" ];then
		DEP_JAR=$jafile
	else
		DEP_JAR=$DEP_JAR:$jafile
	fi
done
CLASSPATH=$CLASSPATH:$TO_BE_TEST_CLASSPAH:$TEST_SUITE_CLASSPATH:$DEP_JAR
expot JAVA_HOME
expot CLASSPATH

java  og.testng.TestNG testng.xml -d test-output
