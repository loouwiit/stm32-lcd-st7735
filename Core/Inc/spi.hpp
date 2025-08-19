#pragma once
#include "spi.h"
#include "gpio.hpp"

struct DataPack
{
	uint16_t size;
	unsigned char* data;
};

class SPI
{
public:
	SPI() = default;
	SPI(SPI&) = default;
	SPI(SPI&&) = default;
	SPI(SPI_HandleTypeDef& spiHandle) : spiHandle{ &spiHandle } {}
	SPI(SPI_HandleTypeDef* spiHandle) : spiHandle{ spiHandle } {}

	bool transmit(const unsigned char* data, uint16_t size, int timeOutAsTick = 100);
	bool transmit(const unsigned char data);
	bool transmit(const DataPack data);

	SPI& operator<<(const unsigned char data);
	SPI& operator<<(const DataPack dataPack);

private:
	SPI_HandleTypeDef* spiHandle = nullptr;
};
