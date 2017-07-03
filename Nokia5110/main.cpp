/*
 * main.cpp
 *
 *  Created on: 2017��6��15��
 *      Author: li
 */
#include "Nokia5110.h"
Nokia5110 led(7,8,0,0,10,9);
unsigned char count = 0;
void setup()
{
	//led.switchLED();
	led.Reset();
	led.OnLED();
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



