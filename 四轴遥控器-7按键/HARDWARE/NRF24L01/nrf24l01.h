#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <reg52.h>

/************************************用户配置************************************/
#define ADDR_WIDTH		5		//发送,接收地址宽度
#define DATA_WIDTH		3		//发送,接收数据的长度(字节,最大为32)
/********************************************************************************/

/************************************外部调用************************************/
#define CHECK_SUCCEED	0		//NRF20L01存在
#define	CHECK_ERROR		1		//NRF20L01不存在
#define TX_SUCCEED		0		//发送成功
#define TX_ERROR		1		//发送失败
#define RX_SUCCEED		0		//接收成功
#define RX_ERROR		1		//接收失败
/********************************************************************************/

sbit NRF24L01_CE   = P0^2;		//芯片模式控制线
sbit NRF24L01_SCK  = P0^3;		//芯片控制时钟线(SPI时钟)
sbit NRF24L01_MISO = P0^4;		//芯片控制数据线(主入从出)
sbit NRF24L01_IRQ  = P0^5;		//中断信号			
sbit NRF24L01_MOSI = P0^6;		//芯片控制数据线(主出从入)
sbit NRF24L01_CSN  = P0^7;		//芯片片选线,CSN为低电平芯片工作

/**********************************************************************
*函数名称:	Nrf24l01Init
*函数功能:	初始化NRF24L01
*函数参数: 	无
*返 回 值:	无
***********************************************************************/
void Nrf24l01Init(void);

/**********************************************************************
*函数名称:	CheckNrf24l01
*函数功能:	检查NRF24L01模块是否存在
*函数参数: 	无
*返 回 值:	CHECK_SUCCEED:	NRF24L01存在
*			CHECK_ERROR:	NRF24L01不存在
***********************************************************************/
unsigned char CheckNrf24l01(void);

/**********************************************************************
*函数名称:	Nrf24l01SetTxMode
*函数功能:	设置NRF24L01为发送模式
*函数参数: 	无
*返 回 值:	无
***********************************************************************/
void Nrf24l01SetTXMode(void);

/**********************************************************************
*函数名称:	Nrf24l01SetRxMode
*函数功能:	设置NRF24L01为接收模式
*函数参数: 	无
*返 回 值:	无
***********************************************************************/
void Nrf24l01SetRXMode(void);

/**********************************************************************
*函数名称:	Nrf24l01TxPacket
*函数功能:	启动NRF24L01发送数据
*函数参数: 	txBuf:		指向要发送的数据的首地址
*返 回 值:	TX_SUCCEED:	发送完成
*			TX_ERROR:	发送失败
***********************************************************************/
unsigned char Nrf24l01TXPacket(unsigned char *txBuf);

/**********************************************************************
*函数名称:	Nrf24l01RXPacket
*函数功能:	启动NRF24L01接收数据
*函数参数: 	rxBuf:	指向要接收的数据的首地址
*返 回 值:	RX_SUCCEED:	接收完成
*			RX_ERROR:	接收失败
***********************************************************************/
unsigned char Nrf24l01RXPacket(unsigned char *rxBuf);



void Nrf24l01DelayUs(unsigned int x);
unsigned char SpiReadAndWrite(unsigned char dat);
unsigned char Nrf24l01WriteReg(const unsigned char reg, const unsigned char dat);
unsigned char Nrf24l01ReadReg(const unsigned char reg);
unsigned char Nrf24l01WriteBuf(const unsigned char reg, const unsigned char *pBuf, const unsigned char len);
unsigned char Nrf24l01ReadBuf(const unsigned char reg, unsigned char *pBuf, const unsigned char len);



#endif
