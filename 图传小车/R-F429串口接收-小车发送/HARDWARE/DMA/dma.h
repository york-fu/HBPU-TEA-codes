#ifndef __DMA_H
#define __DMA_H
#include "sys.h"


//DMA配置
//mem0addr:存储器地址0  将要存储摄像头数据的内存地址(也可以是外设地址)
//mem1addr:存储器地址1  当只使用mem0addr的时候,该值必须为0
//memsize:从来源到目的地传输的数据长度
void DMA1Stream5_4_init(u32 mem0addr,u32 mem1addr,u16 memsize);

//USART2_Start,启动传输
void USART2_Start(void);

//USART2_Stop,关闭传输
void USART2_Stop(void);
	
#endif
