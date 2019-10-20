#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "inv_mpu.h"
#include "usart.h"
#include "motor.h"
#include "irtube.h"

u8 Run = 1;
short PWMValue=0;

_PID Pid = {12,0,0};		//170,0,1.5


/******************************************************************
*函数功能:	PID调节，位置式
*函数参数:	Angle：实际角度
			AngularSpeed：实际角速度
			mode：积分模式
*返 回 值:	PWM波
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed,u8 mode)
{
	float value=0;
	static float Error=0;				//当前误差
	static float Integral=0;			//误差积分
	
	Error = *Angle - 0.0;				//计算误差
	
	if(mode) Integral += Error;
	else Integral = 0;
	
	/*积分限幅*/
	if(Integral>Integral_MAX) Integral = Integral_MAX;
	else if(Integral<-Integral_MAX) Integral = -Integral_MAX;
	
	value = Pid.Kp * Error + Pid.Ki * Integral + Pid.Kd * (float)(*AngularSpeed);
	
	/*输出限幅*/
	if(value>PWM_MAX)
	{
		value = PWM_MAX;
	}
	else if(value<-PWM_MAX)
	{
		value = -PWM_MAX;
	}
//	printf("%f\t%d\t%f\t",Error,*AngularSpeed,value);
	
	return (short)value;
}



/******************************************************************
*函数功能:	PWM波控制
*函数参数:	pwmvalue：PWM波值
			mode：运行模式
*返 回 值:	无
*******************************************************************/
void PWM_Control(short pwmvalue)
{
	u8 mode=0;

	/*方向判断*/
	if(pwmvalue<0)
	{
		mode = FRONT;
	}
	else
	{
		mode = REAR;
	}
	
	/*红外寻迹部分*/
	if(mode==FRONT)					//前进
	{
		if(IR1==0&&IR2==0)			//前进直行
		{			
			mode = FRONT;			//前进
		}
		else if(IR1==0&&IR2==1)		//前进左偏
		{
			mode = RIGHT;			//右转
		}
		else if(IR1==1&&IR2==0)		//前进右偏
		{
			mode = LEFT;			//左转
		}
//		else if(IR1==1&&IR2==1)		//检测到停止线
//		{
//			mode = STOP;			//停止
//			Run =  0;
//		}
	}
	else if(mode==REAR)				//后退
	{
		if(IR3==0&&IR4==0)			//后退直行
		{			
			mode = REAR;			//后退
		}
		else if(IR3==0&&IR4==1)		//后退左偏
		{
			mode = LEFT;			//左转
		}
		else if(IR3==1&&IR4==0)		//后退右偏
		{
			mode = RIGHT;			//右转
		}
//		else if(IR3==1&&IR4==1)		//检测到停止线
//		{
//			mode = STOP;			//停止
//			Run =  0;
//		}
	}
	
	Motor_Control(mode);
	
//	printf("%d\t%d\r\n",pwmvalue,mode);
}


/******************************************************************
*函数功能:	初始化定时器3,用于中断
*函数参数:	arr：自动重装值
			psc：分频系数
*返 回 值:	无
*******************************************************************/
void TIM3_IntInit(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/*初始化定时器计数*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);					//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	/*初始化定时器中断*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);						//定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);
}


/******************************************************************
*函数功能:	定时器3的更新中断,1ms
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM3_IRQHandler(void)
{	
	static u16 Tim3Int=0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源	
		
		Tim3Int++;
		if(Tim3Int>999) Tim3Int = 0;
	}
}


/******************************************************************
*函数名称:	TIM1_IntInit
*函数功能:	初始化定时器1,用于中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/*初始化定时器计数*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	/*初始化定时器中断*/
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);						//定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				//从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM1, ENABLE);
}


/******************************************************************
*函数名称:	TIM1_UP_IRQHandler
*函数功能:	定时器1的更新中断,5ms
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 Tim1Int=0;
	static float Pitch=0;
	static float Roll=0;
	static float Yaw=0;
	static short X_AngularSpeed=0;
	static short Y_AngularSpeed=0;
	static short Z_AngularSpeed=0;
	static u16 PWMTim=0;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源	
		
		if(Run)
		{
			if(mpu_dmp_get_data(&Pitch,&Roll,&Yaw,&X_AngularSpeed,&Y_AngularSpeed,&Z_AngularSpeed)==0)
			{	
				PWMValue = PID_Adjust(&Pitch,&X_AngularSpeed,1);
				
				if(PWMValue<0)
				{
					PWMTim = -PWMValue;
				}
				else
				{					
					PWMTim = PWMValue;
				}
				
				if(Tim1Int<PWMTim)
				{
					PWM_Control(PWMValue);				
				}
				else
				{					
					Motor_Control(0);
				}
			}
			Tim1Int++;
			if(Tim1Int>99) Tim1Int = 0;
		}
		else
		{
			Tim1Int = 0;
			PID_Adjust(0,0,0);		//清空积分
			Motor_Control(0);
		}
		
	}
}

