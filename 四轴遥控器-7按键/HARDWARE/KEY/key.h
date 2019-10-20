#ifndef _key_h_
#define _key_h_

#include "sys.h"

sbit KEY0 = P3^5; 
sbit KEY1 = P3^6;
sbit KEY2 = P3^7;

sbit KEY3 = P2^2;
sbit KEY4 = P2^3;
sbit KEY5 = P2^0;
sbit KEY6 = P2^1;


#define KEY0_VALUE 1  
#define KEY1_VALUE 2
#define KEY2_VALUE 3

#define KEY3_VALUE 4
#define KEY4_VALUE 5
#define KEY5_VALUE 6
#define KEY6_VALUE 7
 
/*************************************************************************
函数功能：	按键扫描函数
入口参数：	mode:0,不支持连续按;1,支持连续按;
返回参数：	返回按键值
函数说明：	函数有响应优先级,KEY0>KEY1>KEY2>KEY3。。。
**************************************************************************/
u8 KEY_Scan(u8 mode);

#endif
