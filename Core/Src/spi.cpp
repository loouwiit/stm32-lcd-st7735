#include "spi.hpp"

bool SPI::transmit(const unsigned char* data, uint16_t size, int timeOutAsTick)
{
	bool ret = HAL_SPI_Transmit(spiHandle, (unsigned char*)data, size, timeOutAsTick) != HAL_StatusTypeDef::HAL_OK;
	// HAL data char*没const
	return ret;
}

bool SPI::transmit(const unsigned char data)
{
	return transmit(&data, 1);
}

bool SPI::transmit(const DataPack dataPack)
{
	return transmit(dataPack.data, dataPack.size);
}

SPI& SPI::operator<<(const unsigned char data)
{
	transmit(&data, 1);
	return *this;
}

SPI& SPI::operator<<(const DataPack dataPack)
{
	transmit(dataPack.data, dataPack.size);
	return *this;
}
