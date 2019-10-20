#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "EasyTracer.h"
#include "key.h"
#include "24l01.h"
#include "dma.h"
#include "control.h"


int main(void)
{	
	u8 rx_sta=0;
	
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART1
    MyUSART2_Init(1382400);         //初始化USART2
	LED_Init();                     //初始化LED 
	KEY_Init();						//初始化KEY
	LCD_Init();	                    //初始化LCD
	
	NRF24L01_Init();	//无线初始化
	while(NRF24L01_Check())
	{
		LED1=!LED1;
		delay_ms(200);
	}
	NRF24L01_RX_Mode();	//设置无线为接收模式 
	
//	DMA1Stream5_4_init((u32)USART2->DR,(u32)RX_buf,1);
	TIM3_Init(19999,89);		//定时器初始化
	
	printf("init ok!\r\n");
	
	USART2->DR = 0xf0;
	while(!(USART2->SR & (1<<6))){}	//等待发送完成
	
    while(1)
    {	
//		if(!NRF24L01_RxPacket(&rx_sta))		//判断帧起始标志
//		{		
//			if(rx_sta==1)
//			{
//				LCD_SetCursor(0, 0);		//设置坐标
//				LCD_WriteRAM_Prepare();		//开始写入GRAM
//				rx_sta = 0;
//			}
//		}
//		printf("%d\r\n",RX_buf[0]);
//		delay_ms(500);	
	}
}
