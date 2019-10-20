#ifndef __SPI_H
#define __SPI_H
#include "sys.h"


/*模拟SPI端口输出输入定义*/
#define SCK_H PBout(5)=1
#define SCK_L PBout(5)=0

#define MOSI_H PBout(6)=1
#define MOSI_L PBout(6)=0

#define MISO_Read PBin(11)

						  	    													  
void SIMUL_SPI_Init(void);		 //初始化SPI口 
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
#endif

