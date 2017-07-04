/*
 * main.cpp
 *
 *  Created on: 2017Äê6ÔÂ15ÈÕ
 *      Author: li
 */
#include <SPI.h>
#include "SPITransfer.h"
#include "Nokia5110.h"

void spi_init()
{
	SPI.beginTransaction(SPISettings(1000,MSBFIRST,SPI_MODE0));
}

unsigned char spi_transfer(unsigned char data)
{
	return SPI.transfer(data);
}
SPITransfer transfer(spi_init,spi_transfer,SPI.begin,SPI.end);

Nokia5110 led(7,8,10,9,transfer);
unsigned char count = 0;

void setup()
{
	led.Reset();
	led.BacklightOn();
	led.PowerOn();
	led.SetDisplayMode(DISPLAY_NORMAL);
	led.SetDisplayDirection(VIRTICAL);
	led.SetInstructSet(BASE_INSTRUCT);
	led.SetColumn(0);
	led.SetRow(0);
	led.Draw(0x55);
	led.Draw(0x80);
}

void loop()
{
	led.SetDisplayDirection(HORIZONTAL);
	led.Draw(3, 42, 0xff);
	led.Draw(0xff);
}



