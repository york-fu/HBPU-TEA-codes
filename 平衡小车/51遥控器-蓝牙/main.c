#include<reg52.h>
#include "usart.h"


void delay_ms(int t)
{
	int i;

	for(;t>0;t--)
	{
		for(i=110;i>0;i--);
	}
}

void main()
{
	int x=0;

	USART_Init();
	
	while(1)
	{
		USART_Send();
		
		if(x%500==0)	LED0 = !LED0;
		x++;
		if(x>1999)  x=0;
		delay_ms(1);
	}	
}
