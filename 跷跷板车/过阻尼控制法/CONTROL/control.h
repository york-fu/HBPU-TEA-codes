#ifndef CONTROL_H_
#define CONTROL_H_

#include "sys.h"


#define PWM_MAX 500

typedef struct
{
	float Kp;				//比列系数
	float Ki;				//积分系数
	float Kd;				//微分系数
}_PID;


extern u8 Run;
extern u8 Time;
extern u16 PWMvalue;

extern _PID Pid;


/******************************************************************
*函数功能:	PID调节，增量式
*函数参数:	Angle：实际角度
			AngularSpeed：实际角速度
*返 回 值:	PWM波
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed);

/******************************************************************
*函数功能:	PWM波控制
*函数参数:	angle：角度值
*返 回 值:	无
*******************************************************************/
void PWM_Control(float angle);

/******************************************************************
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);

#endif
