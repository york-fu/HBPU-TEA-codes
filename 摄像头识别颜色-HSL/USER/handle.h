#ifndef _handle_h_
#define _handle_h_

#include "sys.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "main.h"

#define Variate 200

void Sinistrogyration(void);	//舵机左旋
void Dextroversion(void);		//舵机右旋
void Steering_Reset(void);		//舵机复位
void Conveyor_Advance(void);	//传送带向前
void Conveyor_Recede(void);		//传送带向后
void Conveyor_Stop(void);		//停止传送带
void Push_1(void);				//推向1区
void Push_2(void);				//推向2区
void Push_3(void);				//推向3区

#endif
