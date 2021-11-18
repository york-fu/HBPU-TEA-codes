#ifndef PWM_H_
#define PWM_H_

#include "sys.h"

#define PWM1 TIM3->CCR1	//PA6
#define PWM2 TIM3->CCR2	//PA7

void TIM1_PWM_init(u16 arr, u16 psc);
void TIM2_PWM_init(u16 arr, u16 psc);
void TIM3_PWM_init(u16 arr, u16 psc);
void TIM4_PWM_init(u16 arr, u16 psc);
void TIM8_PWM_init(u16 arr, u16 psc);

/******************************************************************
*函数名称:	Tim4EncoderInit
*函数功能:	初始化定时器4,用于编码器
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Tim4EncoderInit(void);

#endif
