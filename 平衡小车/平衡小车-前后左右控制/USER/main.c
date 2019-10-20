#include "main.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置中断优先级分组2
	delay_init();					//延时函数初始化
	uart_init(9600);				//串口初始化为9600
	JTAG_Set(JTAG_SWD_DISABLE);		//关闭JTAG接口
	JTAG_Set(SWD_ENABLE);			//打开SWD接口
	LedInit();						//初始化LED
	KeyInit();						//初始化按键
	Tim2EncoderInit();				//初始化编码器1
	Tim4EncoderInit();				//初始化编码器2
	MotorInit();					//初始化电机驱动的引脚
	MotorPwmInit();					//初始化用于电机的两路PWM波,10KHz
	Tim1TimeIntInit();				//初始化定时器5,用于5ms中断
	
	MPU_Init();						//初始化MPU6050
	while (mpu_dmp_init())			//初始化DMP,并检查是否成功
	{
		LED = !LED;
		delay_ms(10);
	}
	LED = 1;
	
	while(1)
	{
		KeyStartAndStop();		//按键开启和关闭小车
		Usart1_Dispose();		//串口数据处理
	}
}

