#include <Arduino.h>
#include "SPITransfer.h"
#include "Nokia5110.h"


//功能：Nokia5110对象的构造函数
//参数:
//restPin:复位引脚
//dcPin:命令或者数据操作引脚
//sclkPin:SPI接口的时钟SCLK引脚
//mosiPin:SPI接口的主机输出MOSI引脚
//ssPin:SPI接口的从机使能SS引脚
//ledPin:Nokia5110背光LED控制引脚
//返回值：无
Nokia5110::Nokia5110(unsigned char restPin,unsigned char dcPin,
		unsigned char ssPin,unsigned char ledPin,SPITransfer transfer)
{
	m_reset=restPin;
	m_dc=dcPin;
//	m_sclk=sclkPin;
//	m_mosi=mosiPin;
	m_ss=ssPin;
	m_led=ledPin;
	spiTransfer=transfer;

	pin_Init();
	controlPad_Init();
	spi_Init();
}

//功能：初始化使用构造函数指定的Arduino引脚
//参数：无
//返回值：无
//备注：类的私有方法
void Nokia5110::pin_Init()
{
	pinMode(m_reset,OUTPUT);
	pinMode(m_dc,OUTPUT);
	pinMode(m_ss,OUTPUT);
	pinMode(m_led,OUTPUT);

	digitalWrite(m_dc,HIGH);
	digitalWrite(m_ss,HIGH);
	digitalWrite(m_led,HIGH);
	digitalWrite(m_led,HIGH);
}


//功能：对SPI接口使用缺省值进行初始化
//参数：无
//返回值：无
//备注：类的私有方法
void Nokia5110::spi_Init()
{
//	SPI.beginTransaction(SPISettings(currentControlPad.spiSettings.speed,currentControlPad.spiSettings.bitOrder,currentControlPad.spiSettings.dataMode));
			spiTransfer.Init();
}

//功能：初始化Nokia5110模块的控制寄存器的类内缓存
//参数：无
//返回值：无
//备注：类的私有方法
void Nokia5110::controlPad_Init()
{
//	currentControlPad.spiSettings.speed = 1000;
//	currentControlPad.spiSettings.bitOrder = MSBFIRST;
//	currentControlPad.spiSettings.dataMode = SPI_MODE0;
	currentControlPad.enableFunctions = 0x20;
	currentControlPad.displayMode = 0x08;
	currentControlPad.column = 0;
	currentControlPad.row = 0;
}

//功能：向Nokia5110模块发送指定的命令
//参数：
//cmd：向Nokia5110模块发送的命令，类型unsigned char
//返回值：无
//备注：类的私有方法
void Nokia5110::sendCommand(unsigned char cmd)
{
	  digitalWrite(m_dc,LOW);
	  spiTransfer.Begin();
	  digitalWrite(m_ss,LOW);
	  spiTransfer.Transfer(cmd);
	  digitalWrite(m_ss,HIGH);
	  digitalWrite(m_dc,HIGH);
	  spiTransfer.End();
}

//功能：向Nokia5110模块当前位置发送显示数据
//参数：
//data：向Nokia5110模块发送的显示数据，类型：unsigned char
//返回值：无
//备注：类的私有方法
void Nokia5110::sendData(unsigned char data)
{
	  digitalWrite(m_dc,HIGH);
	  spiTransfer.Begin();
	  digitalWrite(m_ss,LOW);
	  spiTransfer.Transfer(data);
	  digitalWrite(m_ss,HIGH);
	  spiTransfer.End();
}


//功能：对Nokia5110模块进行复位
//参数：无
//返回值：无
void Nokia5110::Reset()
{
	digitalWrite(m_reset, LOW);
	delay(200);
	digitalWrite(m_reset, HIGH);
}

//功能：将指定的列改变Nokia5110的当前列
//参数：
//column：用户指定的列，类型：unsigned char
//返回值：无
void Nokia5110::SetColumn(unsigned char column)
{
  if (column > MAX_COLUMN-1)
	  {
	  	  column = column % MAX_COLUMN;
	  }
	//currentControlPad.column = 0x80;
	currentControlPad.column = column;
	sendCommand(currentControlPad.column|0x80);
}

//功能：获取Nokia5110的当前列
//参数：无
//返回值：Nokia5110的当前列
unsigned char Nokia5110::GetColumn() const
{
	return currentControlPad.column;
}
//功能：根据用户指定的行改变Nokia5110的当前行
//参数：
//row：用户指定的行
//返回值：无
void Nokia5110::SetRow(unsigned char row)
{
  if (row > (MAX_ROW -1))
	  {
      row = row % MAX_ROW;
	  }
	//currentControlPad.row = 0x40;
    currentControlPad.row = row;
	sendCommand(currentControlPad.row|0x40);
}

//功能：获取Nokia5110的当前行
//参数：无
//返回值：Nokia5110的当前行
unsigned char Nokia5110::GetRow() const
{
	return currentControlPad.row;
}

//功能：将Nokia5110设置为上电模式
//参数：无
//返回值：无
void Nokia5110::PowerOn()
{
	bitClear(currentControlPad.enableFunctions, 2);
	sendCommand(currentControlPad.enableFunctions);
}

//功能：将Nokia5110设置为掉电模式
//参数：无
//返回值：无
void Nokia5110::PowerDown()
{
	bitSet(currentControlPad.enableFunctions, 2);
	sendCommand(currentControlPad.enableFunctions);
}

//功能：获取Nokia5110的工作模式
//参数：无
//返回值：NOKIA_SWITCH_STAT类型，返回ON表示Nokia5110处于正常的工作状态，返回OFF表示Nokia5110处于掉电模式
NOKIA_POWER_STAT Nokia5110::GetPowerStat() const {
	return (bitRead(currentControlPad.enableFunctions, 2)) == 0 ? POWER_ON : POWER_DOWN;
}

