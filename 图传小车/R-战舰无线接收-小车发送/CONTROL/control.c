#include "control.h"
#include "pwm.h"
#include "delay.h"
#include "usart.h"
#include "key.h"



/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/*初始化定时器计数*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	/*初始化定时器中断*/
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);						//定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM1, ENABLE);
}


/******************************************************************
*函数名称:	TIM1_UP_IRQHandler
*函数功能:	定时器1的更新中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源					
		
//		printf("cmd!!\r\n");
		if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
		{
			if(KEY0==0)
			{
				USART_SendData(USART2,0x04);		//右转
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));			
			}
			else if(KEY1==0)
			{				
				USART_SendData(USART2,0x02);		//后退
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));		
			}
			else if(KEY2==0)
			{				
				USART_SendData(USART2,0x03);		//左转
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));		
			}
			else if(WK_UP==1)
			{				
				USART_SendData(USART2,0x01);		//前进
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));		
			}
//			printf("cmd ok\r\n");
		}
	}
}


