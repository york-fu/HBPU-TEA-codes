#ifndef _pid_h_
#define _pid_h_

typedef struct
{
	double expect;		//期望值
	double Kp;			//比列系数
	double Ki;			//积分系数
	double Kd;			//微分系数
}_PID;


extern _PID pid;

void PID_Init(void);

double PID_Adjust(double actual); 

#endif
