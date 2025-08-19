#pragma once

#include "gpio.hpp"
#include "spi.hpp"
#include "vector.hpp"

class LCD
{
public:
	using Color = uint16_t;

	LCD() = default;
	LCD(LCD&) = default;
	LCD(LCD&&) = default;
	LCD(SPI spi, GPIO dataControlSelect, GPIO reset);

	void reset();
	void init(Color color = 0x0000);

	void setAddressWindow(Vector2uc start, Vector2uc end);
	void drawPixel(Vector2uc position, Color color);

	void drawLine(Vector2uc start, Vector2uc end, Color color = 0xFFFF);
	void drawRectangle(Vector2uc start, Vector2uc end, Color color = 0xFFFF);
	// void drawTriangle(Vector2uc position[3], Color color = 0xFFFF);

	int drawText(Vector2uc position, char text, Color textColor = 0xFFFF, Color backgroundColor = 0x0000);
	int drawText(Vector2uc position, const char* text, Color textColor = 0xFFFF, Color backgroundColor = 0x0000);
	int drawNumber(Vector2uc position, int number, unsigned base = 10, Color textColor = 0xFFFF, Color backgroundColor = 0x0000);
	int drawNumber(Vector2uc position, unsigned number, unsigned base = 10, Color textColor = 0xFFFF, Color backgroundColor = 0x0000);

	void clear(Color color = 0x0000);

	void test();

protected:
	SPI& commandMode();
	SPI& dataMode();
	SPI& drawMode();

	void command(unsigned char command);
	void data(unsigned char data);
	void data(unsigned char* data, uint16_t size);

private:
	SPI spi;
	GPIO dataControlSelect;
	GPIO resetGpio;
};
