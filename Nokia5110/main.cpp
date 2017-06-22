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
	led.SetAvaliable(ON);
	led.SetDisplayMode(DISPLAY_NORMAL);
}

void loop()
{
	for (unsigned char i = 0; i < 6; i++) {
		for (unsigned char j = 0; j < 84; j++) {
			led.drawDot(i, j, OR);
		}
	}
}



