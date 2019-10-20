#include<reg52.h>
#include "nrf24l01.h"

sbit K1=P3^4;
sbit LED0=P0^0;	
sbit LED1=P0^1;

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
	unsigned char Txbuf[2]={1,1};
	unsigned char x;

	Nrf24l01Init();
	
	while(1)
	{	
		while(CheckNrf24l01())
		{
			LED0=!LED0;
			delay_ms(100);	
		}
		LED0=1;

		if(K1==0)
		 {
			delay_ms(5);
			if(K1==0)
			{
				Nrf24l01SetTXMode();
				Nrf24l01TXPacket(Txbuf);	
			}
			while(!K1);
		}

		if(x%100==0) LED1=!LED1;

		x++;
		if(x>199)  x=0;
		delay_ms(1);
	}	
}