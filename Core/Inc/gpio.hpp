#pragma once
#include "gpio.h"

class GPIO
{
public:
	GPIO() = default;
	GPIO(GPIO&) = default;
	GPIO(GPIO_TypeDef* gpioSet, uint16_t gpioPin) : gpioSet{ gpioSet }, gpioPin{ gpioPin } {}

	operator bool();
	GPIO& operator=(bool level);

	GPIO_TypeDef* gpioSet = nullptr;
	uint16_t gpioPin = 0;
};
