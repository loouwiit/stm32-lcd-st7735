#include "timer.hpp"
#include "tim.h"

Timer::CallBackFunctionType* Timer::callBack[5] = { nullptr,noneCallBack,noneCallBack,noneCallBack,noneCallBack };


void Timer::setCallBack(int timer, CallBackFunctionType* function)
{
	if (function == nullptr) callBack[timer] = noneCallBack;
	callBack[timer] = function;
}

Timer::CallBackFunctionType* Timer::getCallBack(int timer)
{
	return callBack[timer];
}

int Timer::getCnt(int timer)
{
	switch (timer)
	{
	case 1:
		return TIM1->CNT;
	case 2:
		return TIM2->CNT;
	case 3:
		return TIM3->CNT;
	case 4:
		return TIM4->CNT;
	}
	return -1;
}

void Timer::noneCallBack(TIM_HandleTypeDef& timer)
{
	(void)timer;
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	using TimerInstance = Timer::TimerInstance;
	TIM_HandleTypeDef& timer = *htim;
	switch ((TimerInstance)(int)timer.Instance)
	{
	case TimerInstance::Timer1:
		Timer::callBack[1](timer);
		break;
	case TimerInstance::Timer2:
		Timer::callBack[2](timer);
		break;
	case TimerInstance::Timer3:
		Timer::callBack[3](timer);
		break;
	case TimerInstance::Timer4:
		Timer::callBack[4](timer);
		break;
	}
}