#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

#define PWM1	TIM2->CCR1	//PA0
#define PWM2	TIM2->CCR2	//PA1
#define PWM3	TIM2->CCR3	//PA2
#define PWM4	TIM2->CCR4	//PA3


/******************************************************************
*函数名称:	MotorPwmInit
*函数功能:	初始化用于电机的PWM波
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void MotorPWMInit(u16 arr,u16 psc);

#endif
