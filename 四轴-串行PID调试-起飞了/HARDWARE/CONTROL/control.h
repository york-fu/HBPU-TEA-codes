#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern u8  ARMED;		// 电机是否正常标志位
extern u16 THROTTLE;	// 油门控制量
extern u8  TIME_flag;	// 时间标志


//PID 参数
typedef struct
{
	float P;
	float I;
	float D;
}PID;

//XYZ 方向数据
typedef struct
{
	float x;
	float y;
	float z;
}SITE;

//PID 输出数据
typedef struct
{
	float pout;
	float iout;
	float dout;
	s16   OUT;
}MOTOR;

/*---- 外环 P, I, D ----*/
extern PID OUTER_X, OUTER_Y, OUTER_Z;
/*---- 内环 P, I, D ----*/
extern PID INNER_X, INNER_Y, INNER_Z;

extern SITE Angle;				// 欧拉角
extern SITE Expected_Angle;		// 期望欧拉角
extern SITE Angle_Integral;		// 欧拉角积分 
extern SITE Palstance;			// 角速度
extern SITE Expected_Palstance;	// 期望角速度
extern SITE Palstance_Integral;	// 角速度积分 

void Moto_PwmRflash(u16 m1, u16 m2, u16 m3, u16 m4);			// 更新占空比
void Run_Off(void);												// 关闭电机
void Integral_Reset(void);										// 积分清零
void OUTER_CONTROL(SITE angle, SITE expected_angle);			// 外环控制
void INNER_CONTROL(SITE palstance, SITE expected_palstance);  	// 内环控制

#endif
