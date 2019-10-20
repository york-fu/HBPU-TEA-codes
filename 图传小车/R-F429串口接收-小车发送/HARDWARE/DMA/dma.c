#include "dma.h"
#include "lcd.h"
#include "usart.h"


/*******************************************************************************/


DMA_HandleTypeDef UART2RxDMA_Handler;


//DMA配置
//mem0addr:存储器地址0  将要存储摄像头数据的内存地址(也可以是外设地址)
//mem1addr:存储器地址1  当只使用mem0addr的时候,该值必须为0
//memsize:从来源到目的地传输的数据长度
void DMA1Stream5_4_init(u32 mem0addr,u32 mem1addr,u16 memsize)
{ 
    __HAL_RCC_DMA1_CLK_ENABLE();                                    //使能DMA1时钟
    __HAL_LINKDMA(&UART2_Handle,hdmarx,UART2RxDMA_Handler);     //将DMA与USART2联系起来
	
    UART2RxDMA_Handler.Instance=DMA1_Stream5;                          //DMA1数据流5                    
    UART2RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                     //通道4
    UART2RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;            //外设到存储器
    UART2RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                //外设非增量模式
    UART2RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                    //存储器增量模式
    UART2RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;   //外设数据长度:8位
    UART2RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;      //存储器数据长度:8/16/32位
    UART2RxDMA_Handler.Init.Mode=DMA_CIRCULAR;                         //使用循环模式 
    UART2RxDMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;                //高优先级
    UART2RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;              //使能FIFO
    UART2RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_HALFFULL; //使用1/2的FIFO 
    UART2RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                //存储器突发传输
    UART2RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;             //外设突发单次传输 
    HAL_DMA_DeInit(&UART2RxDMA_Handler);                               //先清除以前的设置
    HAL_DMA_Init(&UART2RxDMA_Handler);	                               //初始化DMA
    
    //在开启DMA之前先使用__HAL_UNLOCK()解锁一次DMA,因为HAL_DMA_Statrt()HAL_DMAEx_MultiBufferStart()
    //这两个函数一开始要先使用__HAL_LOCK()锁定DMA,而函数__HAL_LOCK()会判断当前的DMA状态是否为锁定状态，如果是
    //锁定状态的话就直接返回HAL_BUSY，这样会导致函数HAL_DMA_Statrt()和HAL_DMAEx_MultiBufferStart()后续的DMA配置
    //程序直接被跳过！DMA也就不能正常工作，为了避免这种现象，所以在启动DMA之前先调用__HAL_UNLOC()先解锁一次DMA。
    __HAL_UNLOCK(&UART2RxDMA_Handler);
	
    if(mem1addr==0)    //开启DMA，不使用双缓冲
    {
        HAL_DMA_Start(&UART2RxDMA_Handler,(u32)&USART2->DR,mem0addr,memsize);
    }
    else                //使用双缓冲
    {
        HAL_DMAEx_MultiBufferStart(&UART2RxDMA_Handler,(u32)&USART2->DR,mem0addr,mem1addr,memsize);//开启双缓冲
        __HAL_DMA_ENABLE_IT(&UART2RxDMA_Handler,DMA_IT_TC);		//开启传输完成中断
        HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,0,0);        	//DMA中断优先级
        HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    }
	
    __HAL_DMA_ENABLE(&UART2RxDMA_Handler); //使能DMA
}



//USART2,启动传输
void USART2_Start(void)
{  
    __HAL_DMA_ENABLE(&UART2RxDMA_Handler); //使能DMA
	
//	HAL_DMA_Start(UART2_Handle->hdmarx, (u32)RX_buf, (uint32_t)&UART2_Handle->Instance->DR, RxSize);//开启DMA传输
    
//	UART2_Handle->Instance->CR3 |= USART_CR3_DMAR;//使能串口DMA
}

//USART2,关闭传输
void USART2_Stop(void)
{ 
    __HAL_DMA_DISABLE(&UART2RxDMA_Handler);//关闭DMA
} 


//DMA1数据流5中断服务函数
void DMA1_Stream5_IRQHandler(void)
{
    if(__HAL_DMA_GET_FLAG(&UART2RxDMA_Handler,DMA_FLAG_TCIF1_5)!=RESET)//DMA传输完成
    {
        __HAL_DMA_CLEAR_FLAG(&UART2RxDMA_Handler,DMA_FLAG_TCIF1_5);//清除DMA传输完成中断标志位
		printf("DMA IT OK!\r\n");
    } 
}




