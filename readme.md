# stm32 st7735

## 环境
* VS Code
* STM32Cube for Visual Studio Code插件
* STM32 Cube MX
* STM32 Cube CLT
* 0.96寸 4-spi 80x160 st7735s屏幕
* stm32f103c8t6
* st-link

## 使用
配置cmake我也倒不清。（CMAKE项目好绕哇，还有奇妙cache时不时就爆炸）

所以建议复制粘贴lcd.ioc和Core文件夹到能运行的项目中覆盖替换，然后再向cmake添加代码文件。

1. 复制替换ioc配置和Core下的代码
2. 向cmake添加代码文件
3. 参照cube接线，烧录运行

## 向cmake添加代码文件

出于神秘原因cube不会让cmake编译cpp文件，所以我们得给他擦屁股。而且是每次cube生产文件后都要做。（因此建议使用脚本自动添加）

### 自动添加：
复制并运行addCppFileToCmake.sh

### 手动添加：
在[CMakeLick.txt](cmake/stm32cubemx/CMakeLists.txt)里面找到

``` cmake
target_sources(stm32cubemx INTERFACE
	../../Core/Src/main.c
	......
)
```

在其中加入

``` cmake
../../Core/Src/lineChart.cpp
../../Core/Src/spi.cpp
../../Core/Src/lcd.cpp
../../Core/Src/timer.cpp
../../Core/Src/gpio.cpp
../../Core/Src/main.cpp
```
