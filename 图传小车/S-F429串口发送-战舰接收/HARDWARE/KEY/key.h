#ifndef _KEY_H
#define _KEY_H
#include "sys.h"



//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY0        HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_10)	//KEY0按键
#define KEY1        HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_9)  //KEY1按键
#define WK_UP       HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_11)	//WKUP按键

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
