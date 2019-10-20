
/***************
@ brief :  TEARS.C
@ auther:  KXT
@ date  :  2016.7.23
***************/

/*************************
@ WKUP -- 向左 换位
@ KEY0 -- 向右 确定
@ KEY1 -- 形状 改值
@ 右上角 -- 换种类
@ 右下角 -- 加速
**************************/

# include "stm32f10x.h"
# include "led.h"
# include "key.h"
# include "delay.h"
# include "lcd.h"
# include "touch.h" 
# include "usart.h"
# include "tears.h"
# include "timer.h"
# include "interface.h"

extern u32 sh_block[][4];
extern u32 dir_lines[][3];

extern _Shape NewShape;
u32 Interface_ChangeFLAG = 1;

int main(void)
{
	static u32 task1 = 0;
	static u32 down_Addspeed = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
 	LED_Init();			     //LED端口初始化
	KEY_Init();              //按键端口初始化
	uart_init(38400);        //串口初始化
	tp_dev.init();			 //触摸屏初始化
	TIME_Init(999,71);       //定时器初始化
	LCD_Init();              //LCD初始化
	
	Start_Interface();    //进入游戏前的选择界面以及
	Int_Block();          //对Block进行初始化

   	while(1)
	{
		Update_UserLevel(User_Date());    //根据用户得分来更新等级
		if(1 == Interface_ChangeFLAG)
		{
			Select_StartSpeed();
		}
		else if(0 == Interface_ChangeFLAG)
		{
			Interface_ChangeFLAG = 100;          //将Interface_ChangeFLAG=100，结束装载换面，进入下面程序循环
			Game_Interface();
		}	
		else if(100 == Interface_ChangeFLAG)   //由此开始是游戏运行的程序
		{	
			down_Addspeed = 0;
			KEY_Function();  //方向
			tp_dev.scan(0);  //扫描触摸屏
			if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
			{	
				if(tp_dev.x[0]<240&&tp_dev.y[0]<320)
				{
					if(tp_dev.y[0]>290 &&tp_dev.x[0]>210)  //加速
					{
						down_Addspeed = 70-Set_level(User_Date());
						delay_ms(30);
					}
					if(tp_dev.y[0]<30 &&tp_dev.x[0]>210)  
					{
						A_Key_FunCation();  //换数
					}
					LED1=!LED1;
				}
			}
			if(Systick_Count > task1+Set_level(User_Date())+down_Addspeed)
			{
				LED0 = !LED0;
				task1 = Systick_Count;
				Shape_Down_Move(&NewShape);  //图形移动部分
			}		
			Shape_Inform(&NewShape,Draw);  //图形的生成或者擦除
			Load_NextAndSetflag(&NewShape,sh_block,dir_lines);  //读边界坐标并设定移动标志
			Show_UserDate(User_Date());  //显示用户的分数和游戏等级信息
		}
	}		
}

