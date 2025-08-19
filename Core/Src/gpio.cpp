#include "gpio.hpp"

GPIO::operator bool()
{
	return HAL_GPIO_ReadPin(gpioSet, gpioPin) == GPIO_PIN_SET;
}

GPIO& GPIO::operator=(bool level)
{
	HAL_GPIO_WritePin(gpioSet, gpioPin, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
	return *this;
}
