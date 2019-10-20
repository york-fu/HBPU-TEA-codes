#include "wkup.h"
#include "delay.h"
#include "key.h"


	 
void Sys_Sta(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
	PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能
	PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 		 
}
//系统进入待机模式
void Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	Sys_Sta();
}

void WK_UPStandby(void)
{
	if(WK_UP)
	{	
		delay_ms(10);
		if(WK_UP)
		{
			while(WK_UP);
			delay_ms(500);
			Enter_Standby();
		}
	}
}















