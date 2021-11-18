#include "pwm.h"

/******************************************************************
*函数名称: TIM1_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器1,用于PWM波(产生7路波)
*******************************************************************/
void TIM1_PWM_init(u16 arr, u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//TIM1使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB

	//设置该引脚为复用输出功能,输出TIM1-CH1,CH2,CH3,CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;	//CH1,CH2,CH3,CH4	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//CH1N,CH2N,CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;		//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	//比较N输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;							 	//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//N输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);	//CH1预装载使能
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	//CH2预装载使能
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	//CH3预装载使能
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);	//CH4预装载使能
	
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;	//自动输出功能使能
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;					//失能TIM1刹车输入
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;		//TIM1刹车输入管脚极性高 
	TIM_BDTRInitStructure.TIM_DeadTime = 28;								//输出打开和关闭状态之间的延时,死区时间
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;				//不锁任何位
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			//失能TIM1 OSSI状态
   	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSRState_Disable;			//失能TIM1 OSSR状态
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);					//MOE主输出使能		 
	TIM_ARRPreloadConfig(TIM1, ENABLE);					//使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);								//使能TIM1  
}

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

/******************************************************************
*函数名称: TIM3_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器3,用于PWM波
*******************************************************************/
void TIM3_PWM_init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//TIM3时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA,GPIOB,AFIO复用时钟
	
	//设置该引脚为复用输出功能,输出TIM3-CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//CH1,CH2	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);	//使能TIMx在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	//使能TIMx在CCR1上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);		//使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM3, ENABLE);					//使能TIMx外设
}

/******************************************************************
*函数名称: TIM4_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器4,用于PWM波(产生4路)
*******************************************************************/
void TIM4_PWM_init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//TIM2时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB
	
	//设置该引脚为复用输出功能,输出TIM4-CH1,CH2,CH3,CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//CH1,CH2,CH3,CH4	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;		//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	//使能TIMx在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	//使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	//使能TIMx在CCR3上的预装载寄存器
	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	//使能TIMx在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);		//使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM4, ENABLE);					//使能TIMx外设
}

/******************************************************************
*函数名称: TIM8_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器8,用于PWM波(产生7路波)
*******************************************************************/
void TIM8_PWM_init(u16 arr, u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//TIM8使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC

	//设置该引脚为复用输出功能,输出TIM8-CH1,CH2,CH3,CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//CH1,CH2,CH3,CH4	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			//CH1N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//CH2N,CH3N	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;		//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	//比较N输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//N输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);			//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);	//CH1预装载使能
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	//CH2预装载使能
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);	//CH3预装载使能
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);	//CH4预装载使能
	
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;	//自动输出功能使能
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;					//失能TIM1刹车输入
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;		//TIM1刹车输入管脚极性高 
	TIM_BDTRInitStructure.TIM_DeadTime = 28;								//输出打开和关闭状态之间的延时,死区时间
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;				//不锁任何位
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;			//失能TIM OSSI状态
   	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSRState_Disable;			//失能TIM OSSR状态
	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	TIM_CtrlPWMOutputs(TIM8, ENABLE);					//MOE主输出使能		 
	TIM_ARRPreloadConfig(TIM8, ENABLE);					//使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM8, ENABLE);								//使能TIM1  
}



/******************************************************************
*函数名称:	Tim4EncoderInit
*函数功能:	初始化定时器4,用于编码器
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Tim4EncoderInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//根据设定参数初始化GPIOB

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;					// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 65535;					//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);			//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM4, 0);
	
	TIM_Cmd(TIM4, ENABLE);
}

