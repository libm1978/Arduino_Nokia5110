/*
 * SPITransfer.h
 *
 *  Created on: 2017年7月4日
 *      Author: li
 */

#ifndef NOKIA5110_SPITRANSFER_H_
#define NOKIA5110_SPITRANSFER_H_

//功能：为SPI接口提供抽象
class SPITransfer
{
private:
	void (*m_init)(void);
	unsigned char (*m_transfer)(unsigned char data);
	void (*m_begin)(void);
	void(*m_end)(void);
public:
	SPITransfer(void(*init)(void)=nullptr,unsigned char (*trans)(unsigned char)=nullptr,
			void (*begin)(void)=nullptr,void (*end)(void)=nullptr)
{
	m_init=init;
	m_transfer=trans;
	m_begin=begin;
	m_end=end;
}

	inline void Init()
	{
		if(m_init!=nullptr)
			(*m_init)();
	}

	inline void Begin()
	{
		if(m_begin!=nullptr)
			(*m_begin)();
	}

	inline unsigned char Transfer(unsigned char data)
	{
		unsigned char ret=0;
		if(m_transfer!=nullptr)
			ret=(*m_transfer)(data);
		return ret;
	}

	inline void End()
	{
		if(m_end!=nullptr)
			(*m_end)();
	}

	~SPITransfer()
	{

	}
};




#endif /* NOKIA5110_SPITRANSFER_H_ */
