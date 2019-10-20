#include "encoder.h"

TIM_HandleTypeDef TIM3_Handler;
TIM_Encoder_InitTypeDef TIM3_Encoder_Init;

/***************************************************************************
函数功能：	定时器3编码器模式初始化
入口参数：	无
返回参数：	无
****************************************************************************/
void TIM3_EncoderInit(void)
{  
    TIM3_Handler.Instance=TIM3;								//定时器3
    TIM3_Handler.Init.Prescaler=0;							//分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;		//向上计数器
    TIM3_Handler.Init.Period=65535;							//自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	
	TIM3_Encoder_Init.EncoderMode = TIM_ENCODERMODE_TI12;
	TIM3_Encoder_Init.IC1Filter = 0;
	TIM3_Encoder_Init.IC1Prescaler = 0;
	TIM3_Encoder_Init.IC1Polarity = TIM_ICPOLARITY_RISING;
	TIM3_Encoder_Init.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	TIM3_Encoder_Init.IC2Filter = 0;
	TIM3_Encoder_Init.IC2Prescaler = 0;
	TIM3_Encoder_Init.IC2Polarity = TIM_ICPOLARITY_RISING;
	TIM3_Encoder_Init.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_Encoder_Init(&TIM3_Handler,&TIM3_Encoder_Init);
	
	HAL_TIM_Encoder_Start(&TIM3_Handler,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&TIM3_Handler,TIM_CHANNEL_2);
}



/***************************************************************************
函数功能：	定时器底层驱动，时钟使能，引脚配置
入口参数：	htim：定时器编码器初始化句柄
返回参数：	无
函数说明;	此函数会被HAL_TIM_Encoder_Init()调用
****************************************************************************/
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIO时钟
	
    GPIO_Initure.Pin=GPIO_PIN_6 | GPIO_PIN_7;	//pin
    GPIO_Initure.Mode=GPIO_MODE_INPUT;			//浮空输入
    GPIO_Initure.Pull=GPIO_NOPULL;				//无
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;		//复用为TIM
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


/******************************************************************
*函数名称:	Tim2EncoderInit
*函数功能:	初始化定时器2,用于编码器
*函数参数:	无
*返 回 值:	无
*******************************************************************/
/*
void Tim2EncoderInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIOB

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					//预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);			//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM2, 0);
	
	TIM_Cmd(TIM2, ENABLE);
}
*/

/******************************************************************
*函数名称:	Tim4EncoderInit
*函数功能:	初始化定时器4,用于编码器
*函数参数:	无
*返 回 值:	无
*******************************************************************/
/*
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
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//设定计数器自动重装值
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
*/

/******************************************************************
*函数名称:	ReadEncoderValue
*函数功能:	读取编码器的值
*函数参数:	timeValue:		定时器数(3和5)
*返 回 值:	encoderValue:	读取指定的定时器的CNT值
*******************************************************************/
u16 ReadEncoderValue(u8 timeValue)
{
	u16 encoderValue;
	
	switch (timeValue)
	{
		case 3:
			encoderValue = TIM3->CNT;
			TIM3->CNT = 0;
			break;
		case 5:
			encoderValue = TIM5->CNT;
			TIM5->CNT = 0;
			break;
		default:
			encoderValue = 0;
			break;
	}
	
	return encoderValue;
}

