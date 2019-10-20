#include "mytouch.h"
#include "touch.h"
#include "lcd.h"
#include "delay.h"
#include "control.h"
#include "display.h"


s8 Key_Flag = 1;
u8 Press_Flag=1;
u8 Press_Flag_4X4=0;
u16 AimAngleSet = 0;
u16 SetAim = 0;

/********************************************************************************
函数功能：画水平线
入口参数：x0,y0:坐标；
		  len:线长度
		  color:颜色
返回参数：无
*********************************************************************************/
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0) return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}


/********************************************************************************
函数功能：画实心圆
入口参数：x0,y0:坐标
		  r:半径
		  color:颜色
返回参数：无
*********************************************************************************/
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  



/********************************************************************************
函数功能：画一条粗线
入口参数：(x1,y1),(x2,y2):线条的起始坐标
		  size：线条的粗细程度
		  color：线条的颜色
返回参数：无
*********************************************************************************/
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   


/********************************************************************************
函数功能：显示4*4数字键盘
入口参数：(x1,y1),(x2,y2):键盘的对角坐标
		  size：键盘数字大小
返回参数：无
函数作者：@FUYOU
*********************************************************************************/
void Display_4X4_Keypad(u16 x1, u16 y1, u16 x2, u16 y2, u8 size)
{
	u16 x, y;
	
	if((x1>x2)||(y1>y2)||(x1>lcddev.width)||(x2>lcddev.width)||(y1>lcddev.height)||(y2>lcddev.height)) return;
	
	x=(x2-x1)/4;
	y=(y2-y1)/4;
	
	LCD_DrawRectangle(x1,y1,x2,y2);		//画框
	
	/*画竖线*/
	LCD_DrawLine(x1+x,y1,x1+x,y2);
	LCD_DrawLine(x1+x*2,y1,x1+x*2,y2);
	LCD_DrawLine(x1+x*3,y1,x1+x*3,y2);
	
	/*画横线*/
	LCD_DrawLine(x1,y1+y,x2,y1+y);
	LCD_DrawLine(x1,y1+y*2,x2,y1+y*2);
	LCD_DrawLine(x1,y1+y*3,x2,y1+y*3);
	
	/*显示键盘内容*/
	/*第一行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+(y/2-size/2),'1',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+(y/2-size/2),'2',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+(y/2-size/2),'3',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+(y/2-size/2),'C',size,1);
	
	/*第二行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y+(y/2-size/2),'4',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y+(y/2-size/2),'5',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y+(y/2-size/2),'6',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y+(y/2-size/2),'X',size,1);
	
	/*第三行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y*2+(y/2-size/2),'7',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y*2+(y/2-size/2),'8',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*2+(y/2-size/2),'9',size,1);
	LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y*2+(y/2-size/2),'X',size,1);
	
	/*第四行*/
	LCD_ShowChar(x1+(x/2-size/4),y1+y*3+(y/2-size/2),'.',size,1);
	LCD_ShowChar(x1+x+(x/2-size/4),y1+y*3+(y/2-size/2),'0',size,1);
	LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*3+(y/2-size/2),'<',size,1);
	LCD_ShowString(x1+x*3+(x/2-size/2),y1+y*3+(y/2-size/2),24,24,24,(u8*)"OK");
}


