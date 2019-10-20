#include "main.h"


/**************************************************************************************
函数功能：	LED灯闪烁
入口参数：	LEDx：选择闪烁的灯
			time：闪烁时间
返回参数：	无	
***************************************************************************************/
void LED_Flash(LEDx ledx,u16 time)
{
	static u16 t=0;
	
	delay_ms(1);
	t++;
	if(t>time)
	{
		t = 0;
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


/**************************************************************************************
函数功能：	主函数
入口参数：	无
返回参数：	无	
***************************************************************************************/
int main(void)
{	
	u8 key=0;
	
	delay_init();					//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组2
	uart_init(UAR1,115200);			//串口初始化为115200
	JTAG_Set(JTAG_SWD_DISABLE);		//关闭JTAG接口
	JTAG_Set(SWD_ENABLE);			//打开SWD接口
	LED_Init();						//初始化LED
	KEY_Init();						//初始化按键
	IrTube_Init();					//红外对管初始化
	
//	NRF24L01_Init();
//	while(NRF24L01_Check())
//	{
//		LED_Flash(LED_0,50);
//	}
//	NRF24L01_RX_Mode();				//初始化无线到接收模式
	
	MPU_Init();						//初始化MPU6050
	while (mpu_dmp_init())			//初始化DMP,并检查是否成功
	{
		LED_Flash(LED_0,50);
	}
	LED0 = 0;

	TIM2_PWM_init(7199,0);			//定时器PWM波初始化，10KHz
	TIM1_IntInit(499,71);			//初始化定时器1,用于50us中断
	
	while(1)
	{			
//		key = KEY_Scan(0);
//		if(key==KEY1_PRES)
//		{
//			Run = !Run;
//		}
		
//		Uart_DealDate();
//		LED_Flash(LED_0,500);
	}
}

