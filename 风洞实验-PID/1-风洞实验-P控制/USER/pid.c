#include "pid.h"

_PID pid;

void PID_Init()
{
	pid.actual_value=0;		//实际值
	pid.expect_value=20;	//期望值
	pid.error_value=0;		//误差值
	pid.Kp=0.5;
}

float PID_Adjust(float actual)	//PID调节
{
	float value;
	
	PID_Init();
	
	pid.actual_value = actual;
	pid.error_value = pid.actual_value - pid.expect_value;	//计算误差
	value = pid.Kp*pid.error_value;
	
	return value;
}
