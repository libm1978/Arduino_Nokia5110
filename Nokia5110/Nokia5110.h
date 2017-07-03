/*
 * Nokia5110.h
 *
 *  Created on: 2017��6��15��
 *      Author: li
 */

#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include <Arduino.h>
//#include <SPI.h>

//����Nokia5110�Ļ״̬
typedef enum
{
   POWER_ON,//����ģʽ
   POWER_DOWN//����ģʽ
}NOKIA_POWER_STAT;

//����ָ��Nokia5110��ʾģʽ
typedef enum
{
	DISPLAY_SPACE,
   DISPLAY_ALL_SEGMENT,
   DISPLAY_NORMAL,
   DISPLAY_INVERT
} NOKIA_DISPLAY_MODE;

//����ָ��Nokia5110��ʾ����ɨ�跽��
typedef enum
{
  HORIZONTAL,
  VIRTICAL
}NOKIA_DISPLAY_DIRECTION;

//����ָ��Nokia5110��ָ�
typedef enum
{
  BASE_INSTRUCT,
  EXTENT_INSTRUCT
}NOKIA_INSTRUCT_SET;


//���ڱ�ʾSPI�Ļ���ͨѶ���ã��ο�Arduino��SPI��
typedef struct
{
  unsigned char speed;//���ݴ����ٶ�
  unsigned char bitOrder;//���ݴ���˳��
  unsigned char dataMode;//���ݴ���ģʽ
}Nokia_SPI_Settings;


class Nokia5110
{
private:
	//���ڶ���Nokia5110�Ŀ��ƼĴ��������ݻ���
	typedef struct
  {
		Nokia_SPI_Settings spiSettings;//SPIͨѶ�Ļ�������
		unsigned char enableFunctions;//Nokia5110�Ĺ��ܿ��ƼĴ���
		unsigned char displayMode;//Nokia5110����ʾģʽ�Ĵ���
		unsigned char column;//Nokia5110��ǰ����
		unsigned char row;//Nokia5110��ǰ����
  } Nokia5110_ControlPad;

  	 //Nokia5110�������������
	static const unsigned char MAX_COLUMN = 84;
	//Nokia5110�������������
	static const unsigned char MAX_ROW = 6;


	//Nokia5110�Ŀ��ƼĴ��������ڻ���
	Nokia5110_ControlPad currentControlPad;

	void controlPad_Init();
	void UpdateCurrentRowAndColumn(unsigned char row, unsigned char column);

protected:
	//Reset����
	unsigned char m_reset;
	//���������л�����
	unsigned char m_dc;
	//����LED��������
	unsigned char m_led;
	//SPI�ӻ�ʹ������
	unsigned char m_ss;
	//SPIͨѶʱ������
	unsigned char m_sclk;
	//SPIͨѶ���ݴ�������
	unsigned char m_mosi;

	virtual void pin_Init();
	virtual void spi_Init();
	virtual void sendCommand(unsigned char cmd);
	virtual void sendData(unsigned char data);

public:
	Nokia5110(unsigned char resetPin,unsigned char dcPin,unsigned char sclkPin,unsigned char mosiPin,unsigned char ssPin,unsigned char ledPin);
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
