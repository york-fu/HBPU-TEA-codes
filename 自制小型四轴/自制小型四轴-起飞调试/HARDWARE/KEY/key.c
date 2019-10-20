#include "key.h"
#include "delay.h"

/******************************************************************
*函数名称:	KEY_Init
*函数功能:	初始化按键
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_7;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIOB 
}


u8 KEY_Scan()
{	 
	static u8 key_up=1;//按键按松开标志
	
	if(key_up && (READ_KEY0==0||READ_KEY1==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		
		if(READ_KEY0==0)		return KEY0_PRES;
		else if(READ_KEY1==0)	return KEY1_PRES;
	}
	else if(READ_KEY0==1&&READ_KEY1==1)
	{
		key_up=1;
	} 	    
 	return 0;// 无按键按下	
}
