#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "sys.h"


#define PWM_MAX_VALUE	6500	
#define MAX_INTEGRAL	10000	

typedef struct Pid
{
	float Kp;
	float Ki;
	float Kd;
} PID;


/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);


#endif