/********************************************************************************
函数功能：4*4触摸按键,支持连按
入口参数：(x1,y1),(x2,y2):键盘的对角坐标
返回参数：按键值
函数作者：@FUYOU
*********************************************************************************/
s8 Touch_Keyboard(u16 x1, u16 y1, u16 x2, u16 y2)
{
	static u8 Continuous=0;
	u8 flag=0,size=24;
	u16 x, y, t;
	u16 KeyColor=LIGHTBLUE;
	
	if((x1>x2)||(y1>y2)||(x1>lcddev.width)||(x2>lcddev.width)||(y1>lcddev.height)||(y2>lcddev.height)) return -1;
	
	x=(x2-x1)/4;
	y=(y2-y1)/4;
	
	tp_dev.scan(0);
	
	if((tp_dev.sta&0x80)==0x80)		//有按键按下
	{
		if(((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x2))&&((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y2)))	//在区域内
		{
			if((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y1+y))	//第一行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 1;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 2;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 3;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 12;
				}
			}
			else if((tp_dev.y[0]>y1+y)&&(tp_dev.y[0]<y1+y*2))	//第二行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 4;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 5;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 6;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 13;
				}
			}
			else if((tp_dev.y[0]>y1+y*2)&&(tp_dev.y[0]<y1+y*3))	//第三行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 7;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 8;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 9;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 14;
				}
			}
			else if((tp_dev.y[0]>y1+y*3)&&(tp_dev.y[0]<y1+y*4))	//第四行
			{
				if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
				{
					flag = 10;
				}
				else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
				{
					flag = 0;
				}
				else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
				{
					flag = 11;
				}
				else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
				{
					flag = 15;
				}
			}
			
			/*按下显示提示*/
			switch(flag)
			{
				case 1:LCD_Fill(x1+1,y1+1,x1+x-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+(y/2-size/2),'1',size,1);
					break;
				case 2:LCD_Fill(x1+x+1,y1+1,x1+x*2-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+(y/2-size/2),'2',size,1);
					break;
				case 3:LCD_Fill(x1+x*2+1,y1+1,x1+x*3-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+(y/2-size/2),'3',size,1);
					break;
				case 12:LCD_Fill(x1+x*3+1,y1+1,x1+x*4-1,y1+y-1,KeyColor);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+(y/2-size/2),'C',size,1);
					break;
				
				case 4:LCD_Fill(x1+1,y1+y+1,x1+x-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+y+(y/2-size/2),'4',size,1);
					break;
				case 5:LCD_Fill(x1+x+1,y1+y+1,x1+x*2-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y+(y/2-size/2),'5',size,1);
					break;
				case 6:LCD_Fill(x1+x*2+1,y1+y+1,x1+x*3-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y+(y/2-size/2),'6',size,1);
					break;
				case 13:LCD_Fill(x1+x*3+1,y1+y+1,x1+x*4-1,y1+y*2-1,KeyColor);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y+(y/2-size/2),'X',size,1);
					break;
							
				case 7:LCD_Fill(x1+1,y1+y*2+1,x1+x-1,y1+y*3,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+y*2+(y/2-size/2),'7',size,1);
					break;
				case 8:LCD_Fill(x1+x+1,y1+y*2+1,x1+x*2-1,y1+y*3-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y*2+(y/2-size/2),'8',size,1);
					break;
				case 9:LCD_Fill(x1+x*2+1,y1+y*2+1,x1+x*3-1,y1+y*3-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*2+(y/2-size/2),'9',size,1);
					break;
				case 14:LCD_Fill(x1+x*3+1,y1+y*2+1,x1+x*4-1,y1+y*3-1,KeyColor);
					LCD_ShowChar(x1+x*3+(x/2-size/4),y1+y*2+(y/2-size/2),'X',size,1);
					break;
					
				case 10:LCD_Fill(x1+1,y1+y*3+1,x1+x-1,y1+y*4-1,KeyColor);
					LCD_ShowChar(x1+(x/2-size/4),y1+y*3+(y/2-size/2),'.',size,1);
					break;
				case 0:LCD_Fill(x1+x+1,y1+y*3+1,x1+x*2-1,y1+y*4-1,KeyColor);
					LCD_ShowChar(x1+x+(x/2-size/4),y1+y*3+(y/2-size/2),'0',size,1);
					break;
				case 11:LCD_Fill(x1+x*2+1,y1+y*3+1,x1+x*3-1,y1+y*4-1,KeyColor);
					LCD_ShowChar(x1+x*2+(x/2-size/4),y1+y*3+(y/2-size/2),'<',size,1);
					break;
				case 15:LCD_Fill(x1+x*3+1,y1+y*3+1,x1+x*4-1,y1+y*4-1,KeyColor);
					LCD_ShowString(x1+x*3+(x/2-size/2),y1+y*3+(y/2-size/2),24,24,24,(u8*)"OK");
					break;
			}
			
			t=0;
			while((tp_dev.sta&0x80)==0x80)	//松手检测
			{
				if(Continuous==0)		//正常模式
				{
					delay_ms(1);
					tp_dev.scan(0);
					t++;
					if(t>599)
					{
						Continuous=1;	//进入连按模式
						break;
					}
				}
				else		//连按模式
				{	
					delay_ms(100);
					break;
				}
			}
			
			Press_Flag=1;
			Press_Flag_4X4=1;
			LCD_Fill(x1,y1,x2,y2,BLACK);
			Display_4X4_Keypad(x1,y1,x2,y2,size);
		}
	}
	else Continuous = 0;
	
	return flag;
}


