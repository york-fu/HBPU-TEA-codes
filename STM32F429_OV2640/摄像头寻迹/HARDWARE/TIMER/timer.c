#include "timer.h"
#include "motor.h"
#include "encoder.h"
#include "lcd.h"
#include "dcmi.h"
#include "EasyTracer.h"

u8 Run=0;

_PID Pid = {0,0,0};		//170,0,1.5

/******************************************************************
*函数功能:	PID调节，位置式
*函数参数:	position：实际位置
			mode：积分模式
*返 回 值:	PWM波
*******************************************************************/
short PID_Adjust(u16* position,u8 mode)
{
	float value=0;
	static float Error=0;				//当前误差
	static float LastError=0;			//上次误差
	static float Integral=0;			//误差积分
	
	LastError = Error;					//记录上次误差
	Error = *position - 160;			//计算本次误差
	
	if(mode) Integral += Error;
	else Integral = 0;
	
	/*积分限幅*/
	if(Integral>Integral_MAX) Integral = Integral_MAX;
	else if(Integral<-Integral_MAX) Integral = -Integral_MAX;
	
	value = Pid.Kp * Error + Pid.Ki * Integral + Pid.Kd * (Error - LastError);
	
	/*输出限幅*/
	if(value>PWM_MAX)
	{
		value = PWM_MAX;
	}
	else if(value<-PWM_MAX)
	{
		value = -PWM_MAX;
	}
	
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
		pwmvalue = -pwmvalue;
		mode = RIGHT;	//右转
	}
	else
	{
		mode = LEFT;	//左转
	}
	Motor_Control(pwmvalue,pwmvalue,mode);
}


/**************************************************************************************************************/

TIM_HandleTypeDef TIM1_Handler;      //定时器句柄 

/***************************************************************************
函数功能：	定时器中断初始化
入口参数：	arr：自动重装值
			psc：时钟预分频数
返回参数：	无
函数说明;	定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
			Ft=定时器工作频率,单位:Mhz
			定时器1挂在APB2上，时钟为APB2*2=180MHz
****************************************************************************/
void TIM1_Init(u16 arr,u16 psc)
{  
    TIM1_Handler.Instance=TIM1;                          //定时器1
    TIM1_Handler.Init.Prescaler=psc;                     //分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM1_Handler.Init.Period=arr;                        //自动装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM1_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM1_Handler);	//使能定时器和定时器中断
}

//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM1_CLK_ENABLE();					//使能TIM3时钟
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn,1,3);	//设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);			//开启ITM1更新中断 
}

//定时器1中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}


/*中线标志*/
u8 Center_flag=0;
u16 MidlinePos = 160;

//定时器3中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	static short PwmValue=0;
	static u8 mode=STOP;
//	static u8 TimCount=0;
	
    if(htim==(&TIM1_Handler))
    {	
		/*打印帧率*/
//		TimCount++;
//		if(TimCount>9)
//		{
//			TimCount = 0;
//			printf("%d\t",ov_frame);
//			ov_frame = 0;
//		}
		
		if(Run)					//运行中
		{
			if(Center_flag)		//识别到中线
			{
				Center_flag = 0;
				
//				PwmValue = PID_Adjust(&MidlinePos,0);
				
				if(MidlinePos>170)
				{
					mode = LEFT;
				}
				else if(MidlinePos<150)
				{
					mode = RIGHT;
				}
				else
				{
					mode = FRONT;
				}
			}
			else
			{
				mode = STOP;
			}
			Motor_Control(1600,1600,mode);
		}
		else
		{
			Motor_Control(0,0,STOP);	//停车
		}
    }
}


