#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "inv_mpu.h"
#include "usart.h"
#include "motor.h"


/*PID参数*/
_PID Pid = {0,0,0};		//170,0,1.5


/******************************************************************
*函数功能:	PID调节，位置式
*函数参数:	Angle：实际角度
			AngularSpeed：实际角速度
			mode：积分模式
*返 回 值:	PWM波
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed,u8 mode)
{
	float value=0;
	static float Error=0;				//当前误差
	static float Integral=0;			//误差积分
	
	Error = *Angle - 0.0;				//计算误差
	
	if(mode) Integral += Error;
	else Integral = 0;
	
	/*积分限幅*/
	if(Integral>Integral_MAX) Integral = Integral_MAX;
	else if(Integral<-Integral_MAX) Integral = -Integral_MAX;
	
	value = Pid.Kp * Error + Pid.Ki * Integral + Pid.Kd * (float)(*AngularSpeed);
	
	/*输出限幅*/
	if(value>PWM_MAX)
	{
		value = PWM_MAX;
	}
	else if(value<-PWM_MAX)
	{
		value = -PWM_MAX;
	}
//	printf("%f\t%d\t%f\t",Error,*AngularSpeed,value);
	
	return (short)value;
}



/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	arr：自动重装值
			psc：分频系数
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM1, ENABLE);
}


u8 USART3_flag = 0;

/******************************************************************
*函数名称:	TIM1_UP_IRQHandler
*函数功能:	定时器1的更新中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 PwmValue = 4500;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源	
		
		if((USART3_flag&0x0f)!=0)		//收到指令
		{
			switch(USART3_flag&0x0f)
			{
				case 0x01:Motor_Control(PwmValue,PwmValue,FRONT);	//前进
					break;
				case 0x02:Motor_Control(PwmValue,PwmValue,REAR);	//后退
					break;
				case 0x03:Motor_Control(PwmValue,PwmValue,LEFT);	//左转
					break;
				case 0x04:Motor_Control(PwmValue,PwmValue,RIGHT);	//右转
					break;
				default:Motor_Control(0,0,STOP);
			}
			
			USART3_flag &= 0xf0;	//清除
		}
		else
		{
			Motor_Control(0,0,STOP);
		}
	}
}

