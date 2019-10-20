#ifndef __ADC7606_H
#define __ADC7606_H
#include <stdio.h>
#include "sys.h"
#include "delay.h"
#define SOFT_SPI		/* 定义此行表示使用GPIO模拟SPI接口 */

#define BUSY_IS_LOW()				(GPIO_ReadInputDataBit(PORT_BUSY, PIN_BUSY) == Bit_RESET)
#define CH_NUM	1	/* 1通道 */

/***************************
*
* RD   -  SCK     -  PA6
* RST  -  RESET   -  PA3
* BUSY -  BUSY    -  PA5
* CS   -  CSN     -  PA2
* DB7  -  MISO    -  PA1
* RAGE -  RANGE   -  PA7
* CVAB -  CONVST  -  PA4
* 
****************************/


/* 定义SPI总线的 GPIO端口 */
#define RCC_SCK 	RCC_APB2Periph_GPIOA
#define PORT_SCK	GPIOA
#define PIN_SCK		GPIO_Pin_6

#define RCC_MISO 	RCC_APB2Periph_GPIOA
#define PORT_MISO	GPIOA
#define PIN_MISO	GPIO_Pin_1

#define RCC_MOSI 	RCC_APB2Periph_GPIOD
#define PORT_MOSI	GPIOD
#define PIN_MOSI	GPIO_Pin_2
/* SPI总线的SCK、MOSI、MISO  */
#define SCK_0()		PORT_SCK->BRR = PIN_SCK//端口清除操作
#define SCK_1()		PORT_SCK->BSRR = PIN_SCK//端口设置操作

#define MOSI_0()	PORT_MOSI->BRR = PIN_MOSI
#define MOSI_1()	PORT_MOSI->BSRR = PIN_MOSI

#define MISO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_MISO, PIN_MISO) == Bit_SET)



/* CSN片选 */
#define RCC_CS 		RCC_APB2Periph_GPIOA
#define PORT_CS		GPIOA
#define PIN_CS		GPIO_Pin_2

/* RESET */
#define RCC_RESET 	RCC_APB2Periph_GPIOA
#define PORT_RESET	GPIOA
#define PIN_RESET	GPIO_Pin_3

/* RANGE */		
#define RCC_RANGE 	RCC_APB2Periph_GPIOA
#define PORT_RANGE 	GPIOA
#define PIN_RANGE 	GPIO_Pin_7

/* CONVST */		
#define RCC_CONVST 	RCC_APB2Periph_GPIOA
#define PORT_CONVST	GPIOA
#define PIN_CONVST 	GPIO_Pin_4

/* BUSY */
#define RCC_BUSY 	RCC_APB2Periph_GPIOA
#define PORT_BUSY 	GPIOA
#define PIN_BUSY 	GPIO_Pin_5

/* 设置量程 */
#define AD_RANGE_5V()					GPIO_ResetBits(PORT_RANGE, PIN_RANGE)
#define AD_RANGE_10V()					GPIO_SetBits(PORT_RANGE, PIN_RANGE)	//置1

/* 片选 */
#define AD_CS_0()						GPIO_ResetBits(PORT_CS, PIN_CS)
#define AD_CS_1()						GPIO_SetBits(PORT_CS, PIN_CS)

/* 复位引脚 */
#define AD_RESET_LOW()					GPIO_ResetBits(PORT_RESET, PIN_RESET)
#define AD_RESET_HIGH()					GPIO_SetBits(PORT_RESET, PIN_RESET)

/* 起始信号 */
#define	AD_CONVST_LOW()					GPIO_ResetBits(PORT_CONVST, PIN_CONVST)
#define	AD_CONVST_HIGH()				GPIO_SetBits(PORT_CONVST, PIN_CONVST)

extern  int16_t s_volt[8];
void AD7606_ConfigGPIO( void );
void bsp_InitSPIBus(void);
void AD7606_SetInputRange(uint8_t _ucRange);
void AD7606_Reset(void);
void bsp_spi_InitAD7606(void);
uint8_t bsp_spiRead1(void);
void AD7606_Scan(void);
void AD7606_StartConv(void);
void bsp_spiDelay(void);
void AD7606_Disp(void);
int16_t AD7606_ReadAdc(uint8_t _ch);
void AD7606_Count(void);
void Demo_spi_AD7606( void );
void AD7606_Init(void);  //  ad7606模块初始化
#endif

