#ifndef LED_H_
#define LED_H_

#include "sys.h"

#define LED0 PBout(8)	//PB8
#define LED1 PBout(9)	//PB9

#define Bluetooth_P PBin(15)	//蓝牙端口

/******************************************************************
*函数名称:	LED_Init
*函数功能:	初始化LED
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void LED_Init(void);

/*蓝牙端口初始化*/
void BluetoothIO_init(void);

#endif
