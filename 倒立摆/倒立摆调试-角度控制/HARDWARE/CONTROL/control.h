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


extern u8 Run;
extern short AimAngle;
extern u16 IntTime;
extern float Angle;

extern PID upPid;
extern PID SpeedPid;


/**********************************************************************
函数功能：根据ADC的值计算角度
入口参数：ADC的值
返回参数：角度
***********************************************************************/
float Count_Angle(u16 value);

/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);

short Get_EncoderValue(void);

short Upright_PID(short AimAngle,float ActualAngle,u8 Mode);

short Speed_PID(short EncoderValue,u8 Mode);

void Procedure_1(void);
void Procedure_2(void);
void Procedure_3(void);

#endif
