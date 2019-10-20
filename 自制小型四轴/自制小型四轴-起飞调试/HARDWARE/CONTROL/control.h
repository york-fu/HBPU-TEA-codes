#ifndef CONTROL_H_
#define CONTROL_H_

#include "sys.h"

#define THRESHOLD		3000	//油门阀值
#define MAX_INTEGRAL	1000	//积分限幅最大值
#define X_PWM_MAX		7200	//X轴PWM波限幅
#define Y_PWM_MAX		7200	//Y轴PWM波限幅
#define Z_PWM_MAX		4000	//Z轴PWM波限幅

extern u8 LOCK_FLAG;			//锁定标志

extern u16 Accelerator;	//油门值

/*PID参数*/
typedef struct
{
	float P;
	float I;
	float D;
}PID;


/*陀螺仪数据*/
typedef struct
{
	float pitch;
	float roll;
	float yaw;
	short X_AngularSpeed;
	short Y_AngularSpeed;
	short Z_AngularSpeed;
}MPUDATA;


/*X,Y,Z数据*/
typedef struct
{
	float X;
	float Y;
	float Z;
}XYZ_DATA;

extern PID Pid_X;
extern PID Pid_Y;
extern PID Pid_Z;

/******************************************************************
*函数名称:	CheckState
*函数功能:	检测四轴的状态,如果俯仰角绝对值大于40度,停止电机
*函数参数:	pitch:	俯仰角;roll: 横滚角
*返 回 值:	0:		状态正常
*			其他:	状态异常
*******************************************************************/
u8 CheckState(float pitch, float roll);


/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void PID_Control(MPUDATA MpuData, MPUDATA MpuAngularSpeed);


/******************************************************************
*函数名称:	Descend
*函数功能:	智能下降函数，防摔
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Descend(void);


/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);

#endif