//功能：设置Nokia5110的显示模式
//参数：
//dm：NOKIA_DISPLAY_MODE类型，用户选择的工作模式
//返回值：无
void Nokia5110::SetDisplayMode(NOKIA_DISPLAY_MODE dm)
{
	  switch(dm)
	  {
	    case DISPLAY_SPACE:
		bitClear(currentControlPad.displayMode, 0);
		bitClear(currentControlPad.displayMode, 2);
	      break;
	    case DISPLAY_ALL_SEGMENT:
		bitClear(currentControlPad.displayMode, 2);
		bitSet(currentControlPad.displayMode, 0);
	      break;
	    case DISPLAY_NORMAL:
		bitClear(currentControlPad.displayMode, 0);
		bitSet(currentControlPad.displayMode, 2);
	      break;
	    case DISPLAY_INVERT:
		bitSet(currentControlPad.displayMode, 0);
		bitSet(currentControlPad.displayMode, 2);
	      break;
	    default:
		bitClear(currentControlPad.displayMode, 0);
		bitSet(currentControlPad.displayMode, 2);
	      break;
	  }
	  sendCommand(currentControlPad.displayMode);
}

//功能：获取Nokia5110的显示模式
//参数：无
//返回值：NOKIA_DISPLAY_MODE类型，表示Nokia5110当前的显示模式
NOKIA_DISPLAY_MODE Nokia5110::GetDisplayMode() const {
	unsigned char mode = currentControlPad.displayMode & 0x05;
	NOKIA_DISPLAY_MODE retVal;
	switch (mode) {
	case 0:
		retVal = DISPLAY_SPACE;
		break;
	case 1:
		retVal = DISPLAY_ALL_SEGMENT;
		break;
	case 4:
		retVal = DISPLAY_NORMAL;
		break;
	case 5:
		retVal = DISPLAY_INVERT;
		break;
	}
	return retVal;
}

//功能：设置Nokia5110的显示扫描方向
//参数：
//dir：NOKIA_DISPLAY_DIRECTION类型，用户指定的显示扫描方向
//返回值：无
void Nokia5110::SetDisplayDirection(NOKIA_DISPLAY_DIRECTION dir)
{
	  if(dir==VIRTICAL)
	  {
		bitSet(currentControlPad.enableFunctions, 1);
	  }
	  else
	  {
		bitClear(currentControlPad.enableFunctions, 1);
	  }
	  sendCommand(currentControlPad.enableFunctions);
}

//功能：获取Nokia5110的显示扫描方向
//参数：无
//返回值：NOKIA_DISPLAY_DIRECTION类型，表示Nokia5110当前的显示扫描方向
NOKIA_DISPLAY_DIRECTION Nokia5110::GetDisplayDirection() const
{

	return (bitRead(currentControlPad.enableFunctions,1)==1)?VIRTICAL:HORIZONTAL;
}

//功能：设置Nokia5110的指令集
//参数：NOKIA_INSTRUCT_SET类型，用户指定的指令集
//返回值：无
void Nokia5110::SetInstructSet(NOKIA_INSTRUCT_SET is)
{
	  if(is==EXTENT_INSTRUCT)
	  {
		bitSet(currentControlPad.enableFunctions, 0);
	  }
	  else
	  {
		bitClear(currentControlPad.enableFunctions, 0);
	  }
	  sendCommand(currentControlPad.enableFunctions);
}

//功能：获取Nokia5110当前的指令集
//参数：无
//返回值：NOKIA_INSTRUCT_SET类型，表示Nokia5110当前的指令集
NOKIA_INSTRUCT_SET Nokia5110::GetInstructSet()const
{
	return (bitRead(currentControlPad.enableFunctions,0)==1)?EXTENT_INSTRUCT:BASE_INSTRUCT;
}

//功能：Nokia5110对象的析构函数，清除对象占用的资源
Nokia5110::~Nokia5110 ()
{
}

//功能：打开Nokia5110模块的背光LED
//参数：无
//返回值：无
void Nokia5110::BacklightOn()
{
	digitalWrite(m_led,LOW);
}

//功能：关闭Nokia5110模块的背光LED
//参数：无
//返回值：无
void Nokia5110::BacklightOff()
{
	digitalWrite(m_led,HIGH);
}

//功能：在当前的行列位置写入数据
//参数：无
//返回值：无
void Nokia5110::Draw(unsigned char data)
{
	sendData(data);
	UpdateCurrentRowAndColumn(GetRow(), GetColumn());
}

//功能：根据输入的行列值和显示数据扫描方向更新当前的行列值
//参数：
//row：指定的行
//column：指定的列
//返回值：无
void Nokia5110::UpdateCurrentRowAndColumn(unsigned char row,
		unsigned char column)
{
	if (GetDisplayDirection() == VIRTICAL)
	{
		if ((row + 1) > MAX_ROW - 1)
		{
			currentControlPad.row = (row + 1) % MAX_ROW;
		}
		else
		{
			currentControlPad.row = row + 1;
		}
	}
	else
	{
		if ((column + 1) > MAX_COLUMN)
		{
			currentControlPad.column = (column + 1) % MAX_COLUMN;
		}
		else
		{
			currentControlPad.column = column + 1;
		}
	}
}

//在指定的行列写入数据
//参数：
//row：指定的行
//column：指定的列
//data：写入的数据
//返回值：空
void Nokia5110::Draw(unsigned char row, unsigned char column,
		unsigned char data)
{
	SetColumn(column);
	SetRow(row);
	sendData(data);

	UpdateCurrentRowAndColumn(row, column);

}

