#include "encoder.h"

/*********************************************************
*函数：编码器配置函数
*输入：自动重装载数，分频数
*功能：定时器2通道1，2配置为编码器模式
*输出：
**********************************************************/

TIM_ICInitTypeDef  TIM2_ICInitStructure;

void Encoder_L_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0| GPIO_Pin_1;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1| GPIO_Pin_0);						 //PA0 下拉
	
	//初始化定时器2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2通道CH1，CH2输入极性和映射
	TIM2->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
	TIM2->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
	TIM2->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
	TIM2->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
	
	//初始化编码器
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3 ，上下沿都计数
    TIM_ICStructInit(&TIM2_ICInitStructure);//将结构体中的内容缺省输入
    TIM2_ICInitStructure.TIM_ICFilter = 6;  //选择输入捕获滤波器
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);//将TIM_ICInitStructure的指定参数初始化TIM2
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM2->CNT = 0;
	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
 
}
/*********************************************************
*函数：编码器配置函数
*输入：自动重装载数，分频数
*功能：定时器4通道1，2配置为编码器模式
*输出：
**********************************************************/
//void Encoder_R_Init(u16 arr,u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
//	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6| GPIO_Pin_7;  //PA0 清除之前设置  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6 输入  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_6| GPIO_Pin_7);						 //PA6 下拉
//	

//	//初始化定时器4 TIM4	 
//	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	
//	//初始化TIM4通道CH1，CH2输入极性和映射
//	TIM3->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
//	TIM3->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
//	TIM3->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
//	TIM3->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
//	
//	//初始化编码器
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3 ，上下沿都计数
//    TIM_ICStructInit(&TIM3_ICInitStructure);//将结构体中的内容缺省输入
//    TIM3_ICInitStructure.TIM_ICFilter = 6;  //选择输入捕获滤波器
//    TIM_ICInit(TIM3, &TIM3_ICInitStructure);//将TIM_ICInitStructure的指定参数初始化TIM2
//	
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级2级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
//	TIM3->CNT = 0;
//	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器2
//}	

//定时器2中断服务程序
void TIM2_IRQHandler(void)
{ 	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{	  
			     	    					   
	}	
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位
 
}
//定时器4中断服务程序
//void TIM3_IRQHandler(void)
//{ 	
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//	{	  
//			     	    					   
//	}	
// 
//    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
// 
//}