/********************************************************************************
函数功能：显示4*1触摸按键,圆形
入口参数：无
返回参数：无
函数作者：@FUYOU
*********************************************************************************/
void Display_4X1_Key(u16 x1,u16 y1,u16 x2,u16 y2)
{
	u8 size=24;
	u16 KeyColor = BLUE;
	u16 x,y;
	
	if((x1>x2)||(y1>y2)||(x1>lcddev.width)||(x2>lcddev.width)||(y1>lcddev.height)||(y2>lcddev.height)) return;
	
	x=(x2-x1)/4;
	y=y2-y1;
	
	/*显示圆*/
	gui_fill_circle(x1+x/2,y1+y/2,x/2-x/10,KeyColor);
	gui_fill_circle(x1+x/2+x,y1+y/2,x/2-x/10,KeyColor);
	gui_fill_circle(x1+x/2+x*2,y1+y/2,x/2-x/10,KeyColor);
	gui_fill_circle(x1+x/2+x*3,y1+y/2,x/2-x/10,KeyColor);
	
	/*显示内容*/
	POINT_COLOR = WHITE;
	LCD_ShowString((x1+x/2)-(size*2)/4,(y1+y/2)-size/2,(size*2)/2,size,size,(u8*)"<<");
	LCD_ShowString((x1+x/2+x)-(size*2)/4,(y1+y/2)-size/2,(size*2)/2,size,size,(u8*)">>");
	LCD_ShowString((x1+x/2+x*2)-(size*4)/4,(y1+y/2)-size/2,(size*4)/2,size,size,(u8*)"Stop");
	LCD_ShowString((x1+x/2+x*3)-(size*5)/4,(y1+y/2)-size/2,(size*5)/2,size,size,(u8*)"Start");	
	POINT_COLOR = BLUE;
}


/********************************************************************************
函数功能：4*1触摸按键,圆形，支持连按
入口参数：无
返回参数：按键值
函数作者：@FUYOU
*********************************************************************************/
u8 Touch_Key(u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 KeyColor = LIGHTBLUE;
	u16 x,y,t;
	u8 flag,size=24;
	static u8 Continuous=0;
		
	if((x1>x2)||(y1>y2)||(x1>lcddev.width)||(x2>lcddev.width)||(y1>lcddev.height)||(y2>lcddev.height)) return 0;
	
	x=(x2-x1)/4;
	y=y2-y1;
	
	tp_dev.scan(0);
	
	if((tp_dev.sta&0x80)==0x80)		//有按键按下
	{
		if(((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x2))&&((tp_dev.y[0]>y1)&&(tp_dev.y[0]<y2)))	//在区域内
		{
			if((tp_dev.x[0]>x1)&&(tp_dev.x[0]<x1+x))
			{
				flag = 17;
			}
			else if((tp_dev.x[0]>x1+x)&&(tp_dev.x[0]<x1+x*2))
			{
				flag = 18;
			}
			else if((tp_dev.x[0]>x1+x*2)&&(tp_dev.x[0]<x1+x*3))
			{
				flag = 19;
			}
			else if((tp_dev.x[0]>x1+x*3)&&(tp_dev.x[0]<x1+x*4))
			{
				flag = 20;
			}
			
			
			/*按下显示提示*/			
			POINT_COLOR = WHITE;
			switch(flag)
			{
				case 17:gui_fill_circle(x1+x/2,y1+y/2,x/2-x/10,KeyColor);
					LCD_ShowString((x1+x/2)-(size*2)/4,(y1+y/2)-size/2,(size*2)/2,size,size,(u8*)"<<");
					break;
				case 18:gui_fill_circle(x1+x/2+x,y1+y/2,x/2-x/10,KeyColor);
					LCD_ShowString((x1+x/2+x)-(size*2)/4,(y1+y/2)-size/2,(size*2)/2,size,size,(u8*)">>");
					break;
				case 19:gui_fill_circle(x1+x/2+x*2,y1+y/2,x/2-x/10,KeyColor);
					LCD_ShowString((x1+x/2+x*2)-(size*4)/4,(y1+y/2)-size/2,(size*4)/2,size,size,(u8*)"Stop");
					break;
				case 20:gui_fill_circle(x1+x/2+x*3,y1+y/2,x/2-x/10,KeyColor);
					LCD_ShowString((x1+x/2+x*3)-(size*5)/4,(y1+y/2)-size/2,(size*5)/2,size,size,(u8*)"Start");
					break;			
			}		
			POINT_COLOR = BLUE;
			
			t=0;
			while((tp_dev.sta&0x80)==0x80)	//松手检测
			{
				if(Continuous==0)		//正常模式
				{
					delay_ms(1);
					tp_dev.scan(0);
					t++;
					if(t>599)
					{
						Continuous=1;	//进入连按模式
						break;
					}
				}
				else		//连按模式
				{	
					delay_ms(100);
					break;
				}
			}
			
			Press_Flag=1;
			Display_4X1_Key(x1,y1,x2,y2);
		}
	}
	else Continuous = 0;
	
	return flag;
}



