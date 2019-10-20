#include "key.h"


/*************************************************************************
函数功能：	按键扫描函数
入口参数：	mode:0,不支持连续按;1,支持连续按;
返回参数：	返回按键值
函数说明：	函数有响应优先级,KEY0>KEY1>KEY2>KEY3。。。
**************************************************************************/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志

	if(mode) key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)		return KEY0_VALUE;
		else if(KEY1==0)return KEY1_VALUE;
		else if(KEY2==0)return KEY2_VALUE;
		else if(KEY3==0)return KEY3_VALUE; 
		else if(KEY4==0)return KEY4_VALUE;
		else if(KEY5==0)return KEY5_VALUE;
		else if(KEY6==0)return KEY6_VALUE;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1)
	{
		key_up=1; 
	}	    
 	return 0;// 无按键按下
}
