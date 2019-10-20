#include "main.h"
	
u16 AdcValue=0;


/**********************************************************************
函数功能：按键调节
入口参数：无
返回参数：无
***********************************************************************/
void Key_Adjust()
{
	u8 KeyVal=0;
	
	KeyVal = KEY_Scan(1);
	if(KeyVal==KEY0_PRES)
	{
		TIM_Cmd(TIM1, DISABLE);
		PWM1 = 2000;
		PWM2 = 0;
		while(KeyVal==KEY0_PRES)		//松手检测
		{
			delay_ms(10);
			KeyVal = KEY_Scan(1);
		}
		
		PWM1 = 0;
		PWM2 = 0;
		TIM_Cmd(TIM1, ENABLE);
	}
	else if(KeyVal==KEY2_PRES)
	{
		TIM_Cmd(TIM1, DISABLE);
		PWM1 = 0;
		PWM2 = 2000;
		while(KeyVal==KEY2_PRES)		//松手检测
		{
			delay_ms(10);
			KeyVal = KEY_Scan(1);
		}
		PWM1 = 0;
		PWM2 = 0;
		TIM_Cmd(TIM1, ENABLE);
	}
}


/**********************************************************************
函数功能：主函数
入口参数：无
返回参数：无
***********************************************************************/
int main(void)
{
	
	delay_init();				//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(UAR1,115200);	 		//串口初始化为115200
	
	LED_Init();					//LED端口初始化
	KEY_Init();					//按键初始化 	
	LCD_Init();					//LCD初始化
 	tp_dev.init();				//触摸屏初始化
	Adc_Init();					//ADC初始化，PA1
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&AdcValue,64);	//初始化DMA
	ADC_DMACmd(ADC1,ENABLE);  	//使能DMA传输ADC的值
	
	Interface_Init();			//初始界面
	
	Tim4EncoderInit();			//编码器初始化
	TIM3_PWM_init(7199,0);		//定时器PWM波初始化，10KHz
	TIM1_IntInit(4999,71);		//初始化定时器1,用于10ms中断

	while(1)
	{
		Angle = Count_Angle(AdcValue);			//计算角度	
		TouchKey_Set();							//触摸按键
		Operation_Interface(AdcValue);			//操作界面
		Uart_DealDate();						//串口数据处理
		Key_Adjust();							//按键调节
		
		/*程序运行*/
		if(Run)
		{			
			
			switch(Key_Flag)
			{
				case 1:TIM_Cmd(TIM1, ENABLE);
					break;
				case 2:Procedure_2();
					break;
				case 3:
				case 4:
				case 5:
				case 6:TIM_Cmd(TIM1, ENABLE);
					break;
			}
		}
		else
		{
			TIM_Cmd(TIM1, DISABLE);
			IntTime = 0;
			PWM1 = 0;
			PWM2 = 0;
		}
	}
}