/********************************************************************************
函数功能：触摸按键设置
入口参数：无
返回参数：无
函数作者：@FUYOU
*********************************************************************************/
void TouchKey_Set()
{
	s8 key=0,x;
	static u8 KeyValue[4];
	
	/*4*1按键部分*/
	key = Touch_Key(0,400,479,520);
	
	if(Press_Flag)	//有按键按下
	{
		switch(key)
		{
			case 17:Run=0;
				Key_Flag--;
				if(Key_Flag<1) Key_Flag=7;
			
				break;
			case 18:Run=0;
				Key_Flag++;
				if(Key_Flag>7) Key_Flag=1;
				break;
			
			case 19:Run=0;
				Key_Flag=0;
			
				break;
			case 20:if((Key_Flag==1) || (Key_Flag==2) || (Key_Flag==3) || (Key_Flag==4) || (Key_Flag==5) || (Key_Flag==6)) 
				{
					Run=1;
				}
				break;
		}
		AimAngleSet = 0;	//设置角度关
		
		/*清空设置角度*/
		for(x=0;x<4;x++)	
		{
			KeyValue[x] = 0;
		}
	}
	
	/*4*4按键部分*/
	if(Run==0)
	{
		if(Key_Flag==7)
		{			
			key = Touch_Keyboard(0,540,479,798);
			
			if(Press_Flag_4X4)		//有4*4按键按下
			{
				Press_Flag_4X4 = 0;
				AimAngleSet = 1;	//标记角度设置				
				if(key==15)
				{
					Aim = KeyValue[0]*1000 + KeyValue[1]*100 + KeyValue[2]*10 + KeyValue[3];
					if(Aim>4095) Aim = 4095;
					AimAngleSet = 0;	//角度设置完成
					
					/*清空设置角度*/
					for(x=0;x<4;x++)	
					{
						KeyValue[x] = 0;
					}
				}
				else if(key==11)
				{
					KeyValue[3] = KeyValue[2];
					KeyValue[2] = KeyValue[1];
					KeyValue[1] = KeyValue[0];
					KeyValue[0] = 0;
				}
				else if(key<10)
				{		
					KeyValue[0] = KeyValue[1];
					KeyValue[1] = KeyValue[2];
					KeyValue[2] = KeyValue[3];
					KeyValue[3] = key;		
				}
				SetAim = KeyValue[0]*1000 + KeyValue[1]*100 + KeyValue[2]*10 + KeyValue[3];
			}
		}
		else
		{
			Touch_Keyboard(0,540,479,798);		//只显示按键
		}
	}
	
}


