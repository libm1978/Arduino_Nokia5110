#include <Arduino.h>
#include "SPITransfer.h"
#include "Nokia5110.h"


//���ܣ�Nokia5110����Ĺ��캯��
//����:
//restPin:��λ����
//dcPin:����������ݲ�������
//sclkPin:SPI�ӿڵ�ʱ��SCLK����
//mosiPin:SPI�ӿڵ��������MOSI����
//ssPin:SPI�ӿڵĴӻ�ʹ��SS����
//ledPin:Nokia5110����LED��������
//����ֵ����
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

//���ܣ���ʼ��ʹ�ù��캯��ָ����Arduino����
//��������
//����ֵ����
//��ע�����˽�з���
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


//���ܣ���SPI�ӿ�ʹ��ȱʡֵ���г�ʼ��
//��������
//����ֵ����
//��ע�����˽�з���
void Nokia5110::spi_Init()
{
//	SPI.beginTransaction(SPISettings(currentControlPad.spiSettings.speed,currentControlPad.spiSettings.bitOrder,currentControlPad.spiSettings.dataMode));
			spiTransfer.Init();
}

//���ܣ���ʼ��Nokia5110ģ��Ŀ��ƼĴ��������ڻ���
//��������
//����ֵ����
//��ע�����˽�з���
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

//���ܣ���Nokia5110ģ�鷢��ָ��������
//������
//cmd����Nokia5110ģ�鷢�͵��������unsigned char
//����ֵ����
//��ע�����˽�з���
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

//���ܣ���Nokia5110ģ�鵱ǰλ�÷�����ʾ����
//������
//data����Nokia5110ģ�鷢�͵���ʾ���ݣ����ͣ�unsigned char
//����ֵ����
//��ע�����˽�з���
void Nokia5110::sendData(unsigned char data)
{
	  digitalWrite(m_dc,HIGH);
	  spiTransfer.Begin();
	  digitalWrite(m_ss,LOW);
	  spiTransfer.Transfer(data);
	  digitalWrite(m_ss,HIGH);
	  spiTransfer.End();
}


//���ܣ���Nokia5110ģ����и�λ
//��������
//����ֵ����
void Nokia5110::Reset()
{
	digitalWrite(m_reset, LOW);
	delay(200);
	digitalWrite(m_reset, HIGH);
}

//���ܣ���ָ�����иı�Nokia5110�ĵ�ǰ��
//������
//column���û�ָ�����У����ͣ�unsigned char
//����ֵ����
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

//���ܣ���ȡNokia5110�ĵ�ǰ��
//��������
//����ֵ��Nokia5110�ĵ�ǰ��
unsigned char Nokia5110::GetColumn() const
{
	return currentControlPad.column;
}
//���ܣ������û�ָ�����иı�Nokia5110�ĵ�ǰ��
//������
//row���û�ָ������
//����ֵ����
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

//���ܣ���ȡNokia5110�ĵ�ǰ��
//��������
//����ֵ��Nokia5110�ĵ�ǰ��
unsigned char Nokia5110::GetRow() const
{
	return currentControlPad.row;
}

//���ܣ���Nokia5110����Ϊ�ϵ�ģʽ
//��������
//����ֵ����
void Nokia5110::PowerOn()
{
	bitClear(currentControlPad.enableFunctions, 2);
	sendCommand(currentControlPad.enableFunctions);
}

//���ܣ���Nokia5110����Ϊ����ģʽ
//��������
//����ֵ����
void Nokia5110::PowerDown()
{
	bitSet(currentControlPad.enableFunctions, 2);
	sendCommand(currentControlPad.enableFunctions);
}

//���ܣ���ȡNokia5110�Ĺ���ģʽ
//��������
//����ֵ��NOKIA_SWITCH_STAT���ͣ�����ON��ʾNokia5110���������Ĺ���״̬������OFF��ʾNokia5110���ڵ���ģʽ
NOKIA_POWER_STAT Nokia5110::GetPowerStat() const {
	return (bitRead(currentControlPad.enableFunctions, 2)) == 0 ? POWER_ON : POWER_DOWN;
}

//���ܣ�����Nokia5110����ʾģʽ
//������
//dm��NOKIA_DISPLAY_MODE���ͣ��û�ѡ��Ĺ���ģʽ
//����ֵ����
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

//���ܣ���ȡNokia5110����ʾģʽ
//��������
//����ֵ��NOKIA_DISPLAY_MODE���ͣ���ʾNokia5110��ǰ����ʾģʽ
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

//���ܣ�����Nokia5110����ʾɨ�跽��
//������
//dir��NOKIA_DISPLAY_DIRECTION���ͣ��û�ָ������ʾɨ�跽��
//����ֵ����
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

//���ܣ���ȡNokia5110����ʾɨ�跽��
//��������
//����ֵ��NOKIA_DISPLAY_DIRECTION���ͣ���ʾNokia5110��ǰ����ʾɨ�跽��
NOKIA_DISPLAY_DIRECTION Nokia5110::GetDisplayDirection() const
{

	return (bitRead(currentControlPad.enableFunctions,1)==1)?VIRTICAL:HORIZONTAL;
}

//���ܣ�����Nokia5110��ָ�
//������NOKIA_INSTRUCT_SET���ͣ��û�ָ����ָ�
//����ֵ����
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

//���ܣ���ȡNokia5110��ǰ��ָ�
//��������
//����ֵ��NOKIA_INSTRUCT_SET���ͣ���ʾNokia5110��ǰ��ָ�
NOKIA_INSTRUCT_SET Nokia5110::GetInstructSet()const
{
	return (bitRead(currentControlPad.enableFunctions,0)==1)?EXTENT_INSTRUCT:BASE_INSTRUCT;
}

//���ܣ�Nokia5110����������������������ռ�õ���Դ
Nokia5110::~Nokia5110 ()
{
}

//���ܣ���Nokia5110ģ��ı���LED
//��������
//����ֵ����
void Nokia5110::BacklightOn()
{
	digitalWrite(m_led,LOW);
}

//���ܣ��ر�Nokia5110ģ��ı���LED
//��������
//����ֵ����
void Nokia5110::BacklightOff()
{
	digitalWrite(m_led,HIGH);
}

//���ܣ��ڵ�ǰ������λ��д������
//��������
//����ֵ����
void Nokia5110::Draw(unsigned char data)
{
	sendData(data);
	UpdateCurrentRowAndColumn(GetRow(), GetColumn());
}

//���ܣ��������������ֵ����ʾ����ɨ�跽����µ�ǰ������ֵ
//������
//row��ָ������
//column��ָ������
//����ֵ����
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

//��ָ��������д������
//������
//row��ָ������
//column��ָ������
//data��д�������
//����ֵ����
void Nokia5110::Draw(unsigned char row, unsigned char column,
		unsigned char data)
{
	SetColumn(column);
	SetRow(row);
	sendData(data);

	UpdateCurrentRowAndColumn(row, column);

}

