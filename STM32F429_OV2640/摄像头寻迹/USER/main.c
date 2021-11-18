#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "malloc.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "usmart.h"
#include "timer.h"
#include "motor.h"
#include "EasyTracer.h"


/*********************************************************************
函数功能：	主函数
入口参数：	无
返回参数：	无
**********************************************************************/
int main(void)
{	
	u8 key=0;
	
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz   
	HAL_Init();                     //初始化HAL库
	delay_init(180);                //初始化延时函数
	uart_init(921600);              //初始化USART
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	LCD_Init();                     //LCD初始化	
	
	LCD_ShowString(20,20,200,24,24,(u8*)"OV2640 test");		

	while(OV2640_Init())//初始化OV2640
	{
		LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 Error!!!");
		delay_ms(200);
		LCD_Fill(20,50,220,80,BLACK);
		delay_ms(200);
	}
	LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 OK!"); 
	delay_ms(500);
	OV2640_Config();		//OV2640初始设置
	
	DCMI_Init();			//DCMI配置	
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);	//DCMI_DMA配置,MCU屏	
	LCD_Clear(BLACK);		//清屏
	DCMI_Start(); 			//启动传输 
	
	TIM2_PWM_Init(8999,0);	//TIM，PWM初始化 ,10KHz
	TIM1_Init(9999,1799);	//定时器中断，100ms
	
	while(1)
	{				
		if(frame_over)	//一帧传输完成
		{
			Find_Midline();		//寻找黑线
			
			/*画中线*/
			POINT_COLOR = RED;
			LCD_DrawLine(160,100,160,140);
			POINT_COLOR = WHITE;
			
			frame_over = 0;
			DCMI_Start();
		}
		
		key = KEY_Scan(0);
		if(key == KEY0_PRES)
		{
			Run = !Run;
		}
	}
}
