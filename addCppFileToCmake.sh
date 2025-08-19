#!/bin/bash

files=("main.cpp" "gpio.cpp" "timer.cpp" "lcd.cpp" "spi.cpp" "lineChart.cpp");
cmakeListPath="./cmake/stm32cubemx/CMakeLists.txt";

line=0;

echo "add cpp file to cmake"

addCppFile()
{
	if [ "$line" == "0" ];
	then
		echo '添加位置不能为0';
	fi

	for file in ${files[*]}
	do
		echo "adding $file";
		sed -i "$line i ../../Core/Src/$file" $cmakeListPath;
	done
}

while read string
do
	((line++))
	if [ "$string" == '../../Core/Src/main.cpp' ];then
		echo '已经添加过cpp文件';
		exit -1;
	fi
	if [ "$string" == '../../Core/Src/main.c' ];then
		echo "在${line}处添加";
		addCppFile;
	fi
done < $cmakeListPath
