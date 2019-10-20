#ifndef _KEY_H
#define _KEY_H
#include "sys.h"


#define KEY0        PIin(10)	//KEY0°´¼ü
#define KEY1        PIin(9)		//KEY1°´¼ü
#define WK_UP       PIin(11)	//WKUP°´¼ü

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3


void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
