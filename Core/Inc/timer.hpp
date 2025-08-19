#pragma once
#include "main.h"

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);

class Timer
{
public:
	enum class TimerInstance
	{
		Timer1 = TIM1_BASE,
		Timer2 = TIM2_BASE,
		Timer3 = TIM3_BASE,
		Timer4 = TIM4_BASE,
	};

	using CallBackFunctionType = void(TIM_HandleTypeDef&);
	static void setCallBack(int timer, CallBackFunctionType* function);
	static CallBackFunctionType* getCallBack(int timer);
	static int getCnt(int timer);

private:
	static CallBackFunctionType* callBack[5];

	friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
	static CallBackFunctionType noneCallBack;
};
