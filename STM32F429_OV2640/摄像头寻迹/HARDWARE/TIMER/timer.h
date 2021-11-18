#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"

#define Integral_MAX 10000
#define PWM_MAX 5000

typedef struct
{
	float Kp;				//比列系数
	float Ki;				//积分系数
	float Kd;				//微分系数
}_PID;

extern u8 Run;
extern u8 Center_flag;
extern u16 MidlinePos;

/***************************************************************************
函数功能：	定时器中断初始化
入口参数：	arr：自动重装值
			psc：时钟预分频数
返回参数：	无
函数说明;	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
			Ft=定时器工作频率,单位:Mhz
			定时器1挂在APB2上，时钟为APB2*2=180MHz
****************************************************************************/
void TIM1_Init(u16 arr,u16 psc);

#endif

