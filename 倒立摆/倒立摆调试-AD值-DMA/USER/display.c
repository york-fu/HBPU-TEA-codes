#include "display.h"
#include "lcd.h"
#include "control.h"
#include "mytouch.h"


/********************************************************************************
函数功能：初始化界面
入口参数：无
返回参数：无
函数作者：@FUYOU
*********************************************************************************/
void Interface_Init()
{
	LCD_ShowString(100,30,180,24,24,(u8*)"ADC_Value:    ");
	LCD_ShowString(100,70,400,24,24,(u8*)"Actual_Angle: ");
	LCD_ShowString(100,110,400,24,24,(u8*)"Expect_Value: ");
	
	POINT_COLOR = RED;		//画笔显示红色
	LCD_ShowString(80-30,170,200-30,24,24,(u8*)"Procedure:");
	POINT_COLOR = BLUE;	
	
	Display_4X1_Key(0,400,479,520);
	Display_4X4_Keypad(0,540,479,798,24);
}


/********************************************************************************
函数功能：操作界面
入口参数：无
返回参数：无
函数作者：@FUYOU
*********************************************************************************/
void Operation_Interface(u16 AdcValue)
{
	/*显示数据*/
	LCD_ShowxNum(268,30,AdcValue,4,24,0x80);			//显示ADC的值
	
	LCD_ShowxNum(268,70,(int)Angle,3,24,0x80);			//显示实际角度
	LCD_ShowChar(304,70,'.',24,0);						//显示"."
	LCD_ShowxNum(316,70,(int)(Angle*100)%100,2,24,0x80);
	
	if(Press_Flag)
	{
		Press_Flag = 0;
		
		if(Key_Flag==7)
		{	
			if(AimAngleSet)		//设置角度
			{
				BACK_COLOR = LIGHTBLUE;
				LCD_Fill(268,110,316,134,LIGHTBLUE);
				LCD_ShowxNum(268,110,SetAim,4,24,0x80);
				BACK_COLOR = BLACK;
			}
			else				//未设置
			{			
				BACK_COLOR = LIGHTBLUE;
				LCD_Fill(268,110,316,134,LIGHTBLUE);
				LCD_ShowxNum(268,110,(int)Aim,4,24,0x80);
				BACK_COLOR = BLACK;
			}	
		}
		else
		{
			LCD_Fill(268,110,316,134,BLACK);
			LCD_ShowxNum(268,110,(int)Aim,4,24,0x80);
		}
				
		POINT_COLOR = RED;		//画笔显示红色
		if(Key_Flag==1)
		{
			LCD_Fill(80-30,220,140-30,244,LIGHTBLUE);
			LCD_ShowString(80-30,220,60,24,24,(u8*)"- 1 -");
		}
		else
		{
			LCD_Fill(80-30,220,140-30,244,BLACK);
			LCD_ShowString(80-30,220,60,24,24,(u8*)"- 1 -");
		}
		
		if(Key_Flag==2)
		{
			LCD_Fill(240-30,220,300-30,244,LIGHTBLUE);
			LCD_ShowString(240-30,220,60,24,24,(u8*)"- 2 -");
		}
		else
		{
			LCD_Fill(240-30,220,300-30,244,BLACK);
			LCD_ShowString(240-30,220,60,24,24,(u8*)"- 2 -");
		}
		
		if(Key_Flag==3)
		{
			LCD_Fill(400-30,220,460-30,244,LIGHTBLUE);
			LCD_ShowString(400-30,220,60,24,24,(u8*)"- 3 -");
		}
		else
		{
			LCD_Fill(400-30,220,460-30,244,BLACK);
			LCD_ShowString(400-30,220,60,24,24,(u8*)"- 3 -");
		}
		
		if(Key_Flag==4)
		{
			LCD_Fill(80-30,260,140-30,284,LIGHTBLUE);
			LCD_ShowString(80-30,260,60,24,24,(u8*)"- 4 -");
		}
		else
		{
			LCD_Fill(80-30,260,140-30,284,BLACK);
			LCD_ShowString(80-30,260,60,24,24,(u8*)"- 4 -");
		}	
		
		if(Key_Flag==5)
		{
			LCD_Fill(240-30,260,300-30,284,LIGHTBLUE);
			LCD_ShowString(240-30,260,60,24,24,(u8*)"- 5 -");
		}
		else
		{
			LCD_Fill(240-30,260,300-30,284,BLACK);
			LCD_ShowString(240-30,260,60,24,24,(u8*)"- 5 -");
		}
		
		if(Key_Flag==6)
		{
			LCD_Fill(400-30,260,460-30,284,LIGHTBLUE);
			LCD_ShowString(400-30,260,60,24,24,(u8*)"- 6 -");
		}
		else
		{
			LCD_Fill(400-30,260,460-30,284,BLACK);
			LCD_ShowString(400-30,260,60,24,24,(u8*)"- 6 -");
		}
		POINT_COLOR = BLUE;	
	}
	
	if(Run==0)
	{
		LCD_Fill(100,320,400,344,BLACK);
		LCD_ShowString(100,320,400,24,24,(u8*)"Not Run...");
	}
	else
	{
		LCD_Fill(100,320,400,344,BLACK);
		LCD_ShowString(100,320,400,24,24,(u8*)"In Service...");
	}
}
