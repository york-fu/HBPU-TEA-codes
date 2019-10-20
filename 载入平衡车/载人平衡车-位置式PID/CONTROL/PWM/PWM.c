#include "pwm.h"

void Balance_PWM_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB1ENR|=1<<2;       //TIM4时钟使能    
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能     
	GPIOB->CRL&=0X0FFFFFFF;   //PORTB 7复用输出
	GPIOB->CRL|=0XB0000000;   //PORTB 7复用输出   
	GPIOB->CRH&=0XFFFF00F0;   //PORTB 8复用输出,10,11推挽输出
	GPIOB->CRH|=0X0000220B;   //PORTB 8复用输出,10,11推挽输出
	
	TIM4->ARR=arr;//设定计数器自动重装值 
	TIM4->PSC=psc;//预分频器不分频
	
	TIM4->CCMR1|=6<<12;	//CH2 PWM2模式		
	TIM4->CCMR1|=1<<11;	//CH2预装载使能	  
	TIM4->CCER|=1<<4; 	//CH2输出使能	
	TIM4->CCMR2|=6<<4;	//CH3 PWM2模式		
	TIM4->CCMR2|=1<<3;	//CH3预装载使能	  
	TIM4->CCER|=1<<8; 	//CH3输出使能
  	
	TIM4->CR1=0x0080;  	//ARPE使能 					///////////这句话有点疑惑，不确定对不对
	TIM4->CR1|=0x01;   	//使能定时器3 

	
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

/*********************************************************
*函数：方向IO口配置函数
*输入：
*功能：
*输出：
**********************************************************/
void MOTO_DIR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

/*********************************************************
*函数：PWM输出初始化, 定时器3 CH1，CH2，pwm输出
*输入：自动重装载数，分频数//arr：自动重装值, psc：时钟预分频
*功能：定时器2通道1，2配置为编码器模式
*输出: 无
**********************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CH1, TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM2->CCR1 =  0;
	TIM2->CCR2 =  0;
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2 
   
}

