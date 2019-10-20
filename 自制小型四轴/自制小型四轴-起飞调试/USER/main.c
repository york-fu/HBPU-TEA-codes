#include "main.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置中断优先级分组2
	delay_init();					//延时函数初始化
	uart_init(9600);				//串口初始化为9600
	LED_Init();						//初始化LED
//	KEY_Init();						//初始化按键
	
	NRF24L01_Init();				//无线初始化
	while (NRF24L01_Check())		//检查无线是否成功
	{
		LED1 = !LED1;
		delay_ms(100);
	}
	NRF24L01_RX_Mode();				//设置无线为接收模式
	
	MPU_Init();						//初始化MPU6050
	while (mpu_dmp_init())			//初始化DMP,并检查是否成功
	{
		LED1 = !LED1;
		delay_ms(100);
	}
	LED1 = 1;
	
	delay_ms(1000);
	
	MotorPWMInit(7199,0);			//初始化用于电机的两路PWM波,10KHz
	TIM1_IntInit(4999,71);			//初始化定时器1,用于5ms中断
	
	while(1)
	{	
		Usart1_Dispose();	//串口数据处理
		NRF_Receive();		//无线接收数据
	}
}

