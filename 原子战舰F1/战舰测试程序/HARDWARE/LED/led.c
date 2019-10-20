#include "led.h"


		    
//LED IO初始化
void LED_Init(void)
{	   	 
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟
	RCC->APB2ENR|=1<<5;    //使能PORTD时钟	   	 
	RCC->APB2ENR|=1<<6;    //使能PORTE时钟
	RCC->APB2ENR|=1<<7;    //使能PORTF时钟	   	 
	RCC->APB2ENR|=1<<8;    //使能PORTG时钟	
	   	 
	GPIOA->CRL = 0X33333333;//PA 推挽输出
	GPIOA->CRH = 0X33333333;//PA 推挽输出  
	GPIOB->CRL = 0X33333333;//PB 推挽输出 
	GPIOB->CRH = 0X33333333;//PB 推挽输出 
	GPIOC->CRL = 0X33333333;//PC 推挽输出
	GPIOC->CRH = 0X33333333;//PC 推挽输出  
	GPIOD->CRL = 0X33333333;//PD 推挽输出  
	GPIOD->CRH = 0X33333333;//PD 推挽输出  
	GPIOE->CRL = 0X33333333;//PE 推挽输出  
	GPIOE->CRH = 0X33333333;//PE 推挽输出  
	GPIOF->CRL = 0X33333333;//PF 推挽输出  
	GPIOF->CRH = 0X33333333;//PF 推挽输出  
	GPIOG->CRL = 0X33333333;//PG 推挽输出 
	GPIOG->CRH = 0X33333333;//PG 推挽输出 
}






