#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "key.h"   
#include "mpu6050.h" 
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "encoder.h" 
#include "pwm.h" 
#include "timer.h" 
#include "myusart.h"
#include "blance.h"
#include "24l01.h"

extern float roll[2];					//欧拉角


/*********************************************************************
函数功能：左右陀螺仪初始化
入口参数：无
返回参数：无
**********************************************************************/
void mpu_6050_init(void)
{
	u8 val=0;
	
	USE_MPU = MPU_FIRST;
	while(MPU_Init(USE_MPU))					//初始化MPU6050_1
	{
		printf("MPU1_Init error\r\n");
		LED0=1;
		delay_ms(100);
		LED0=0;
		delay_ms(100);
	}
	while((mpu_dmp_init(USE_MPU)))		//陀螺仪1初始化
	{
		printf("mpu1_dmp_init error\t%d\r\n",val);
		LED0=1;
		delay_ms(100);
		LED0=0;
		delay_ms(100);
	}
	
	USE_MPU = MPU_SECOND;
	while(MPU_Init(USE_MPU))				//初始化MPU6050_2
	{
		printf("MPU2_Init error\r\n");
		LED1=1;
		delay_ms(100);
		LED1=0;
		delay_ms(100);
	}	
	while((mpu_dmp_init(USE_MPU)))		//陀螺仪2初始化
	{
		printf("mpu2_dmp_init error\t%d\r\n",val);
		LED1=1;
		delay_ms(100);
		LED1=0;
		delay_ms(100);
	} 
}


/*********************************************************************
函数功能：无线初始化和自检
入口参数：无
返回参数：无
**********************************************************************/
void NRF24L01_init()
{
	NRF24L01_Init();    	  		// nrf24l01 init
	
	while(NRF24L01_Check()==1)  	// nrf24L01 check
	{
		LED0_Flash(666);
	}
	LED0=1;
	
	NRF24L01_RX_Mode();     		// set to receive mode
}


/*********************************************************************
函数功能：一阶滞后滤波
入口参数：待处理数据
返回参数：无
**********************************************************************/
void FirstOrder_Filtering(short* value)
{
	static short LastValue=0;
	
	*value = (*value) * 0.3 + LastValue * 0.7;
	LastValue = *value;
}


/*********************************************************************
函数功能：获取陀螺仪数据
入口参数：无
返回参数：无
**********************************************************************/
void Get_MPU6050()
{
	USE_MPU = MPU_FIRST;
	if(mpu_dmp_get_data(roll)==0)	//读取到dmp数据
	{							
		if(roll[0] < 0) roll[0] += 180;					
		else roll[0] = roll[0] - 180;
		
		FirstOrder_Filtering(&(Gyro[0][0]));
	}
	
	USE_MPU = MPU_SECOND;
	if(mpu_dmp_get_data(roll)==0)	//读取到dmp数据
	{			
//		if(roll[1] < 0) roll[1] += 180;					
//		else roll[1] = roll[1] - 180;	
		
		FirstOrder_Filtering(&(Gyro[1][0]));			
	}
	
//	printf("MPU_1: %f\t%d\t MPU_2: %f\t%d\r\n",roll[0],Gyro[0][0],roll[1],Gyro[1][0]);
}


/*********************************************************************
函数功能：载人平衡车主函数
入口参数：无
返回参数：无
**********************************************************************/
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 		//延时函数初始化	  
	uart_init(UAR1,9600);	 		//串口初始化
	LED_Init();		  				//初始化与LED连接的硬件接口
	KEY_Init();						//初始化按键
	
	mpu_6050_init();				//左右陀螺仪初始化
//	NRF24L01_init();				// nrf24l01 init and check
		
	MOTO_DIR_Init();
	TIM2_PWM_Init(7199,0);		
		
	TIM1_Init(4999,71);  			// 5毫秒中断

	while(1)
	{	
		Get_MPU6050();				//获取陀螺仪数据
		Uart_DealDate();			//串口处理
		
		/*按键控制*/
		if(KEY_Scan(0) == KEY0_PRES)
		{
			ARMED = !ARMED;
		}	
	} 	
}



/**********
*角度
*调试时：前：+		后：-
*运行时：前：+		后：-
*
*
*编码器
*调试时：前：+		后：-
*运行时：前：+		后：-
*
****/

