#include "sys.h"

/*12MHz时钟延时函数*/
#define Delay_12MHz

#ifdef Delay_12MHz
/*************************************************************************
函数功能：	延时函数，单位（us）
入口参数：	t：延时时间（t*10us）
返回参数：	无
**************************************************************************/
void delay_10us(u16 t)
{ 
    unsigned char a,b;

	for(;t>0;t--)
	{
	    for(b=1;b>0;b--)
		{
	        for(a=1;a>0;a--);
		}
	}
}

/*************************************************************************
函数功能：	延时函数，单位（ms）
入口参数：	t：延时时间（t*1ms）
返回参数：	无
**************************************************************************/
void delay_ms(u16 t)
{
	unsigned char a,b;

	for(;t>0;t--)
	{
	    for(b=199;b>0;b--)
		{
	        for(a=1;a>0;a--);
		}
	}
}

#else

//参数：ms，要延时的ms数，自动适应主时钟
#define MAIN_Fosc        SysClock    //定义主时钟HZ

void delay_ms(u16 ms)    //u16 等价于 unsigned int
{
    u16 i;
	do{
        i=MAIN_Fosc/9600;
        while(--i);     //96T per loop
    }while(--ms);       //--ms  ms=ms-1
}


#endif