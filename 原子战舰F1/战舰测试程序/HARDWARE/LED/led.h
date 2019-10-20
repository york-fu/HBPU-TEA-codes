#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



//LED端口定义
#define GPIO_L	do{\
				GPIOA->ODR=0X0000;\
				GPIOB->ODR=0X0000;\
				GPIOC->ODR=0X0000;\
				GPIOD->ODR=0X0000;\
				GPIOE->ODR=0X0000;\
				GPIOF->ODR=0X0000;\
				GPIOG->ODR=0X0000;\
				}while(0)
#define GPIO_H	do{\
				GPIOA->ODR=0Xffff;\
				GPIOB->ODR=0Xffff;\
				GPIOC->ODR=0Xffff;\
				GPIOD->ODR=0Xffff;\
				GPIOE->ODR=0Xffff;\
				GPIOF->ODR=0Xffff;\
				GPIOG->ODR=0Xffff;\
				}while(0)
				



void LED_Init(void);	//初始化		 				    
#endif

















