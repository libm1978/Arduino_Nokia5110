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


typedef enum
{
   ON,
   OFF
}NOKIA_SWITCH_STAT;

typedef enum {
	NOT, AND, OR, XOR
} NOKIA_DISPLAY_WRITE_MODE;

typedef enum
{
	DISPLAY_SPACE,
   DISPLAY_ALL_SEGMENT,
   DISPLAY_NORMAL,
   DISPLAY_INVERT
} NOKIA_DISPLAY_MODE;


typedef enum
{
  HORIZONTAL,
  VIRTICAL
}NOKIA_DISPLAY_DIRECTION;


typedef enum
{
  BASE_INSTRUCT,
  EXTENT_INSTRUCT
}NOKIA_INSTRUCT_SET;

typedef struct
{
  unsigned char speed;
  unsigned char bitOrder;
  unsigned char dataMode;
}Nokia_SPI_Settings;


class Nokia5110
{
private:

  typedef struct
  {
    Nokia_SPI_Settings spiSettings;
    unsigned char enableFunctions;
    unsigned char displayMode;
    unsigned char column;
    unsigned char row;
  } Nokia5110_ControlPad;

	static const unsigned char MAX_COLUMN = 84;
	static const unsigned char MAX_ROW = 6;

	unsigned char m_reset;
	unsigned char m_dc;
	unsigned char m_led;
	unsigned char m_ss;
	unsigned char m_sclk;
	unsigned char m_mosi;
	unsigned char * m_buffer;
	unsigned char *m_pCurrentOfBuffer;

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
