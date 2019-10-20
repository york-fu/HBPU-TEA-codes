#include "dma.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"


DMA_InitTypeDef DMA_InitStructure;
NVIC_InitTypeDef NVIC_Initstur;

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 

//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			//工作在循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMA通道 x拥有高优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);		//根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	NVIC_Initstur.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Initstur.NVIC_IRQChannelSubPriority = 1;
	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstur);
	
	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);	//开启DMA传输完成中断
	
	DMA_Cmd(DMA_CHx,DISABLE);	//失能DMA传输
}


//开启DMA传输
void MYDMA_Enable()
{ 	
	DMA_Cmd(DMA1_Channel6,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
}

//关闭DMA传输
void MYDMA_Disable()
{ 	
	USART_DMACmd(USART2,USART_DMAReq_Rx,DISABLE);
	DMA_Cmd(DMA1_Channel6,DISABLE);
}	  


/*接收标记*/
u8 RX_sta=0;

/*数据接收BUF*/
u8* RX_buf;

/*DMA1通道6中断服务函数*/
void DMA1_Channel6_IRQHandler(void)
{
	static u16 i,PixCount=0,Line=1;
	
	if(DMA_GetITStatus(DMA1_IT_TC6) != RESET)	//DMA通道传输完成中断
	{
		DMA_ClearITPendingBit(DMA1_IT_TC6);		//清除中断标志
	
		USART_SendData(USART2,0xf0);		//发送0xf0
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
		
		for(i=0;i<RxSize;i+=2)
		{
			LCD->LCD_RAM = (RX_buf[i]<<8)+RX_buf[i+1];
//			PixCount++;
//			if(PixCount==OvLcdHigh)
//			{
//				PixCount=0;
//				LCD_SetCursor(0, Line);		//设置坐标
//				LCD_WriteRAM_Prepare();		//开始写入GRAM
//				Line++;
//				if(Line==OvLcdWide-1) Line=0;			
//			}
		}	
		
//		MYDMA_SRAMLCD_Enable();
	}
	
}



/***********************************************************************/


//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void DMAChannel7_Config(u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA1_Channel7);   //将DMA的通道1寄存器重设为缺省值

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设发送到内存
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//外设地址寄存器递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;						//内存地址寄存器不变
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;			//工作在单次模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;			//DMA通道设置为内存到内存传输
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);			//初始化DMA的通道
	
//	NVIC_Initstur.NVIC_IRQChannel = DMA1_Channel6_IRQn;
//	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_Initstur);
	
//	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);	//开启DMA传输完成中断
	
	DMA_Cmd(DMA1_Channel7,DISABLE);	//失能DMA传输
}


//开启DMA传输
void DMAChannel7_Enable()
{ 	
	DMA_Cmd(DMA1_Channel7,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
}

//关闭DMA传输
void DMAChannel7_Disable()
{ 	
	USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE);
	DMA_Cmd(DMA1_Channel7,DISABLE);
}	  





/***********************************************************************/

//SRAM --> LCD_RAM dma配置
//caddr：数据源地址
//16位,从SRAM传输到LCD_RAM. 
void MYDMA_SRAMLCD_Init(u32 caddr)
{  
	DMA_InitTypeDef DMA_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA2_Channel5);   //将DMA的通道1寄存器重设为缺省值

	DMA_InitStructure.DMA_PeripheralBaseAddr = caddr;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&LCD->LCD_RAM;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读
	DMA_InitStructure.DMA_BufferSize = 0;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  //外设地址寄存器增量
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址寄存器非增量
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;  //DMA通道x设置为内存到内存传输
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
	
	NVIC_Initstur.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Initstur.NVIC_IRQChannelSubPriority = 2;
	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstur);
	
	DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);	//开启DMA传输完成中断
}  

//开启一次SRAM到LCD的DMA的传输
void MYDMA_SRAMLCD_Enable(void)
{
	DMA_Cmd(DMA2_Channel5, DISABLE ); //关闭DMA传输        
 	DMA_SetCurrDataCounter(DMA2_Channel5,RxSize/2);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA2_Channel5, ENABLE);  //使能USART1 TX DMA1 所指示的通道 	
} 




/*DMA中断服务函数*/
void DMA2_Channel4_5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC5) != RESET)	//DMA通道传输完成中断
	{
		DMA_ClearITPendingBit(DMA2_IT_TC5);		//清除中断标志
		
		USART_SendData(USART2,0x66);		//发送0x66
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	}
	
}









