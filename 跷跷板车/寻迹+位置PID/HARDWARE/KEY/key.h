#ifndef KEY_H_
#define KEY_H_

#include "sys.h"

#define KEY0 PCin(5)
#define KEY1 PBin(0)
#define WK_UP PBin(1)

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

/******************************************************************
*函数名称:	KEY_Init
*函数功能:	初始化按键
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void KEY_Init(void);

/******************************************************************
*函数名称:	KEY_Scan
*函数功能:	按键处理函数
*函数参数:	mode:0,不支持连续按;1,支持连续按;
*返 回 值:	返回按键值
			0，没有任何按键按下
			KEY0_PRES，KEY0按下
			KEY1_PRES，KEY1按下
			WKUP_PRES，WK_UP按下
*注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
*******************************************************************/
u8 KEY_Scan(u8 mode);

#endif
