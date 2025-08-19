#include "lcd.hpp"
#include "font.hpp"

LCD::LCD(SPI spi, GPIO dataControlSelect, GPIO reset) : spi{ spi }, dataControlSelect{ dataControlSelect }, resetGpio{ reset }
{}

void LCD::reset()
{
	resetGpio = false;
	HAL_Delay(100);
	resetGpio = true;
	HAL_Delay(100);
}

void LCD::init(Color color)
{
	reset();
	command(0x11); // SLPOUT: sleep out
	HAL_Delay(120);

	command(0xB1); // FRMCTR1: normal mode
	dataMode() << 0x05 << 0x3A << 0x3A;

	command(0xB2); // FRMCTR2: idle mode
	dataMode() << 0x05 << 0x3A << 0x3A;

	command(0xB3); // FRMCTR3: partial mode
	dataMode() << 0x05 << 0x3A << 0x3A << 0x05 << 0x3A << 0x3A;

	command(0xB4); // INVCTR: inversion control
	data(0x03);

	command(0xC0); // PWCTR1: power control setting
	dataMode() << 0x62 << 0x02 << 0x04;

	command(0xC1); // PWCTR2
	data(0xC0);

	command(0xC2); // PWCTR3: in normal mode
	dataMode() << 0x0D << 0x00;

	command(0xC3); // PWCTR4: in idle mode
	dataMode() << 0x8D << 0x6A;

	command(0xC4); // PWCTR5: in partial mode
	dataMode() << 0x8D << 0xEE;

	command(0xC5); // VMCTR1: VCOM control 1
	data(0x0E);

	// inverse	: 0x21
	// not		: 0x20
	command(0x21); // INVON

	command(0x3A); // COLMOD: pixel format
	data(0x05); // 16bit / pixel

	command(0xE0); // GAMCTRP1: gamma adjustment+
	dataMode() << 0x10 << 0x0E << 0x02 << 0x03 << 0x0E << 0x07 << 0x02 << 0x07 << 0x0A << 0x12 << 0x27 << 0x37 << 0x00 << 0x0D << 0x0E << 0x10;

	command(0xE1); // GAMCTRN1: gamma adjustment-
	dataMode() << 0x10 << 0x0E << 0x03 << 0x03 << 0x0F << 0x06 << 0x02 << 0x08 << 0x0A << 0x13 << 0x26 << 0x36 << 0x00 << 0x0D << 0x0E << 0x10;

	command(0x13); // NORON: normal mode
	HAL_Delay(10);

	command(0x36); // MADCTL: memory data access control
	data(0xA8);

	command(0x29); // display on

	clear(color);
}

SPI& LCD::commandMode()
{
	dataControlSelect = false;
	return spi;
}

SPI& LCD::dataMode()
{
	dataControlSelect = true;
	return spi;
}

SPI& LCD::drawMode()
{
	command(0x2C); // RAMWR: menmory write
	return dataMode();
}

void LCD::command(unsigned char command)
{
	commandMode();
	spi << command;
}

void LCD::data(unsigned char data)
{
	dataMode();
	spi << data;
}

void LCD::data(unsigned char* data, uint16_t size)
{
	dataMode();
	spi.transmit(data, size);
}

void LCD::setAddressWindow(Vector2uc start, Vector2uc end)
{
	command(0x2A); // CASET: column address set
	dataMode() << 0x00 << start.x + (unsigned int)1 << 0x00 << end.x + (unsigned int)1;

	command(0x2B); // RASET: row address set
	dataMode() << 0x00 << start.y + (unsigned int)0x1A << 0x00 << end.y + (unsigned int)0x1A;
}

void LCD::drawPixel(Vector2uc position, Color color)
{
	setAddressWindow(position, position);
	drawMode() << (color >> 8) << (color & 0xFF);
}

void LCD::drawLine(Vector2uc start, Vector2uc end, Color color)
{
	using Vector2s = Vector2<signed short>;
	Vector2s delta = (Vector2s)end - (Vector2s)start;
	Vector2uc absDelta = { (unsigned char)abs(delta.x), (unsigned char)abs(delta.y) };
	if (absDelta.x > absDelta.y)
	{
		if (delta.x < 0)
		{
			start.swap(end);
			delta = -delta;
		}

		float k = (float)delta.y / (float)delta.x;

		Vector2f position = start;
		while ((int)position.x <= end.x)
		{
			drawPixel((Vector2uc)position, color);
			position.x++;
			position.y += k;
		}
	}
	else
	{
		if (delta.y < 0)
		{
			start.swap(end);
			delta = -delta;
		}

		float l = (float)delta.x / (float)delta.y;

		Vector2f position = start;
		while ((int)position.y <= end.y)
		{
			drawPixel((Vector2uc)position, color);
			position.y++;
			position.x += l;
		}
	}
}

