#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern uint32_t Systick_Count;

#define GetSystick() Systick_Count

void TIME_Init(u16 psc,u16 arr);
 
#endif
