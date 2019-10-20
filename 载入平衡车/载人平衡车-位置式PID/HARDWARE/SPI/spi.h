#ifndef __MYSPI_H
#define __MYSPI_H
#include "sys.h"

/*-- 用户自定义IO --*/
#define RCC_SCLK      			RCC_APB2Periph_GPIOB
#define GPIO_SCLK_PORT  		GPIOB    
#define GPIO_SCLK_PIN       	GPIO_Pin_5

#define RCC_MOSI         		RCC_APB2Periph_GPIOB
#define GPIO_MOSI_PORT   		GPIOB    
#define GPIO_MOSI_PIN          	GPIO_Pin_6

#define RCC_MISO             	RCC_APB2Periph_GPIOB
#define GPIO_MISO_PORT       	GPIOB  
#define GPIO_MISO_PIN      		GPIO_Pin_3

#define SCK_H					GPIO_SetBits(GPIO_SCLK_PORT,GPIO_SCLK_PIN)
#define SCK_L					GPIO_ResetBits(GPIO_SCLK_PORT,GPIO_SCLK_PIN)

#define MOSI_H					GPIO_SetBits(GPIO_MOSI_PORT,GPIO_MOSI_PIN)
#define MOSI_L					GPIO_ResetBits(GPIO_MOSI_PORT,GPIO_MOSI_PIN)

#define MISO_Read 				GPIO_ReadInputDataBit(GPIO_MISO_PORT,GPIO_MISO_PIN)


						  	    													  
void SIMUL_SPI_Init(void);		 //初始化SPI口 
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
#endif

