/*
 * GPIO.h
 *
 *  Created on: 2017年7月8日
 *      Author: libm
 */

#ifndef GPIO_H_
#define GPIO_H_

class GPIO
{
public:
	virtual void Init();
	virtual void Set();
	virtual unsigned char Get();
	virtual ~GPIO();
};



#endif /* GPIO_H_ */
