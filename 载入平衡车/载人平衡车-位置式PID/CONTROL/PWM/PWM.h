#ifndef _PWM_H_
#define _PWM_H_

#include "sys.h"

void MOTO_DIR_Init(void);					//方向IO口初始化
void TIM2_PWM_Init(u16 arr,u16 psc);		//pwm初始化
void Balance_PWM_Init(u16 arr,u16 psc);		//pwm、io口方向初始化

#endif
