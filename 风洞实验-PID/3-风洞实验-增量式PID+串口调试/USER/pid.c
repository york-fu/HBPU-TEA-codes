#include "pid.h"
#include "lcd.h"
#include "usart.h"

_PID pid;

void PID_Init()
{
	pid.expect = 20;	//期望值
	
	pid.Kp = 0;		//12.3
	pid.Ki = 0;		//0.045
	pid.Kd = 0;		//0
}


double PID_Adjust(double actual)	//PID调节,增量式
{
	double value;
	static double error;			    //当前误差
	static double error_last1 = 0;		//上次误差
	static double error_last2 = 0;		//上上次误差
	static double error_change = 0;		//误差变化率（微分）
	
	error = actual - pid.expect;	//计算本次误差
	
	error_change = error - 2*error_last1 - error_last2;	//计算微分
	
	value = pid.Kp*(error - error_last1) + pid.Ki*error + pid.Kd*error_change;
	
	error_last2 = error_last1;		//记录上上次误差
	error_last1 = error;			//记录上次误差
	
	return value;
}

