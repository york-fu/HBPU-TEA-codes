#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


extern u8 ov_frame;	//在timer.c里面定义


void TIM3_Int_Init(u16 arr,u16 psc); 
 
#endif
