#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

/*PWM波*/
#define PWM1 TIM8->CCR1		//PC6
#define PWM2 TIM8->CCR2		//PC7
#define PWM3 TIM8->CCR3		//PC8
#define PWM4 TIM8->CCR4		//PC9

/*小车运行模式*/
#define FRONT 1		//前
#define REAR  2		//后
#define LEFT  3		//左
#define RIGHT 4		//右
#define STOP  5		//停止

/******************************************************************
*函数名称: TIM8_PWM_init
*返 回 值: 无
*函数参数: arr:自动重装值
		   psc:自动预分频数
*函数功能: 初始化定时器8,用于PWM波(产生4路)
*******************************************************************/
void TIM8_PWM_init(u16 arr, u16 psc);

/**************************************************************************************
函数功能：	小车控制
入口参数：	LeftPwm：左边电机PWM值
			RightPwm：右边电机PWM值
			Mode：运行模式
返回参数：	无
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode);

#endif

