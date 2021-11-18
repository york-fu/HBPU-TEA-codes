#include "display.h"


/**数字时钟显示**/
void Display_Time(TimeStruct Time)
{	
	LCD_ShowString(96,230,12,2,24,":");	
	LCD_ShowString(132,230,12,2,24,":");	//显示":"
	
	LCD_ShowxNum(72,230,Time.hour,2,24,0X80);	//显示时
	LCD_ShowxNum(108,230,Time.min,2,24,0X80);	//显示分
	LCD_ShowxNum(144,230,Time.sec,2,24,0X80);	//显示秒
}



/**模拟时钟显示**/
void Analog_Timer_Display(void)
{
	u8 i=10, hour_needle;
	End_Coord center_xy,end_xy;
	
	LCD_Draw_Circle(120,100,75);	//画内圆
	LCD_Draw_Circle(120,100,77);	//画外圆
	LCD_Draw_Circle(120,100,2);		//画中心
	LCD_Draw_Circle(120,100,1);		//画中心
	
	for(i=0;i<60;i++)		//获取刻度盘坐标并画出刻度盘
	{
		if(i%5==0)		//显示小时刻度盘
		{			
			center_xy = Timer_Needle(120,100,60,i,0);
			end_xy = Timer_Needle(120,100,75,i,0);
			LCD_DrawLine((u16)center_xy.end_x, (u16)center_xy.end_y, (u16)end_xy.end_x, (u16)end_xy.end_y);
		}
		else		//显示其余刻度盘
		{
			center_xy = Timer_Needle(120,100,65,i,0);
			end_xy = Timer_Needle(120,100,75,i,0);
			LCD_DrawLine((u16)center_xy.end_x, (u16)center_xy.end_y, (u16)end_xy.end_x, (u16)end_xy.end_y);
		}
	}
	
	POINT_COLOR = RED;
	Timer_Needle(120,100,55,Time.sec,1);		//画秒针
	
	POINT_COLOR = DARKBLUE;
	Timer_Needle(120,100,45,Time.min+((float)Time.sec*(1.0/60.0)),1);		//画分针
	
	POINT_COLOR = WHITE;
	if(Time.hour>12)  hour_needle=Time.hour-12;		//画时针
	else hour_needle = Time.hour;
	Timer_Needle(120,100,30,(float)hour_needle*5.0+((float)Time.min*(5.0/60.0)),1);
}



End_Coord Timer_Needle(float center_x, float center_y, float length, float time, u8 draw_flog)
{
	End_Coord end_coord;
	
	if((time>=0)&&(time<15))	//第一象限
	{
		end_coord.end_x=center_x + length*cos((90.0-(time*6.0))*(3.1415/180.0));		//计算x坐标
		end_coord.end_y=center_y - length*sin((90.0-(time*6.0))*(3.1415/180.0));		//计算y坐标
	}
	
	else if((time>=15)&&(time<30))	//第四象限
	{
		end_coord.end_x=center_x + length*cos(((time*6.0)-90.0)*(3.1415/180.0));		//计算x坐标
		end_coord.end_y=center_y + length*sin(((time*6.0)-90.0)*(3.1415/180.0));		//计算y坐标
	}
	
	else if((time>=30)&&(time<45))	//第三象限
	{
		end_coord.end_x=center_x - length*cos((270.0-(time*6.0))*(3.1415/180.0));		//计算x坐标
		end_coord.end_y=center_y + length*sin((270.0-(time*6.0))*(3.1415/180.0));		//计算y坐标
	}
	
	else if((time>=45)&&(time<60))	//第二象限
	{
		end_coord.end_x=center_x - length*cos(((time*6.0)-270.0)*(3.1415/180.0));		//计算x坐标
		end_coord.end_y=center_y - length*sin(((time*6.0)-270.0)*(3.1415/180.0));		//计算y坐标
	}
	
	if(draw_flog)  LCD_DrawLine((u16)center_x, (u16)center_y, (u16)end_coord.end_x, (u16)end_coord.end_y);	//画出指针
	
	return end_coord;
}



void Add_Icon(u16 sx,u16 sy)
{
	LCD_Fill(sx,sy,sx+24,sy+24,GRAY);	//图标区域
	LCD_Fill(sx+4,sy+10,sx+24-4,sy+24-10,WHITE);
	LCD_Fill(sx+10,sy+4,sx+24-10,sy+24-4,WHITE);		
}



void Subtract_Icon(u16 sx,u16 sy)
{
	LCD_Fill(sx,sy,sx+24,sy+24,GRAY);	//图标区域
	LCD_Fill(sx+4,sy+10,sx+24-4,sy+24-10,WHITE);	
}