void LCD::drawRectangle(Vector2uc start, Vector2uc end, Color color)
{
	setAddressWindow(start, end);
	drawMode();

	unsigned short count = (end.x - start.x + 1) * (end.y - start.y + 1);

	for (int i = 0; i < count; i++)
		spi << (color >> 8) << (color & 0xFF);
}

// void LCD::drawTriangle(Vector2uc position[3], Color color)
// {
// 	if (position[0].y > position[1].y) position[0].swap(position[1]);
// 	if (position[0].y > position[2].y) position[0].swap(position[2]);
// 	if (position[1].y > position[2].y) position[1].swap(position[2]);
// 	// 0 <= 1 <= 2
// }

int LCD::drawText(Vector2uc position, char text, Color textColor, Color backgroundColor)
{
	if (text < 0x20) return 0;
	text -= 0x20;

	setAddressWindow(position, position + Vector2uc{ 7,15 });
	drawMode();

	const unsigned char* font = fonts[(unsigned char)text];

	for (unsigned char i = 0;i < 16;i++)
	{
		const unsigned char& mod = font[i];
		for (unsigned char j = 0; j < 8; j++)
		{
			bool draw = mod & ((1 << 7) >> j);
			if (draw)
				spi << (textColor >> 8) << (textColor & 0xFF);
			else
				spi << (backgroundColor >> 8) << (backgroundColor & 0xFF);
		}
	}
	return 1;
}

int LCD::drawText(Vector2uc position, const char* text, Color textColor, Color BackgroundColor)
{
	int drawCount = 0;

	Vector2uc lineBegin = position;
	for (; *text != '\0'; text++)
	{
		switch (*text)
		{
		case '\n':
			position = lineBegin + Vector2uc{ 0, 16 };
			lineBegin = position;
			continue;
		case '\r':
			position = lineBegin;
			continue;
		case '\b':
			position.x -= 8;
			continue;
		case '\t':
			position.x += 8;
			continue;
		}
		drawCount += drawText(position, *text, textColor, BackgroundColor);

		position.x += 8;
	}

	return drawCount;
}

int LCD::drawNumber(Vector2uc position, int number, unsigned base, Color textColor, Color backgroundColor)
{
	if (number < 0)
	{
		drawText(position, '-', textColor, backgroundColor);
		return 1 + drawNumber(position + Vector2uc{ 8, 0 }, (unsigned)-number, base, textColor, backgroundColor);
	}
	else return drawNumber(position, number, base, textColor, backgroundColor);
}

int LCD::drawNumber(Vector2uc position, unsigned number, unsigned base, Color textColor, Color backgroundColor)
{
	if (number == 0)
	{
		drawText(position, '0', textColor, backgroundColor);
		return 1;
	}
	int drawCount = 0;
	auto draw = [this, &base, &position, &textColor, &backgroundColor, &drawCount](unsigned number, auto draw)->void
		{
			if (number >= base)
			{
				draw(number / base, draw);
				position.x += 8;
			}
			this->drawText(position, '0' + (number % 10), textColor, backgroundColor);
			drawCount++;
		};
	draw(number, draw);
	return drawCount;
}

void LCD::clear(Color color)
{
	drawRectangle({ 0,0 }, { 159,79 }, color);
}

void LCD::test()
{
	drawText({ 5,10 }, "QWERTYUIO\nPASDFGHJK\nLZXCVBNM", 0xFFFF, 0x00FF);
	drawText({ 85,10 }, "qwertyuio\npasdfghjk\nlzxcvbnm", 0xFFFF, 0x00FF);
	HAL_Delay(2000);
	drawText({ 5,10 }, "         \n         \n         ");
	drawText({ 85,10 }, "         \n         \n         ");
	drawText({ 5,10 }, "123456789\n0!@#$%^&*\n()`~-=_+");
	drawText({ 85,10 }, "[]{}|\\:;\n\"'<>,.?/\nLCD OK");
	HAL_Delay(2000);
	drawText({ 5,10 }, "         \n         \n         ");
	drawText({ 85,10 }, "         \n         \n         ");
	drawText({ 5,10 }, "The quick brown\nfox jumps over\nthe lazy dog");
	HAL_Delay(1500);
	drawText({ 5,10 }, "\nfox\t\t\t\t\t\t\t    \n         dog", 0x0000, 0x00FF);
	HAL_Delay(500);
	drawText({ 5,10 }, "               \n              \n            ");
}
