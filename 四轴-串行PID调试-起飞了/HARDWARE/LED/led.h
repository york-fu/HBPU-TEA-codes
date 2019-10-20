#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#define LED0 PBout(14)	
#define LED1 PBout(15)
#define SBQ  PBout(10)

void LED_Init(void);//初始化
void LED0_Flash(u16 time);//闪烁函数
void LED1_Flash(u16 time);//闪烁函数
		 				    
#endif
