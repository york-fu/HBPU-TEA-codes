#include "main.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	LED_Init();                	// LED初始化
	KEY_Init();				   	// 按键初始化
	delay_init();	    	   	// 延时函数初始化
	uart_init(115200);		   	// 串口初始化
	
//	MPU_Init();					// 初始化MPU6050
//	while(mpu_dmp_init())		// 一定要初始化成功
//	{LED0_Flash(666);}
	
	NRF24L01_Init();    	  	// 初始化NRF24L01 
	while(NRF24L01_Check()==1)  // 检测NRF24L01
	{
		LED0_Flash(666);
	}
	LED0=1;
	NRF24L01_TX_Mode();			// 配置为发送模式 
	
	OLED_IIC_Init();			// 初始化OLEDIIC
	
	Adc_Init();					// ADC初始化
	
	OLED_Basic();				// 基本界面
	
	while(1)
	{	
		ADC_Gather();			// 摇杆数据采集
		KEY_Control();			// 按键控制
		NRF_SendData();			// 无线数据发送
		OLED_Display();			// OLED显示数据
	} 
}



