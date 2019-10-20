#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "inv_mpu.h"
#include "usart.h"
#include "motor.h"
#include "irtube.h"


extern float Pitch;

u8 Run = 0;
u8 Time = 0;
u16 PWMvalue = 0;

_PID Pid = {0,0,0};


/******************************************************************
*函数功能:	PID调节，增量式
*函数参数:	Angle：实际角度
			AngularSpeed：实际角速度
*返 回 值:	PWM波
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed)
{
	float value=0;
	static float Error=0;				//当前误差
	static short Last_AngularSpeed=0;	//上次角速度
	
	Error = *Angle - 0.0;					//计算误差
	
	value = Pid.Kp * (float)(*AngularSpeed) + Pid.Ki * Error + Pid.Kd * (float)((*AngularSpeed) - Last_AngularSpeed);
	
	Last_AngularSpeed = *AngularSpeed;	//上次角速度
	
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
*函数参数:	angle：角度值
*返 回 值:	无
*******************************************************************/
void PWM_Control(float angle)
{
	u8 mode=0;

	if(angle<-2)
	{
		mode = FRONT;
	}
	else if(angle>2)
	{
		mode = REAR;
	}
	else
	{	
		mode = STOP;
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
	
	Motor_Control(PWMvalue,PWMvalue,mode);
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
	static u16 time=0;
//	static short PWMValue=0;
//	static short PWMMAX = 6000;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源	
		
		if(Run)
		{
			if(time<Time)
			{
				PWM_Control(Pitch);
//				printf("%f\r\n",Pitch);
			}
			else
			{
				Motor_Control(0,0,0);
			}
			time++;
			if(time>(Time*20)) time = 0;
		}
		else
		{
			Motor_Control(0,0,0);
			time = 0;
		}
	}
}

