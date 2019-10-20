#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern u16 TIM2_Number;
extern u8 nvic_flog;

void TIM2_Capture_Init(u16 arr, u16 psc);

void TIM3_PWM_Init(u16 arr,u16 psc); 
 
#endif
