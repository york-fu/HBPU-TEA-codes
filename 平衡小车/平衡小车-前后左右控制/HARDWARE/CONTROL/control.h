#ifndef CONTROL_H_
#define CONTROL_H_

#include "sys.h"

#define PWM_MAX_VALUE	6000	//PWM波限幅,最大为7200
#define MAX_INTEGRAL	3500	//积分限幅最大值
#define	MAX_PI			5500	//PI速度环输出PWM的最大值

/************************SpeedPi()函数专用************************/
#define NORMAL_MODE		0		//正常积分
#define EMPTY_INTEGRAL	1		//清空积分
/*****************************************************************/

/**********************SaveAndRead()函数专用**********************/
#define SAVE_DATA		0		//保存数据
#define READ_DATA		1		//读取数据
#define EMPTY_DATA		2		//清空数据
/*****************************************************************/

typedef struct Pid
{
	float kp;
	float ki;
	float kd;
} PID;

extern PID upPD;		//直立PD参数
extern PID velocityPI;	//速度PI参数
extern s8 LeftSteer;
extern s8 RightSteer;
extern s8 Steer;
extern s16 TurnValue;

/******************************************************************
*函数名称:	KeyStartAndStop
*函数功能:	按键用于开启和关闭小车
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void KeyStartAndStop(void);

/***********************************************************************
*函数名称:	SaveAndRead
*函数功能:	用于保存和读取需要的数据(俯仰角,Y轴角速度,左边编码器的数,
*			右边编码器的数),避免了全局变量的适用,降低程序耦合度
*函数参数:	pitch:				指向俯仰角的指针
*			yAngularSpeed:		指向Y轴角速度的指针
*			encoderLeftNum:		指向左边编码器的数的指针
*			encoderRightNum:	指向右边编码器的数的指针
*			mode:				模式(SAVE_DATA为保存数据,
*								READ_DATA为读取数据,
*								EMPTY_DATA为清空数据)
*返 回 值:	无
************************************************************************/
void SaveAndRead(float *pitch, short *yAngularSpeed, short *encoderLeftNum, short *encoderRightNum, u8 mode);

/******************************************************************
*函数名称:	Tim3TimeIntInit
*函数功能:	初始化定时器3,用于5ms中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Tim1TimeIntInit(void);

#endif
