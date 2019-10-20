#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern float ADCX[4]; 
extern u16 THROTTLR;
extern u8  ARMED;

extern u8 Before_after;  	// 前后标志  
extern u8 Right_left;  		// 左右标志  
extern u8 Rotation;  		// 旋转标志 


void ADC_Gather(void);
void KEY_Control(void);
void NRF_SendData(void);

int myabs(int a);  // 绝对值函数


#endif
