#include "dma.h"
#include "lcd.h"
#include "usart.h"
#include "display.h"


DMA_InitTypeDef DMA_InitStructure;
NVIC_InitTypeDef NVIC_Initstur;

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	   

//DMA1通道配置
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void DMA1_Channel3_Config(u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA1_Channel3);   //将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;					//内存地址寄存器不变
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			//工作在循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMA通道 x拥有高优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);		//根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
//	NVIC_Initstur.NVIC_IRQChannel = DMA1_Channel3_IRQn;
//	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_Initstur);
	
//	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);	//开启DMA传输完成中断
	
	DMA_Cmd(DMA1_Channel3,ENABLE);	//使能DMA传输
}


//开启DMA传输
void MYDMA_Enable()
{ 	
	DMA_Cmd(DMA1_Channel3,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
}

//关闭DMA传输
void MYDMA_Disable()
{ 	
	USART_DMACmd(USART3,USART_DMAReq_Rx,DISABLE);
	DMA_Cmd(DMA1_Channel3,DISABLE);
}	  



/*DMA1通道3中断服务函数*/
void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3) != RESET)	//DMA通道传输完成中断
	{
		DMA_ClearITPendingBit(DMA1_IT_TC3);		//清除中断标志
		
	}
	
}



/*********************************************************************************/

//DMA1的各通道配置
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void DMA1_Channel2_Config(u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA1_Channel2);   //将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;			//工作在单次模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);		//根据DMA_InitStruct中指定的参数初始化DMA的通道
}


//开启DMA传输
void DMA1_Channel2_Enable()
{ 	
	DMA_Cmd(DMA1_Channel2, DISABLE);  	//关闭DMA1所指示的通道      
 	DMA_SetCurrDataCounter(DMA1_Channel2,buf_size);//DMA通道的DMA缓存的大小
	DMA_Cmd(DMA1_Channel2,ENABLE);
}

//关闭DMA传输
void DMA1_Channel2_Disable()
{ 	
	USART_DMACmd(USART3,USART_DMAReq_Tx,DISABLE);
	DMA_Cmd(DMA1_Channel2,DISABLE);
}	  





















