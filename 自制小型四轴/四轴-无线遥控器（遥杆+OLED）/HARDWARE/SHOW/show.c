#include "show.h"
#include "main.h"


void OLED_Basic(void)
{	
	u8 i=0;
	OLED_ON();
	OLED_Fill(0x00);//全屏灭
	for(i=0;i<5;i++)
	{
		OLED_ShowCN(24+i*16,0,i);//四轴飞行器
	}
}

/*
	╱▁╲
	▏锁▕
	╲▔╱
*/
void Lock_Display(void)
{
	u8 i,j,k=0;
	static u8 time=0;
	static u8 flag=0;
	
	for(i=1;i<3;i++)//显示锁
	{
		for(j=1;j<4;j++)
		{
			OLED_ShowCN(24+j*16,i*2,22+k);
			k++;
		}
	}
	
	if(time>=4)//散弹发射  
	{
		time=0;
		flag++;
		if(flag>=4)flag=0;
		for(i=5+flag;i<8;i++)
		{
			OLED_ShowCN(i*16,6,31);//"    "
		}
	}
	for(i=0;i<5+flag;i++)
	{
		OLED_ShowCN(i*16,6,5+i);//▄︻┻┳═一∵∴∷∶∵(∵_,∵)>>>>散弹发射！！
	}
	time++;
}

/*
	↖↑↗
	←㊣→
	↙↓↘
mode: 	0-8  	9种不同的飞行状态
speed:	1,2,3,4,5	代表三个不同的速度
*/
void Flying_Display(u8 mode,u8 speed)
{
	u8 i,j,k=0;
	static u16 flag=0;
	static u16 time=0;
	
	for(i=1;i<4;i++)//显示方向
	{
		for(j=1;j<4;j++)
		{
			if(k==mode)// 闪烁效果
			{
				if(flag)OLED_ShowCN(24+j*16,i*2,13+k);
				else OLED_ShowCN(24+j*16,i*2,31);
			}
			else 
				OLED_ShowCN(24+j*16,i*2,13+k);
			k++;
		}
	}
	if(time>=2*speed)  // 2*speed  
	{
		time=0;
		flag=!flag;
	}
	time++;
} 

void data_display(u8 x,u8 y,u8 data)
{
	u8 string[3];
		
	if(data>=5)
	{
		if(data==5)string[0]=' ',string[1]=' ';
		else 
		{
			string[0]='+';
			string[1]=data-5+'0';
		}
	}
	else 
	{
		string[0]='-';
		string[1]=5-data+'0';
	}
	string[2]='\0';
	OLED_ShowStr(x,y,string,2);
}

void OLED_Display(void)
{
	static u8 ARMED_Change=0;
	if(ARMED_Change!=ARMED)  // 切换一次 清屏
	{	
		ARMED_Change=ARMED;
		OLED_Basic(); 
	}
	
	if(ARMED)
	{
		u8 mode,speed,data[2];
		
		data_display(0,3,THROTTLR);  // 油门变换量
		data_display(0,5,Rotation);  // 旋转控制值
		data_display(112,3,Before_after);  // 前后控制值
		data_display(112,5,Right_left);    // 左右控制值
		
		if(Before_after>5)  // 得出显示方向和速度
		{
			if(Right_left>5)mode=2;
			else if(Right_left<5)mode=0;
			else mode=1;
		}
		else if(Before_after<5)
		{
			if(Right_left>5)mode=8;
			else if(Right_left<5)mode=6;
			else mode=7;
		}
		else 
		{
			if(Right_left>5)mode=5;
			else if(Right_left<5)mode=3;
			else mode=4;
		}
		data[0]=myabs(Before_after-5);
		data[1]=myabs(Right_left-5);
		if(data[0]==0)speed=6-data[1];
		else if(data[1]==0)speed=6-data[0];
		else speed=6-(data[0]+data[1])/2;
		if(mode==4)speed=3;
		
		Flying_Display(mode,speed);
	}
	else Lock_Display();
}
