/*
 * Nokia5110.h
 *
 *  Created on: 2017��6��15��
 *      Author: li
 */

#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include <Arduino.h>
#include <SPI.h>

//����Nokia5110�Ļ״̬
typedef enum
{
   ON,//����ģʽ
   OFF//����ģʽ
}NOKIA_SWITCH_STAT;

//����ָ����Nokia5110��ʾ���ݻ�����д������ʱ�Ĳ���ģʽ
typedef enum {
	NOT,
	AND,
	OR,
	XOR
} NOKIA_DISPLAY_WRITE_MODE;

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





class Nokia5110
{
private:
	//���ڱ�ʾSPI�Ļ���ͨѶ���ã��ο�Arduino��SPI��
	typedef struct
	{
	  unsigned char speed;//���ݴ����ٶ�
	  unsigned char bitOrder;//���ݴ���˳��
	  unsigned char dataMode;//���ݴ���ģʽ
	}Nokia_SPI_Settings;

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
	//Nokia5110��ʾ����Ķ����ڻ�����
	unsigned char * m_buffer;
	//Nokia5110��ʾ�����ڵ�ǰ�ֽ�ָ��
	unsigned char *m_pCurrentOfBuffer;

	//Nokia5110�Ŀ��ƼĴ��������ڻ���
	Nokia5110_ControlPad currentControlPad;

	void pin_Init();
	void spi_Init();
	void controlPad_Init();
	void sendCommand(unsigned char cmd);
	void sendData(unsigned char data);
	void setColumn (unsigned char x);
	void setRow (unsigned char y);
	void draw(unsigned char row, unsigned char column, unsigned char data);
public:
	Nokia5110(unsigned char resetPin,unsigned char dcPin,unsigned char sclkPin,unsigned char mosiPin,unsigned char ssPin,unsigned char ledPin);
	void OnLED();
	void OffLED();
	void Reset();
	void SetAvaliable(NOKIA_SWITCH_STAT s);
	NOKIA_SWITCH_STAT GetAvaliable() const;
	void SetDisplayMode(NOKIA_DISPLAY_MODE m);
	NOKIA_DISPLAY_MODE GetDisplayMode() const;
	void SetDisplayDirection(NOKIA_DISPLAY_DIRECTION d);
	NOKIA_DISPLAY_DIRECTION GetDisplayDirection() const;
	void SetInstructSet(NOKIA_INSTRUCT_SET is);
	NOKIA_INSTRUCT_SET GetInstructSet() const;
	void drawDot(unsigned char x, unsigned char y,NOKIA_DISPLAY_WRITE_MODE mode);
	unsigned char * GetCurrentOfByte () const;
	void Refresh(unsigned char x = 0, unsigned char y = 0, unsigned char width =
			MAX_COLUMN, unsigned char high = MAX_ROW);
  ~Nokia5110 ();
};




#endif /* NOKIA5110_H_ */
