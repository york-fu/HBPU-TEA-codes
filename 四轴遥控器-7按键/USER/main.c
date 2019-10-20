#include "main.h"


u8 LOCK=0;	//锁定，解锁

/*
发送BUF
TXbuf[0]: 0，锁定；1，解锁 
TXbuf[1]: 0，无动作；1，加油门；2，减油门
TXbuf[2]: 0，无动作；1，前；2，后；3，左；4，右
*/
u8 TXbuf[3]={0};

/**************************************************************************************
函数功能：	LED灯闪烁
入口参数：	LEDx：选择闪烁的灯
			xms：闪烁时间,ms
返回参数：	无	
***************************************************************************************/
void LED_Flash(LEDx ledx,u16 xms)
{
	static u16 t_us=0,t_ms=0;
	
	delay_10us(1);
	t_us++;
	if(t_us>90)
	{
		t_us=0;
		t_ms++;
		if(t_ms>xms)
		{
			t_ms = 0;
			if(ledx==LED_0)
			{
				LED0 = !LED0;
			}
			else if(ledx==LED_1)
			{		
				LED1 = !LED1;
			}
		}
	}
}

/*************************************************************************
函数功能：	主函数
入口参数：	无
返回参数：	无
**************************************************************************/
void main()
{
	u16 key=0;

//	USART_Init(9600);		//初始化串口，波特率9600
	Nrf24l01Init();			//无线初始化
	while(CheckNrf24l01())	//无线自检
	{
		LED1 = !LED1;
		delay_ms(100);
	}
	LED1 = 1;
	Nrf24l01SetTXMode();	//设置无线为发送模式

	TIM0Int_Init(60000);	//定时器0中断初始化，60ms
	   
	while(1)
	{
		if(KEY1==0)
		{
			delay_ms(10);
			if(KEY1==0)
			{
				LOCK = !LOCK; 	//锁定、解锁
				while(!KEY1);	//松手检测
			}
		}

		if(LOCK)		//解锁
		{		
			LED1 = 0;

			key = KEY_Scan(1);
			
			if(key!=0)
			{	
				if(key==KEY0_VALUE)
				{
					TXbuf[1]=1;	
				}
				else if(key==KEY2_VALUE)
				{
					
					TXbuf[1]=2;	
				}
				else
				{
				   switch(key)
				   {
				   		case KEY3_VALUE:	//前
							TXbuf[2]=1;
							break; 
				   		case KEY4_VALUE:	//左
							TXbuf[2]=3;
							break;
				   		case KEY5_VALUE:	//后 
							TXbuf[2]=2;
							break;
				   		case KEY6_VALUE:	//右
							TXbuf[2]=4;
							break;
						default:
							TXbuf[2]=0;
				   }
				}
			}
			else
			{	
				LED1 = 1;			 
				TXbuf[1]=0;
				TXbuf[2]=0;
			}
		}
		else
		{
			LED1 = 1;
		}
	}	
}