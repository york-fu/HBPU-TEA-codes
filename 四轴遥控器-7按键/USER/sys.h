#ifndef _sys_h_
#define _sys_h_

#include<reg52.h>
#include "stdio.h"

/*用户定义*/
sbit LED0 = P0^0; 
sbit LED1 = P0^1;

/*数据类型定义*/
typedef unsigned char		u8;
typedef unsigned int		u16;
typedef xdata unsigned char xu8;
typedef xdata unsigned int	xu16;

typedef signed char			s8;
typedef signed int			s16;

/********************系统时钟设置********************/	  
/*系统输入时钟*/
#define SysClock 11059200UL	 
//#define SysClock 12000000UL
/*系统分频系数*/
#define SysPsc 12


/*************************************************************************
函数功能：	延时函数，单位（us）
入口参数：	t：延时时间（t*10us）
返回参数：	无
**************************************************************************/
void delay_10us(u16 t);

/*************************************************************************
函数功能：	延时函数，单位（ms）
入口参数：	t：延时时间（t*1ms）
返回参数：	无
**************************************************************************/
void delay_ms(u16 ms);


#endif