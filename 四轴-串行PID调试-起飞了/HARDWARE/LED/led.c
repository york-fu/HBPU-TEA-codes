#include "led.h"



//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_10;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz    		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  			
	GPIO_SetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_10); 						 
}
 

/**************************************************************************
函数功能：LED闪烁
入口参数：闪烁频率 
返回  值：无
**************************************************************************/
void LED0_Flash(u16 time)
{
	static int temp;
	if(++temp==time)
		LED0=~LED0,temp=0;
}
void LED1_Flash(u16 time)
{
	static int temp;
	if(++temp==time)
		LED1=~LED1,temp=0;
}
