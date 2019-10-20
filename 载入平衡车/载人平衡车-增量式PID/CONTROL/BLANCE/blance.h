#ifndef _BLANCE_H_
#define _BLANCE_H_

#include "sys.h"

#define PWM_L   TIM2->CCR1		//PA0
#define DIR_L   PBout(11)

#define PWM_R   TIM2->CCR2		//PA1
#define DIR_R   PBout(10)

typedef struct 
{
	float P;
	float I;
	float D;
	
}PID;

extern u8 ARMED;		//锁定解锁标志位
extern PID BlancePid;	//平衡环pid
extern u8 Accelerator;	//速度档位：1，2，3


#endif
