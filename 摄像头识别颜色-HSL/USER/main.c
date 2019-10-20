#include "main.h"


/******************************************************************
*函数名称:	System_Init
*函数功能:	系统初始化
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void System_Init(void)		//系统初始化
{
	u8 i=0, num=1;
	
	delay_init();	    	 	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为 115200
	LCD_Init();			   		//初始化LCD 
 	Interface();				//初始化界面
		
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,20+i,290,BarColor);		//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
 	LED_Init();		  			//初始化与LED连接的硬件接口
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,40+i,290,BarColor);		//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	KEY_Init();					//初始化按键 
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,60+i,290,BarColor);		//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	TPAD_Init(6);				//触摸按键初始化
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,80+i,290,BarColor);		//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	IR_Tube_Init();				//红外对管初始化
	Tim4EncoderInit();			//定时器编码器模式初始化
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,100+i,290,BarColor);		//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	EXTI8_Init();				//使能定时器捕获
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,120+i,290,BarColor);	//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	TIM3_PWM_Init(7199, 0);		//定时器3，PWM波初始化，10KHz
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,140+i,290,BarColor);	//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	TIM8_PWM_Init(10000, 71);	//定时器8，PWM波初始化，100Hz 10000
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,160+i,290,BarColor);	//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	OV7670_init();				//OV7670初始化检查
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,180+i,290,BarColor);	//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	NRF24L01_init();			//无线模块初始化检查
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,200+i,290,BarColor);	//显示进度条
		LCD_ShowNum(104,300,num/2,3,16);		//显示百分比
		delay_ms(10);
	}		
	
	delay_ms(500);
	LCD_Clear(BLACK);
}


/******************************************************************
*函数名称:	Interface
*函数功能:	初始化界面
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Interface()
{
	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(0,50,240,24,24,(u8*)"Goods Sorting System");
	LCD_ShowString(30,90,200,16,16,(u8*)"SYSTEM Init...");	
	delay_ms(500);	
	LCD_Fill(19,269,221,291,LIGHTBLUE);	//显示进度条
	LCD_ShowString(128,300,8,16,16,(u8*)"%");
}


/******************************************************************
*函数名称:	OV7670_init
*函数功能:	初始化摄像头
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void OV7670_init()
{
	LCD_ShowString(30,110,200,16,16,(u8*)"OV7670 Init...");	
	
	while(OV7670_Init())	//初始化OV7670
	{
		LCD_ShowString(30,130,200,16,16,(u8*)"OV7670 Error!!!");
		delay_ms(200);
	    LCD_Fill(30,110,230,130,WHITE);
		delay_ms(200);
	}
	
 	LCD_ShowString(30,130,200,16,16,(u8*)"OV7670 Init OK");
	delay_ms(300);
	OV7670_Config();		//OV7670初始设置
}


/******************************************************************
*函数名称:	NRF24L01_init
*函数功能:	初始化无线
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void NRF24L01_init()
{
	LCD_ShowString(30,150,200,16,16,(u8*)"NRF24L01 Init...");	
	
	NRF24L01_Init();		//初始化NRF24L01
	
	while(NRF24L01_Check())	//无线自检
	{
		LCD_ShowString(30,170,200,16,16,(u8*)"NRF24L01 Error!!!");
		delay_ms(200);
	    LCD_Fill(30,150,230,170,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,170,200,16,16,(u8*)"NRF24L01 Init OK");
	delay_ms(500);
}


/******************************************************************
*函数名称:	Sys_Start
*函数功能:	系统无线启动程序
*函数参数:	无
*返 回 值:	0:没有启动信号
			1:有启动信号
*******************************************************************/
u8 Sys_Start()
{
	u8 Nrf_TX_buf[6]={"start"};	//发送数组
	
	if(TPAD_Scan(0) == 1)			//检测到按键 
	{
		NRF24L01_TX_Mode();		//设置无线为发送模式
		
		if(MAX_TX == NRF24L01_TxPacket(Nrf_TX_buf))	//无线发送启动命令给小车
		{			
//			NRF24L01_RX_Mode();	//设置无线为接收模式
			LED0 = 0;
		}
		return 1;
	}
	else 
	{	
		return 0;
	}
}


/******************************************************************
*函数名称:	Led_Test
*函数功能:	LED运行闪烁
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Led_Test(u16 time)
{
	static u16 i = 0;
	
	i++;
	if(i>59999) i=0;
	
	if(i%time==0)			//DS0闪烁.
	{
		LED0=!LED0;
	}
	delay_us(10);
}



u8 Conveyor_Run=0;	//传送带运行标记

/******************************************************************
*函数名称:	main
*函数功能:	主函数
*函数参数:	无
*返 回 值:	无
*******************************************************************/
int main(void)
{	
	u8 System_Run=0;	//系统运行标志
	
	System_Init();		//系统初始化
	
 	while(1) 
	{	
		Camera_refresh();			//更新LCD显示	
		
		if(System_Run==0)			//系统没有运行
		{
			if(Sys_Start())			//系统启动
			{	
				Conveyor_Advance();	//启动传送带
				System_Run = 1;		//标记系统运行
			}
					
		}
	
		if((TIM4->CNT > 3000)&&(TIM4->CNT<4000))
		{
			TIM4->CNT = 0;		//编码器计数清0
			Conveyor_Stop();
			System_Run = 0;
			LED0 = 1;
		}
		
		if(IR == 0)		//检测到木块
		{	
			delay_ms(5);
			if(IR == 0)
			{
				LED1 = 1;
				
				if(System_Run)				//系统运行
				{
					if(Conveyor_Run)		//传送带运行
					{			
						Conveyor_Stop();	//停止传送带
						TIM4->CNT = 0;		//编码器计数清0
						Discern_Flag = 1;	//标记颜色识别	
					}
				}
			}
		}
		else
		{
			LED1 = 0;
		}
		
		if(over)		//识别完成
		{
			Result_Handler(COLOR);
			over = 0;
			
			if(IR==1)
			{
				Conveyor_Advance();	//启动传送带
			}
			else
			{
				Discern_Flag = 1;	//标记颜色识别				
			}
		}
			
//		Led_Test(30);	
	}	   
}


