#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"						    					    

/*DMA1通道3配置*/
void DMA1_Channel3_Config(u32 cpar,u32 cmar,u16 cndtr);//配置DMA1_CHx
//开启DMA传输
void MYDMA_Enable(void);//使能DMA
//关闭DMA传输
void MYDMA_Disable(void);


/*DMA1通道2配置*/
void DMA1_Channel2_Config(u32 cpar,u32 cmar,u16 cndtr);
//开启DMA传输
void DMA1_Channel2_Enable(void);
//关闭DMA传输
void DMA1_Channel2_Disable(void);
		   
#endif




