#include <Arduino.h>
#include <SPI.h>
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
Nokia5110::Nokia5110(unsigned char restPin,unsigned char dcPin,unsigned char sclkPin,unsigned char mosiPin,
		unsigned char ssPin,unsigned char ledPin)
{
	m_reset=restPin;
	m_dc=dcPin;
	m_sclk=sclkPin;
	m_mosi=mosiPin;
	m_ss=ssPin;
	m_led=ledPin;

	pin_Init();
	controlPad_Init();
	spi_Init();
	m_buffer = new unsigned char[MAX_ROW * MAX_COLUMN];
	m_pCurrentOfBuffer = m_buffer;
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
	SPI.beginTransaction(SPISettings(currentControlPad.spiSettings.speed,currentControlPad.spiSettings.bitOrder,currentControlPad.spiSettings.dataMode));
}

//功能：初始化Nokia5110模块的控制寄存器的类内缓存
//参数：无
//返回值：无
//备注：类的私有方法
void Nokia5110::controlPad_Init()
{
	currentControlPad.spiSettings.speed = 1000;
	currentControlPad.spiSettings.bitOrder = MSBFIRST;
	currentControlPad.spiSettings.dataMode = SPI_MODE0;
	currentControlPad.enableFunctions = 0x20;
	currentControlPad.displayMode = 0x08;
	currentControlPad.column = 0x80;
	currentControlPad.row = 0x40;
}

//功能：向Nokia5110模块发送指定的命令
//参数：
//cmd：向Nokia5110模块发送的命令，类型unsigned char
//返回值：无
//备注：类的私有方法
void Nokia5110::sendCommand(unsigned char cmd)
{
	  digitalWrite(m_dc,LOW);
	  SPI.begin();
	  digitalWrite(m_ss,LOW);
	  SPI.transfer(cmd);
	  digitalWrite(m_ss,HIGH);
	  digitalWrite(m_dc,HIGH);
	  SPI.end();
}

//功能：向Nokia5110模块当前位置发送显示数据
//参数：
//data：向Nokia5110模块发送的显示数据，类型：unsigned char
//返回值：无
//备注：类的私有方法
void Nokia5110::sendData(unsigned char data)
{
	  digitalWrite(m_dc,HIGH);
	  SPI.begin();
	  digitalWrite(m_ss,LOW);
	  SPI.transfer(data);
	  digitalWrite(m_ss,HIGH);
	  SPI.end();
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
void Nokia5110::setColumn (unsigned char column)
{
  if (column > MAX_COLUMN-1)
	  {
	  	  column = column % MAX_COLUMN;
	  }
	currentControlPad.column = 0x80;
	currentControlPad.column |= column;
	sendCommand(currentControlPad.column);
}

//功能：获取Nokia5110的当前列
//参数：无
//返回值：Nokia5110的当前列
unsigned char Nokia5110::getColumn()
{
	return currentControlPad.column&0x7f;
}
//功能：根据用户指定的行改变Nokia5110的当前行
//参数：
//row：用户指定的行
//返回值：无
void Nokia5110::setRow (unsigned char row)
{
  if (row > (MAX_ROW -1))
	  {
      row = row % MAX_ROW;
	  }
	currentControlPad.row = 0x40;
    currentControlPad.row |= row;
	sendCommand(currentControlPad.row);
}

//功能：获取Nokia5110的当前行
//参数：无
//返回值：Nokia5110的当前行
unsigned char Nokia5110::getRow()
{
	return currentControlPad.row&0x3f;
}
//功能：设置Nokia5110的工作模式
//参数：
//s：指定的Nokia5110的状态，类型：NOKIA_SWITCH_STAT
//返回值：无
void Nokia5110::SetAvaliable(NOKIA_SWITCH_STAT s)
{
	  if(s==OFF)
	  {
		bitSet(currentControlPad.enableFunctions, 2);
	  }
	  else
	  {
		bitClear(currentControlPad.enableFunctions, 2);
	  }
	  sendCommand(currentControlPad.enableFunctions);
}

//功能：获取Nokia5110的工作模式
//参数：无
//返回值：NOKIA_SWITCH_STAT类型，返回ON表示Nokia5110处于正常的工作状态，返回OFF表示Nokia5110处于掉电模式
NOKIA_SWITCH_STAT Nokia5110::GetAvaliable() const {
	return (bitRead(currentControlPad.enableFunctions, 2)) == 0 ? ON : OFF;
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
  delete[] m_buffer;
}

//功能：打开Nokia5110模块的背光LED
//参数：无
//返回值：无
void Nokia5110::OnLED()
{
	digitalWrite(m_led,LOW);
}

//功能：关闭Nokia5110模块的背光LED
//参数：无
//返回值：无
void Nokia5110::OffLED()
{
	digitalWrite(m_led,HIGH);
}

void Nokia5110::draw(unsigned char row,unsigned char column,unsigned char data)
{
	currentControlPad.row=row;
	currentControlPad.column=column;

	sendData(data);
}

void Nokia5110::drawDot(unsigned char x, unsigned char y,
		NOKIA_DISPLAY_WRITE_MODE mode)
{
  unsigned char bytes = y / 8;
  unsigned char bit = y % 8;
  unsigned char data = 0;
	unsigned char bdata = 0;
  bitSet(data, bit);
  if (bytes > 5)
    bytes %= 6;
  if (x > 83)
    x %= 84;
	bdata = *(m_buffer + bytes * 84 + x);
	switch (mode) {
	case AND:
		data &= bdata;
		break;
	case OR:
		data |= bdata;
		break;
	case XOR:
		data ^= bdata;
		break;
	}

  draw (x, bytes, data);
}

unsigned char * Nokia5110::GetCurrentOfByte() const
{
  return m_buffer + currentControlPad.row * MAX_COLUMN
      + currentControlPad.column;
}

void Nokia5110::Refresh(unsigned char x, unsigned char y, unsigned char width,
		    unsigned char high)
{
  if (GetDisplayDirection () == HORIZONTAL)
    {
      m_pCurrentOfBuffer = m_buffer + y * MAX_COLUMN + x;
      for (unsigned char row = 0; row < high; row++)
	{
	  for (unsigned char column = 0; column < width; column++)
	    {
	      m_pCurrentOfBuffer = m_buffer + row * MAX_COLUMN + column;
	      sendData (*m_pCurrentOfBuffer);
	    }
	}
    }
}

