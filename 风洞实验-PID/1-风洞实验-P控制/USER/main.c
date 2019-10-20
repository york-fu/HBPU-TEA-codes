#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ultrasonic.h"
#include "timer.h"
#include "pid.h"


u8 nvic_flog = 0;

/************************************************
************************************************/
 	
int main(void)
{	 
	u8 key;
	u16 num,x,pwmvalue=2000;
	float dis,pid_value;
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();			    //LED端口初始化
	KEY_Init();
	LCD_Init();
	TIM2_Capture_Init(60000-1, 72-1);	//TIM2中断初始化,频率1M，计数到60000为60ms
	TIM3_PWM_Init(10000-1,72-1);		//TIM3_PWM初始化
	Ultrasonic_Init();		//超声波初始化  
	
	LCD_ShowString(20,50,220,16,16,"Distance: 000.00 cm");
	 
	while(1) 
	{	
		if(x%20==0)
		{
			dis = Ultrasonic_Ranging();
			if(dis==-1)
			{
				LCD_ShowString(100,50,56,16,16," error ");
			}
			else
			{
				num = dis*100;
				LCD_ShowxNum(100,50,num/100,3,16,0x80);
				LCD_ShowString(124,50,8,16,16,".");	//显示"."
				LCD_ShowxNum(132,50,num%100,2,16,0x80);
				
//				pid_value = PID_Adjust(dis);	//P调节
//				if(pid_value<0) 
//				{
//					pwmvalue=0;
//				}
//				else
//				{
//					pwmvalue = pid_value;
//				}
				TIM_SetCompare1(TIM3,pwmvalue);
			}
		}
		
		/*按键调波*/
//		key=KEY_Scan(0);
//		if(key==KEY0_PRES && pwmvalue<=10000)
//		{
//			pwmvalue+=500;
//			key=0;
//		}
//		if(key==KEY1_PRES && pwmvalue!=0)
//		{
//			pwmvalue-=500;
//			key=0;
//		}
			
		
		if(x%300==0)  LED1=!LED1;
		x++;
		if(x>60000)  x=0;				   		 
		delay_ms(1);	

	} 
}
