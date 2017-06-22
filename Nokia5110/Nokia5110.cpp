#include <Arduino.h>
#include <SPI.h>
#include "Nokia5110.h"

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

void Nokia5110::spi_Init()
{
	SPI.beginTransaction(SPISettings(currentControlPad.spiSettings.speed,currentControlPad.spiSettings.bitOrder,currentControlPad.spiSettings.dataMode));
}

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

void Nokia5110::sendData(unsigned char data)
{
	  digitalWrite(m_dc,HIGH);
	  SPI.begin();
	  digitalWrite(m_ss,LOW);
	  SPI.transfer(data);
	  digitalWrite(m_ss,HIGH);
	  SPI.end();
}

void Nokia5110::Reset()
{
	digitalWrite(m_reset, LOW);
	delay(200);
	digitalWrite(m_reset, HIGH);
}

void Nokia5110::setColumn(unsigned char colPos)
{
	if (colPos > 83)
	  {
		colPos = colPos % 84;
	  }
	currentControlPad.column = 0x80;
	currentControlPad.column |= colPos;
	sendCommand(currentControlPad.column);
}

void Nokia5110::setRow(unsigned char rowPos)
{
	if (rowPos > 5)
	  {
		rowPos = rowPos % 6;
	  }
	currentControlPad.row = 0x40;
	currentControlPad.row |= rowPos;
	sendCommand(currentControlPad.row);
}

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

NOKIA_SWITCH_STAT Nokia5110::GetAvaliable() const {
	return (bitRead(currentControlPad.enableFunctions, 2)) == 0 ? ON : OFF;
}

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

NOKIA_DISPLAY_DIRECTION Nokia5110::GetDisplayDirection() const
{

	return (bitRead(currentControlPad.enableFunctions,1)==1)?VIRTICAL:HORIZONTAL;
}


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

NOKIA_INSTRUCT_SET Nokia5110::GetInstructSet()const
{
	return (bitRead(currentControlPad.enableFunctions,0)==1)?EXTENT_INSTRUCT:BASE_INSTRUCT;
}


void Nokia5110::draw(unsigned char x,unsigned char y,unsigned char data)
{
	setColumn(x);
	setRow(y);
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

Nokia5110::~Nokia5110 ()
{
  delete[] m_buffer;
}
