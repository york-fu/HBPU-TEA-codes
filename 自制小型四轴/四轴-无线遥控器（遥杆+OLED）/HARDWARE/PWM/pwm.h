#ifndef __PWM_H
#define __PWM_H
#include "sys.h"


#define PWM1   TIM2->CCR1
#define PWM2   TIM2->CCR2
#define PWM3   TIM2->CCR3
#define PWM4   TIM2->CCR4




void TIM2_Init(u16 arr,u16 psc);
void Motor_SelfCheck(void);

#endif
