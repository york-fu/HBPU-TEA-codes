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


extern u8 U2Rx_sta;	//串口2接收标记


/*数据接收BUF*/
u8 Rxbuf[32];


//// .
//RESULT Resu;           
//TARGET_CONDI Condition={120,180,50,240,20,190,50,50,100,100};


int main(void)
{
	u16 i=0;		
	
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART
    MyUSART2_Init(115200);          //初始化USART
	LED_Init();                     //初始化LED 
	KEY_Init();						//初始化KEY
	LCD_Init();	                    //初始化LCD 
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		LED1 = !LED1;
		delay_ms(200);
	}
	NRF24L01_RX_Mode();
	LED1 = 1;
		
    while(1)
    {	
		
		if(!NRF24L01_RxPacket(Rxbuf))
		{		
			for(i=0;i<32;i+=2)
			{
				LCD->LCD_RAM=(Rxbuf[i]<<8)+Rxbuf[i+1];	
			}
		}
		
		


		
		// 搜索小车
//		if(Trace(&Condition,&Resu))
//		{
//			LCD_DrawRectangle(Resu.x-Resu.w/2+2,Resu.y-Resu.h/2+2,Resu.x+Resu.w/2-2,Resu.y+Resu.h/2-2);
//			LCD_DrawRectangle(Resu.x-2,Resu.y-2,Resu.x+2,Resu.y+2);
//		}
//		
//		// 显示坐标数据
//		LCD_ShowxNum(280, 190, Resu.x, 3, 16, 0);
//		LCD_ShowxNum(280, 210, Resu.y, 3, 16, 0);
//		
//		// 发送数据
//		uart1_send_data(Resu.x, Resu.y, Resu.w, Resu.h);
	}
}
