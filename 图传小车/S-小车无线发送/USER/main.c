#include "main.h"



/**************************************************************************************
函数功能：	LED灯闪烁
入口参数：	LEDx：选择闪烁的灯
			time：闪烁时间
返回参数：	无	
***************************************************************************************/
void LED_Flash(LEDx ledx,u16 time)
{
	static u16 t_1,t=0;
	
	delay_us(1);
	t_1++;
	if(t_1>499)
	{
		t_1 = 0;
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
	MyUSART3_init(115200);			//串口初始化为115200
	JTAG_Set(JTAG_SWD_DISABLE);		//关闭JTAG接口
	JTAG_Set(SWD_ENABLE);			//打开SWD接口
	LED_Init();						//初始化LED
	KEY_Init();						//初始化按键
//	mallco_dev.init(SRAMIN);		//初始化内存池
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		LED_Flash(LED_1,20);
	}
	NRF24L01_TX_Mode();				//初始化无线到发送模式
	
	while(OV7670_Init())			//初始化OV7670
	{
		LED_Flash(LED_1,20);
	}
	OV7670_Window_Set(10,174,240,320);	//设置窗口								  
	EXTI3_Init();						//使能定时器捕获

	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)&USART3_flag,1);	//串口2接收DMA
	
	TIM8_PWM_init(7199,0);			//定时器PWM波初始化，10KHz
	TIM1_IntInit(49999,71);			//定时器中断初始化，50ms
	
	OV7670_CS=0;
	
	printf("Init OK!\r\n");
	
	while(1)
	{		
		Image_DataHandle();		//图像数据处理
	}
}

