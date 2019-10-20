#ifndef _pid_h_
#define _pid_h_


typedef struct
{
	float actual_value;	//实际值
	float expect_value;	//期望值
	float error_value;	//误差值
	float Kp;			//比列系数
}_PID;

void PID_Init(void);

float PID_Adjust(float actual); 

#endif
