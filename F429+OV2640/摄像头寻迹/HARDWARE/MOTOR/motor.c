#include "motor.h"


TIM_HandleTypeDef TIM2_Handler;         //定时器PWM句柄 
TIM_OC_InitTypeDef TIM2_CHAllHandler;	//定时器通道句柄

/***************************************************************************
函数功能：	TIM，PWM初始化(产生4路)
入口参数：	arr：自动重装值
			psc：时钟预分频数
返回参数：	无
****************************************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{ 
    TIM2_Handler.Instance=TIM2;            //定时器
    TIM2_Handler.Init.Prescaler=psc;       //定时器分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM2_Handler.Init.Period=arr;          //自动重装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM2_Handler);       //初始化PWM
    
    TIM2_CHAllHandler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM2_CHAllHandler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM2_CHAllHandler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_1);//配置TIM通道
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_2);//配置TIM通道
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_3);//配置TIM通道
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_4);//配置TIM通道
	
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);//开启PWM通道
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);//开启PWM通道
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);//开启PWM通道
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_4);//开启PWM通道
	
//	HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_ALL);//???
}


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM2_CLK_ENABLE();			//使能定时器
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;	//pin
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;			//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;				//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//高速
	GPIO_Initure.Alternate= GPIO_AF1_TIM2;		//复用为TIM
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
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
	switch(Mode)
	{
		/*前进*/
		case FRONT:	
				PWM1 = LeftPwm;
				PWM2 = 0;
				PWM3 = RightPwm;
				PWM4 = 0;
			break;
		
		/*后退*/
		case REAR:	
				PWM1 = 0;
				PWM2 = LeftPwm;
				PWM3 = 0;
				PWM4 = RightPwm;
			break;
		
		/*左转*/
		case LEFT:	
				PWM1 = 0;
				PWM2 = LeftPwm+1000;
				PWM3 = RightPwm+1000;
				PWM4 = 0;
			break;
		
		/*右转*/
		case RIGHT:
				PWM1 = LeftPwm+1000;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = RightPwm+1000;
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


