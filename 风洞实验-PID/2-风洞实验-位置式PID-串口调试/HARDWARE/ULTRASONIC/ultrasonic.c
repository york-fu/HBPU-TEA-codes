#include "ultrasonic.h"
#include "lcd.h"
#include "timer.h"

u8 nvic_flog = 0;

void Ultrasonic_Init()	//超声波初始化，PA4做触发引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_Ultrasonic_ECHO|RCC_Ultrasonic_TRIG, ENABLE);	//使能IO口时钟
	
	GPIO_InitStructure.GPIO_Pin = PIN_Ultrasonic_TRIG;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_Ultrasonic_TRIG, &GPIO_InitStructure);		//IO口初始化
	
	GPIO_ResetBits(GPIO_Ultrasonic_TRIG, PIN_Ultrasonic_TRIG);	//置低
	
	GPIO_InitStructure.GPIO_Pin = PIN_Ultrasonic_ECHO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			//输入下拉
	GPIO_Init(GPIO_Ultrasonic_ECHO, &GPIO_InitStructure);	//IO口初始化
}




float Ultrasonic_Ranging()		//超声波测距
{	
	u16 i;
	u16 TIM2_Number=0;
	float value;
	
	TRIG = 0;
	
	/*触发时序*/
	TRIG = 1;
	delay_us(12);
	TRIG = 0;
	
	while(!ECHO)
	{
		i++;
		if(i>65500)
		{
			i = 0;
			return -1;
		}
	}
	TIM_Cmd(TIM2,ENABLE); 	//使能定时器2
	
	while(ECHO)
	{
		if(nvic_flog==2)
		{
			nvic_flog=0;
			return -1;
		}		
	}
	TIM2_Number = TIM_GetCounter(TIM2);
	TIM_Cmd(TIM2,DISABLE);		//失能能定时器2
	TIM_SetCounter(TIM2,0);

//	LCD_ShowxNum(20,150,TIM2_Number,6,16,0X80);
	
	value = (float)TIM2_Number*0.017;	//计算距离，单位cm
	
	return value;
}




