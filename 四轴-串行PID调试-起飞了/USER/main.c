#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "pwm.h"
#include "timer.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "control.h"
#include "myusart.h"
#include "spi.h"
#include "24l01.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	LED_Init();                	// LED初始化
	KEY_Init();				   	// 按键初始化
	delay_init();	    	   	// 延时函数初始化
	uart_init(115200);		   	// 串口初始化
	
	MPU_Init();					// 初始化MPU6050
	while(mpu_dmp_init())		// 一定要初始化成功
	{LED0_Flash(666);}
	
	TIM2_Init(7199,49);			// 频率200HZ
	Motor_SelfCheck();  		// 无刷电机自检函数 
	
	NRF24L01_Init();    	  	// 初始化NRF24L01 
	while(NRF24L01_Check()==1)  // 检测NRF24L01
	{LED0_Flash(666);}LED0=1;
	NRF24L01_RX_Mode();     	// 无线设置为接收模式 
	
	TIM1_Init(49,7199);         // 5MS进一次中断服务函数，中断服务函数在control.c
	
	while(1)
	{	
//		PC_data();  			// 给上位机发送欧拉角数据  时间：3.62ms
		USART_Adjust();  		// 串口数据处理
		Telecontrol();  		// 遥控控制 
	} 
}



