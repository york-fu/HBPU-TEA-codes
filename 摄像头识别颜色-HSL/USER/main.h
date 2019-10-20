#ifndef _main_h_
#define _main_h_

#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "display.h"
#include "discern.h"
#include "24l01.h"
#include "spi.h"
#include "ir_tube.h"
#include "handle.h"

#define BarColor DARKBLUE	//进度条颜色

extern u8 Conveyor_Run;	//传送带运行标记

void System_Init(void);
void Led_Test(u16 time);
void Interface(void);
void OV7670_init(void);
void NRF24L01_init(void);
u8 Sys_Start(void);
void Result_Handler(u8 dat);

#endif
