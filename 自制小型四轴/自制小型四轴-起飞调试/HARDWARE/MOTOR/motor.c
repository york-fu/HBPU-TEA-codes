#include "motor.h"


/******************************************************************
*函数名称:	MotorPwmInit
*函数功能:	初始化用于电机的PWM波
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void MotorPWMInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//使能GPIO外设时钟使能
	
	/*初始化定时器IO口*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	//CH1,CH2,CH3,CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*初始化定时器计数*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	/*初始化定时器PWM波*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);						//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);						//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);						//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);						//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_CtrlPWMOutputs(TIM2, ENABLE);					//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH1预装载使能	 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH2预装载使能	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH3预装载使能	 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH4预装载使能	 

	TIM_ARRPreloadConfig(TIM2, ENABLE);					//使能TIMx在ARR上的预装载寄存器

	TIM_Cmd(TIM2, ENABLE);								//使能定时器
}
