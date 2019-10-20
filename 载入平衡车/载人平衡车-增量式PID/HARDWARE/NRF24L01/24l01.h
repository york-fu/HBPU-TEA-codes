#ifndef __24L01_H
#define __24L01_H
#include "sys.h"



/*-- 用户自定义IO --*/
#define RCC_CE      			RCC_APB2Periph_GPIOB
#define GPIO_CE_PORT  			GPIOB    
#define GPIO_CE_PIN       		GPIO_Pin_7

#define RCC_CSN         		RCC_APB2Periph_GPIOB
#define GPIO_CSN_PORT   		GPIOB    
#define GPIO_CSN_PIN          	GPIO_Pin_8

#define RCC_IRQ             	RCC_APB2Periph_GPIOB
#define GPIO_IRQ_PORT       	GPIOB  
#define GPIO_IRQ_PIN      		GPIO_Pin_4

//24L01操作线
#define NRF24L01_CE   PBout(7) //24L01片选信号
#define NRF24L01_CSN  PBout(8) //SPI片选信号	   
#define NRF24L01_IRQ  PBin(4)  //IRQ主机数据输入
//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5   	//5字节的地址宽度
#define RX_ADR_WIDTH    5   	//5字节的地址宽度
#define TX_PLOAD_WIDTH  3  		//3字节的用户数据宽度
#define RX_PLOAD_WIDTH  3  		//3字节的用户数据宽度
									   	   

void NRF24L01_Init(void);//初始化
void NRF24L01_RX_Mode(void);//配置为接收模式
void NRF24L01_TX_Mode(void);//配置为发送模式
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 u8s);//写数据区
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 u8s);//读数据区		  
u8 NRF24L01_Read_Reg(u8 reg);			//读寄存器
u8 NRF24L01_Write_Reg(u8 reg, u8 value);//写寄存器
u8 NRF24L01_Check(void);//检查24L01是否存在
u8 NRF24L01_TxPacket(u8 *txbuf);//发送一个包的数据
u8 NRF24L01_RxPacket(u8 *rxbuf);//接收一个包的数据

void Remotecontrol(void);

extern u8 ARMED;
extern s8 Turn_pwm;


#endif


