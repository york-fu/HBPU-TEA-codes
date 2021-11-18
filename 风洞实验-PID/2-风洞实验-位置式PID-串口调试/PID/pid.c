#include "pid.h"

_PID pid;

void PID_Init()
{
	pid.expect= 20;		//期望值
	
	pid.Kp = 10;	//10
	pid.Ki = 0.1;	//0.1
	pid.Kd = 0;		//
}

double PID_Adjust(double actual)	//PID调节,位置式
{
	double value;
	static double error;			    //误差值（比例）
	static double error_change = 0;		//误差变化率（微分）
	static double error_last = 0;		//上次误差
	static double error_add = 0;		//累计误差值（积分）
	
	error = actual - pid.expect;	//计算本次误差
	
	error_add += error;		//计算积分
	if(error_add>20)  error_add=20;		//积分限幅
	if(error_add<-20)  error_add=-20;
	
	error_change  = error - error_last;		//计算微分
	
	value = pid.Kp*error + pid.Ki*error_add + pid.Kd*error_change;
	
	error_last = error;		//记录上次误差
	
	return value;
}
