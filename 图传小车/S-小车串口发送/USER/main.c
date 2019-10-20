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
	delay_init();					//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组2
	uart_init(UAR1,115200);			//串口初始化为115200
	MyUSART3_init(1382400);			//串口初始化为1382400
	JTAG_Set(JTAG_SWD_DISABLE);		//关闭JTAG接口
	JTAG_Set(SWD_ENABLE);			//打开SWD接口
	LED_Init();						//初始化LED
	KEY_Init();						//初始化按键
//	BluetoothIO_init();				//蓝牙端口初始化
	mallco_dev.init(SRAMIN);		//初始化内存池
	TX_buf = mymalloc(SRAMIN,buf_size);
	if(TX_buf==NULL)
	{
		printf("TX buf Error!\r\n");
		while(1)
		{
			LED_Flash(LED_1,20);
		}
	}
	else printf("TX buf OK!\r\n");
	
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
	OV7670_Window_Set(10,174,OvLcdWide,OvLcdHigh);	//设置窗口								  
	EXTI3_Init();						//使能定时器捕获

	DMA1_Channel3_Config((u32)&USART3->DR,(u32)&USART3_flag,1);	//串口3接收DMA
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	//使能串口3接收DMA
	
	DMA1_Channel2_Config((u32)&USART3->DR,(u32)TX_buf,buf_size);//串口3发送DMA	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	
//	TIM3_Int_Init(9999,7199);		//帧率统计
	TIM8_PWM_init(7199,0);			//定时器PWM波初始化，10KHz
	TIM1_IntInit(29999,71);			//定时器中断初始化，30ms
	
//	while(Bluetooth_P)				//等待蓝牙连接
//	{
//		delay_ms(1);
//	}
//	delay_ms(200);
	OV7670_CS=0;	
	printf("Init OK!\r\n");
	
	while(1)
	{		
		Image_DataHandle();		//图像数据处理
	}
}

