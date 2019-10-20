#include "main.h"


/**********************************************************************
函数功能：主函数
入口参数：无
返回参数：无
***********************************************************************/
int main(void)
{	
	u8 rx_sta=0;
	
	delay_init();				//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(UAR1,115200);	 		//串口初始化为115200	
	uart_init(UAR2,1382400);	 	//串口初始化为1382400	
	LED_Init();					//LED端口初始化
	KEY_Init();					//按键初始化 	
//	BluetoothIO_init();			//蓝牙端口初始化
	LCD_Init();					//LCD初始化
	mallco_dev.init(SRAMIN);	//初始化内部内存池
	RX_buf = mymalloc(SRAMIN,RxSize);
	if(RX_buf==NULL)
	{
		while(1)
		{
			LED1 = !LED1;
			delay_ms(200);
		}
	}
	else
	{
		printf("RX_buf OK!\r\n");
	}
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		delay_ms(100);
		LED1 = !LED1;
	}
	LED1 = 1;
	NRF24L01_RX_Mode();			//初始化无线到接收模式	
	
	MYDMA_Config(DMA1_Channel6,(u32)&USART2->DR,(u32)RX_buf,RxSize);
	MYDMA_Enable();	
//	MYDMA_SRAMLCD_Init((u32)RX_buf);//RAM写到LCD
	
	TIM1_IntInit(99,127);		//定时器中断
	
//	while(Bluetooth_P)			//等待蓝牙连接
//	{
//		delay_ms(1);
//	}
//	delay_ms(1200);
	
	USART_SendData(USART2,0xf0);		//发送0xf0
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));	
	
	while(1)
	{	
		if(!NRF24L01_RxPacket(&rx_sta))		//判断帧起始标志
		{		
			if(rx_sta==1)
			{
				LCD_SetCursor(0, 0);		//设置坐标
				LCD_WriteRAM_Prepare();		//开始写入GRAM
				rx_sta = 0;
			}
		}
	}
}

