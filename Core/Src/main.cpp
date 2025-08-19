#include "main.hpp"
#include "gpio.hpp"
#include "timer.hpp"
#include "lcd.hpp"
#include "lineChart.hpp"

unsigned clickCount = 0;

GPIO led{ LED_GPIO_Port, LED_Pin };
LCD lcd{ {hspi1}, {LCD_DC_GPIO_Port, LCD_DC_Pin}, {LCD_RST_GPIO_Port, LCD_RST_Pin} };

void clickDetect(TIM_HandleTypeDef&);

void app_main()
{
	led = true;
	lcd.init(0xFFFF);
	lcd.drawRectangle({ 1,1 }, { 158,78 }, 0x0000);
	led = false;

	Timer::setCallBack(2, clickDetect);

	LineChart<unsigned> curve{ 70 };
	curve.setPosition({ 10,30 }, { 140,40 });

	curve.setAutoMinMax(false, true);
	curve.setMinMax(0, 0);

	unsigned lastClickCount = -1;
	while (true)
	{
		if (lastClickCount != clickCount)
		{
			if (clickCount < lastClickCount)
				lcd.drawText({ 10,10 }, "          ");
			lcd.drawNumber({ 10,10 }, clickCount);
			lastClickCount = clickCount;
		}

		curve.addData(clickCount);
		curve.drawTo(lcd);
		HAL_Delay(10);
	}
}

void clickDetect(TIM_HandleTypeDef&)
{
	GPIO led{ LED_GPIO_Port, LED_Pin };
	GPIO in{ Button_GPIO_Port, Button_Pin }; // Hi -> Low

	constexpr unsigned char autoSubTimeReload = 100;
	constexpr unsigned doubleClickTime = 30;
	constexpr unsigned holdTime = 50;
	static unsigned timeCount = 0;
	static unsigned char autoSubTime = 0;
	static unsigned holdExpectedReseaseTime = -1;
	static unsigned doubleClickExpectedStartTime = -1;
	static unsigned doubleClickExpectedEndTime = -1;

	timeCount++; // 10ms

	if (autoSubTime > 0)
	{
		autoSubTime--;
		if (autoSubTime == 0)
		{
			if (clickCount > 0)
			{
				clickCount--;
				autoSubTime = autoSubTimeReload;
			}
		}
	}

	static bool lastState = in;
	if (in == lastState)
	{
		// stable
		if (in)
		{
			// none
			return;
		}
		// hold
		if (timeCount > holdExpectedReseaseTime)
		{
			// hold for enoght time
			clickCount++;
		}
		return;
	}
	lastState = in;

	// change
	if (in)
	{
		// release
		autoSubTime = autoSubTimeReload; // enable auto sub
		holdExpectedReseaseTime = -1; // clean for hold
		if (timeCount > holdExpectedReseaseTime)
		{
			// hold befor
			doubleClickExpectedStartTime = -1; // clean for double click
			return;
		}
		else
		{
			// click
			if (doubleClickExpectedStartTime < timeCount && timeCount < doubleClickExpectedEndTime)
			{
				// double click
				clickCount--; // the first click add one
				clickCount *= 2;
				doubleClickExpectedStartTime = -1; // clean for double click
				return;
			}
			else
			{
				// out of double click time
				// click once
				clickCount++;
				doubleClickExpectedStartTime = timeCount; // set double click time
				doubleClickExpectedEndTime = timeCount + doubleClickTime;
				return;
			}
		}
	}
	else
	{
		// press
		holdExpectedReseaseTime = timeCount + holdTime; // 500ms
		autoSubTime = 0; // disable auto sub
		return;
	}
}
