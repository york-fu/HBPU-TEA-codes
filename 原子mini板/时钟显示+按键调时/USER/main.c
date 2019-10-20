
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "lcd.h"
#include "display.h"
#include "key.h"

TimeStruct Time;
u8 update_flog;

/**时钟规则函数**/
void Timer_Rule()
{
	if(Time.sec>59)
	{
		Time.min++;
		Time.sec = 0;
	}		
	if(Time.min>59)
	{
		Time.hour++;
		Time.min = 0;
	}		
	if(Time.hour>23) 
	{
		Time.hour = 0;
	}
	
	if(Time.sec<0)
	{
		Time.min--;
		Time.sec = 59;
	}
	if(Time.min<0)
	{			
		Time.hour--;
		Time.min=59;
	}
		
	if(Time.hour<0)
	{
		Time.hour=23;
	}
}



/**按键设置时间**/
void KEY_SetTimer()
{
	u8 key, set_key;
	
	key = KEY_Scan(0);
	if(key==WKUP_PRES)		//按下设置键
	{
		set_key++;
		if(set_key>3)  set_key=0;
		
		while(set_key!=0)
		{	
			TIM_Cmd(TIM3, DISABLE);		//TIM3定时器失能
			
			switch(set_key)
			{
				case 1:
					Add_Icon(144,206-1);
					Subtract_Icon(144,254);
					break;
				
				case 2:
					LCD_Fill(144,206-1,144+24,206-1+24,BLACK);	//刷新设置区域
					LCD_Fill(144,254,144+24,256+24,BLACK);		//刷新设置区域
				
					Add_Icon(108,206-1);
					Subtract_Icon(108,254);
					break;
				
				case 3:
					LCD_Fill(108,206-1,108+24,206-1+24,BLACK);	//刷新设置区域
					LCD_Fill(108,254,108+24,254+24,BLACK);		//刷新设置区域	
				
					Add_Icon(72,206-1);
					Subtract_Icon(72,254);
					break;
			}
			
			key = KEY_Scan(0);
			if(key==WKUP_PRES)		//按下设置键
			{
				set_key++;
				if(set_key>3)  set_key=0;
			}
			
			else if(key==KEY0_PRES)		//加时间
			{
				LCD_Fill(50,30,190,170,BLACK);	//刷新时钟区域
				switch(set_key)
				{
					case 1:
						Time.sec++;break;
					case 2:
						Time.min++;break;
					case 3:
						Time.hour++;break;
				}
			}
			
			else if(key==KEY1_PRES)		//减时间
			{
				LCD_Fill(50,30,190,170,BLACK);	//刷新时钟区域
				switch(set_key)
				{
					case 1:
						Time.sec--;break;
					case 2:
						Time.min--;break;
					case 3:
						Time.hour--;break;
				}
			}
			Timer_Rule();		//时间函数
			Analog_Timer_Display();	//模拟时钟显示	
			Display_Time(Time);		//数字时钟显示
		}
		LCD_Fill(72,206-1,72+24,206-1+24,BLACK);	//刷新设置区域
		LCD_Fill(72,254,72+24,254+24,BLACK);		//刷新设置区域
		TIM_Cmd(TIM3, ENABLE);		//TIM3定时器使能
	}
}

/*********************************************************************
函数名称：main
函数功能：主函数
输入参数：无
返回参数：无
*********************************************************************/
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断分组2
	delay_init();
	KEY_Init();
	LED_Init();
	LCD_Init();
	TIM3_Int_Init(10000-1, 7200-1);		//TIM3中断初始化,0.1S中断一次
	
	BACK_COLOR = BLACK;		//设置默认背景黑色
	POINT_COLOR = WHITE;	//设置画笔颜色白色
	LCD_Clear(BLACK);		//清屏为黑色
	
	while(1)
	{	
		KEY_SetTimer();			//按键调时
		if(update_flog==1)		//时间更新
		{
			Timer_Rule();		//时间函数
			LCD_Fill(50,30,190,170,BLACK);	//刷新时钟区域
			Analog_Timer_Display();		//模拟时钟显示	
			Display_Time(Time);			//数字时钟显示
			
			update_flog = 0;	//清除更新标志，等待下一次更新
		}
	}
}

