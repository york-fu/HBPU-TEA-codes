#include "timer.h"
#include "led.h"
#include "display.h"

extern TimeStruct Time;
extern u8 update_flog;

/*********************************************************************
函数名称：TIM3_Int_Init			interrupt
函数功能：TIM3定时器中断配置
输入参数：arr:自动重装载值
		  psc:预分频系数
返回参数：无
*********************************************************************/
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;					//定时器初始化结构体
	NVIC_InitTypeDef NVIC_InitStrue;								//中断初始化结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//使能定时器3时钟APB1
	
	TIM_TimeBaseInitStrue.TIM_Period = arr;							//自动重装载值，Period---周期
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;						//预分频系数
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up;		//计数模式，向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;			//时钟分频，1分频
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStrue);					//TIM3定时器初始化
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);						//TIM3定时器中断初始化
	
	NVIC_InitStrue.NVIC_IRQChannel = TIM3_IRQn;						//定时器3中断
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;						//使能中断
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 0;			//抢占优先级0
	NVIC_InitStrue.NVIC_IRQChannelSubPriority = 3;					//响应优先级3
	
	NVIC_Init(&NVIC_InitStrue);										//中断初始化
	
	TIM_Cmd(TIM3, ENABLE);											//TIM3定时器使能
}



/*********************************************************************
函数名称：TIM3_IRQHandler
函数功能：TIM3定时器中断服务函数
输入参数：无
返回参数：无
*********************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)!=RESET)		//判断中断类型是否为TIM3更新中断
	{	
		Time.sec++;
		update_flog = 1;	//时间更新标志
	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//清除TIM3中断标志
	}
}
