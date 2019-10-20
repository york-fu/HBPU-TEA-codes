#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

extern u16 ADCX_NORM[4];  //电压标准值

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
