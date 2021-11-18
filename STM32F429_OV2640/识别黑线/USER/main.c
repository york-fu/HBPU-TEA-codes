#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "pcf8574.h" 
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usmart.h"
#include "sdio_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"
#include "timer.h"
#include "EasyTracer.h"


/*********************************************************************
函数功能：	主函数
入口参数：	无
返回参数：	无
**********************************************************************/
int main(void)
{	
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz   
	HAL_Init();                     //初始化HAL库
	delay_init(180);                //初始化延时函数
	uart_init(921600);              //初始化USART
//	usmart_dev.init(90);            //初始化USMART
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	LCD_Init();                     //LCD初始化
	
	LCD_ShowString(20,20,200,24,24,(u8*)"OV2640 test");		
	
//	my_mem_init(SRAMIN);            //初始化内部内存池
	  	
//	dcmi_line_buf[0]=mymalloc(SRAMIN,jpeg_line_size*4);	//为jpeg dma接收申请内存	
//	dcmi_line_buf[1]=mymalloc(SRAMIN,jpeg_line_size*4);	//为jpeg dma接收申请内存

	while(OV2640_Init())//初始化OV2640
	{
		LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 Error!!!");
		delay_ms(200);
		LCD_Fill(20,50,220,80,BLACK);
		delay_ms(200);
	}
	LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 OK!"); 
	delay_ms(500);
	OV2640_Config();	//OV2640初始设置
	
	DCMI_Init();		//DCMI配置	
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);	//DCMI DMA配置,MCU屏
	
	LCD_Clear(BLACK);	
	TIM3_Init(10000-1,9000-1);		//10Khz计数,1秒钟中断一次
	DCMI_Start(); 					//启动传输 
	
	while(1)
	{		
		if(frame_over)	//一帧传输完成
		{
			Find_Midline();		//寻找黑线

			frame_over = 0;
			DCMI_Start();
		}
		
	}
}
