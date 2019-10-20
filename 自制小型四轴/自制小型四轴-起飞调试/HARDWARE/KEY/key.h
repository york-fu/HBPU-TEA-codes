#ifndef KEY_H_
#define KEY_H_

#include "sys.h"

#define KEY0 PBin(9)
#define KEY1 PBin(7)

#define READ_KEY0	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//读取按键0
#define READ_KEY1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//读取按键1 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下

/******************************************************************
*函数名称:	KEY_Init
*函数功能:	初始化按键
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void KEY_Init(void);

u8 KEY_Scan(void);

#endif
