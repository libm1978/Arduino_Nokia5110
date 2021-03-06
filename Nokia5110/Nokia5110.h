/*
 * Nokia5110.h
 *
 *  Created on: 2017年6月15日
 *      Author: li
 */

#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include <Arduino.h>
#include "SPITransfer.h"

//代表Nokia5110的活动状态
typedef enum
{
   POWER_ON,//工作模式
   POWER_DOWN//掉电模式
}NOKIA_POWER_STAT;

//用于指定Nokia5110显示模式
typedef enum
{
	DISPLAY_SPACE,
   DISPLAY_ALL_SEGMENT,
   DISPLAY_NORMAL,
   DISPLAY_INVERT
} NOKIA_DISPLAY_MODE;

//用于指定Nokia5110显示数据扫描方向
typedef enum
{
  HORIZONTAL,
  VIRTICAL
}NOKIA_DISPLAY_DIRECTION;

//用于指定Nokia5110的指令集
typedef enum
{
  BASE_INSTRUCT,
  EXTENT_INSTRUCT
}NOKIA_INSTRUCT_SET;


//用于表示SPI的基本通讯设置，参考Arduino的SPI库
//typedef struct
//{
//  unsigned char speed;//数据传输速度
//  unsigned char bitOrder;//数据传输顺序
//  unsigned char dataMode;//数据传输模式
//}Nokia_SPI_Settings;


class Nokia5110
{
private:
	//用于定义Nokia5110的控制寄存器中数据缓存
	typedef struct
  {
//		Nokia_SPI_Settings spiSettings;//SPI通讯的基本设置
		unsigned char enableFunctions;//Nokia5110的功能控制寄存器
		unsigned char displayMode;//Nokia5110的显示模式寄存器
		unsigned char column;//Nokia5110当前的列
		unsigned char row;//Nokia5110当前的行
  } Nokia5110_ControlPad;

  	 //Nokia5110的最大列数常量
	static const unsigned char MAX_COLUMN = 84;
	//Nokia5110的最大行数常量
	static const unsigned char MAX_ROW = 6;

	SPITransfer spiTransfer;
	//Nokia5110的控制寄存器对象内缓存
	Nokia5110_ControlPad currentControlPad;



	void controlPad_Init();
	void UpdateCurrentRowAndColumn(unsigned char row, unsigned char column);

protected:
	//Reset引脚
	unsigned char m_reset;
	//数据命令切换引脚
	unsigned char m_dc;
	//背光LED控制引脚
	unsigned char m_led;
	//SPI从机使能引脚
	unsigned char m_ss;
	//SPI通讯时钟引脚
//	unsigned char m_sclk;
	//SPI通讯数据传输引脚
//	unsigned char m_mosi;

	virtual void pin_Init();
	virtual void spi_Init();
	virtual void sendCommand(unsigned char cmd);
	virtual void sendData(unsigned char data);

public:
	Nokia5110(unsigned char resetPin,unsigned char dcPin,unsigned char ssPin,unsigned char ledPin,SPITransfer transfer);
	void BacklightOn();
	void BacklightOff();
	void Reset();
	void PowerOn();
	void PowerDown();
	NOKIA_POWER_STAT GetPowerStat() const;
	void SetDisplayMode(NOKIA_DISPLAY_MODE m);
	NOKIA_DISPLAY_MODE GetDisplayMode() const;
	void SetDisplayDirection(NOKIA_DISPLAY_DIRECTION d);
	NOKIA_DISPLAY_DIRECTION GetDisplayDirection() const;
	void SetInstructSet(NOKIA_INSTRUCT_SET is);
	NOKIA_INSTRUCT_SET GetInstructSet() const;
	void SetColumn(unsigned char column);
	unsigned char GetColumn() const;
	void SetRow(unsigned char row);
	unsigned char GetRow() const;
	void Draw(unsigned char data);
	void Draw(unsigned char row, unsigned char column, unsigned char data);
	virtual ~Nokia5110 ();
};

#endif /* NOKIA5110_H_ */
