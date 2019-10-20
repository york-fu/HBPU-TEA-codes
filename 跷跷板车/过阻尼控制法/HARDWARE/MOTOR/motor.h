#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

#define PWM1 TIM2->CCR1		//PA0
#define PWM2 TIM2->CCR2		//PA1
#define PWM3 TIM2->CCR3		//PA2
#define PWM4 TIM2->CCR4		//PA3

#define FRONT 1		//前
#define REAR  2		//后
#define LEFT  3		//左
#define RIGHT 4		//右
#define STOP  5		//停止

/******************************************************************
*函数名称: TIM2_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器2,用于PWM波(产生4路)
*******************************************************************/
void TIM2_PWM_init(u16 arr, u16 psc);

/**************************************************************************************
函数功能：	小车控制
入口参数：	LeftPwm：左边电机PWM值
			RightPwm：右边电机PWM值
			Mode：运行模式
返回参数：	无
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode);

#endif

