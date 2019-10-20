#include "timer.h"
#include "led.h"


//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIO外设
 
   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			//TIM3_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; 						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  						//根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 			 	//使能TIM3在CCR1上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}

/*********************************************************************
函数名称：TIM2_Capture_Init
函数功能：TIM2定时器捕获配置
输入参数：arr:自动重装载值
		  psc:预分频系数
返回参数：无
*********************************************************************/


void TIM2_Capture_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//定时器初始化结构体
 	NVIC_InitTypeDef NVIC_InitStructure;				//中断初始化结构体
	TIM_ICInitTypeDef TIM2_ICInitStructure;				//TIM2输入捕获结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
	
	/*初始化定时器3*/	 
	TIM_TimeBaseStructure.TIM_Period = arr; 					//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//初始化TIM2
  
	/*初始化TIM2输入捕获参数*/
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; 					//CC1S=01,IC1映射到TI1上
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿捕获
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//配置输入分频,不分频 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;							//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	/*中断分组初始化*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							//初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);			//允许更新中断 ,允许CC2IE捕获中断	
	
//	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
}



/*********************************************************************
函数名称：TIM2_IRQHandler
函数功能：TIM2定时器中断服务函数
输入参数：无
返回参数：无
*********************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2, TIM_IT_CC2)!=RESET && (nvic_flog==0))	//捕获中断且捕获到上升沿,此时模块发出超声波
	{
		TIM_SetCounter(TIM2, 0);		//设置定时器计数
		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//设置下降沿捕获
		nvic_flog = 1;		//标记捕获到高电平
	
	}
	
	else if(TIM_GetFlagStatus(TIM2, TIM_IT_CC2)!=RESET && (nvic_flog==1))	//捕获中断且捕获到下降沿，此时模块接收到超声波
	{		
		TIM2_Number = TIM_GetCapture2(TIM2);	//获取定时器数值
		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);		//设置上升沿捕获		
		nvic_flog = 2;		//标记一次捕获完成
	}
	
	else if(TIM_GetFlagStatus(TIM2, TIM_IT_Update)!=RESET)	//判断中断为更新中断，此时模块没有接收到超声波，距离大于10m
	{
		nvic_flog = 3;		//标记溢出中断
	}	
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update);		//清除TIM2中断标志
}

