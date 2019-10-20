#include "sys.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "touch.h" 
#include "timer.h"
#include "my_gui.h"
#include "adc7606.h"


extern int16_t s_volt[8];

int main(void)
{	
	u8 i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	   //延时函数初始化	  
	uart_init(9600);	 	   //串口初始化为9600
 	LED_Init();			       //LED端口初始化
	LCD_Init();	               //LCD屏初始化
	KEY_Init();	 			   //按键初始化
 	tp_dev.init();			   //触摸屏初始化 
	AD7606_Init();			   //AD7606模块初始化	

	Basic_Interface();        //基本界面
	LCD_Keyboard();           //键盘
	
	while(1)
	{ 
		LCD_Touch();  	//触摸屏
		LCD_Input();  	//按键输入
		if(i>=20)
		{
			AD7606_Scan();			//读取AD转换器数据
			AD7606_Count();			//处理采样后的数据
			i = 0;
		}
		LCD_Count(s_volt[0]);	//输入电压值计算质量
		i++;
		delay_ms(5);
	}
}



