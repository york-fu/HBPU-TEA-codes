#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern u8 TIM3CH3_CAPTURE_STA;	//输入捕获状态		    				
extern u16 TIM3CH3_CAPTURE_VAL;	//输入捕获值

void TIM1_Init(u16 arr,u16 psc);
void TIM3_Cap_Init(u16 arr,u16 psc);

#endif
