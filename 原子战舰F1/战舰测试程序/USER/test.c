#include "sys.h" 	
#include "delay.h"	
#include "led.h"


int main(void)
{				  
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);	  	//延时初始化
	LED_Init();		  	//初始化与LED连接的硬件接口
	while(1)
	{
		GPIO_L;
		delay_ms(300);
		GPIO_H;
		delay_ms(300);
	}	 
}
