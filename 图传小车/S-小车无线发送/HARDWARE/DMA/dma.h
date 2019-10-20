#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"


/*数据接收BUF大小*/
#define RxSize 640*2							    					    

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//配置DMA1_CHx

//开启DMA传输
void MYDMA_Enable(void);//使能DMA

//关闭DMA传输
void MYDMA_Disable(void);
		   
#endif




