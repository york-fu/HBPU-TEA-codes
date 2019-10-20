#ifndef _LED_H
#define _LED_H
#include "sys.h"


#define LED0 PIout(5)   //LED0
#define LED1 PIout(6)   //LED1

void LED_Init(void);
#endif
