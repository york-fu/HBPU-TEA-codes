#include "motor.h"


/******************************************************************
*函数名称: TIM2_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器2,用于PWM波(产生4路)
*******************************************************************/
void TIM2_PWM_init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//TIM2时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA
	
	//设置该引脚为复用输出功能,输出TIM2-CH1,CH2,CH3,CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	//CH1,CH2,CH3,CH4	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;		//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);	//使能TIMx在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	//使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);	//使能TIMx在CCR3上的预装载寄存器
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);	//使能TIMx在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);		//使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);					//使能TIMx外设
}



/**************************************************************************************
函数功能：	小车控制
入口参数：	LeftPwm：左边电机PWM值
			RightPwm：右边电机PWM值
			Mode：运行模式
返回参数：	无
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode)
{
	static u16 TURNPWM = 1000;
	
	switch(Mode)
	{
		/*前进*/
		case FRONT:
				PWM1 = LeftPwm;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = RightPwm;		
			break;
		
		/*后退*/
		case REAR:	
				PWM1 = 0;
				PWM2 = LeftPwm;
				PWM3 = RightPwm;
				PWM4 = 0;
			break;
		
		/*左转*/
		case LEFT:
				PWM1 = 0;
				PWM2 = TURNPWM;
				PWM3 = 0;
				PWM4 = TURNPWM;
			break;
		
		/*右转*/
		case RIGHT:
				PWM1 = TURNPWM;
				PWM2 = 0;
				PWM3 = TURNPWM;
				PWM4 = 0;
			break;
		
		/*停止*/
		case STOP:
				PWM1 = 0;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = 0;
			break;
		
		default:PWM1 = 0;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = 0;
	}
}


