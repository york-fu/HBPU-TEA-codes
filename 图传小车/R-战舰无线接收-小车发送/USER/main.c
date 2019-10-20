#include "main.h"



/*无线接收BUF*/
u8 Rxbuf[32]={0};

/**********************************************************************
函数功能：主函数
入口参数：无
返回参数：无
***********************************************************************/
int main(void)
{
	u8 i,key=0;
	u16 x=0;
	
	delay_init();				//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(UAR1,115200);	 	//串口初始化为115200
	MyUSART2_init(1382400);	
	LED_Init();					//LED端口初始化
	KEY_Init();					//按键初始化
	LCD_Init();					//LCD初始化
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		delay_ms(100);
		LED1 = !LED1;
	}
	LED1 = 1;
	NRF24L01_RX_Mode();			//初始化无线到接收模式	

//	MYDMA_Config(DMA1_Channel4,(u32)&SPI2->DR,(u32)Rxbuf,32);
//	EXTI6_Init();
	
	USART_DMAConfig(DMA1_Channel6,(u32)&USART2->DR,(u32)&Rx_sta,1);
	
	TIM1_IntInit(199,71);		//初始化定时器中断,20ms
	
	USART_SendData(USART2,0xf0);		//发送0xf0
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	
	while(1)
	{				
		if(!NRF24L01_RxPacket(Rxbuf))
		{		
			USART_SendData(USART2,0xf0);		//发送0xf0
			while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
			
			for(i=0;i<32;i+=2)
			{
				LCD->LCD_RAM=(Rxbuf[i]<<8)+Rxbuf[i+1];	
			}
			
//			x++;
//			if(x==4800)
//			{
//				USART_SendData(USART2,0xf0);		//发送0xf0
//				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
//				x = 0;
//			}
		}
		
//		if(Rx_sta==0xff)
//		{			
//			USART_SendData(USART2,0xf0);		//发送0xf0
//			while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
//			Rx_sta=0;
//			printf("ff\r\n");
//			LCD_SetCursor(0,0);	//设置光标位置 
//			LCD_WriteRAM_Prepare();     //开始写入GRAM	
//		}

//		printf("%x\t",Rx_sta);
//		Rx_sta = 0;
//		delay_ms(500);
	}
}

