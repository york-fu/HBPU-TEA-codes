#include "control.h"
#include "motor.h"
#include "led.h"
#include "inv_mpu.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "24l01.h"


PID Pid_X = {12, 0, 0.2};
PID Pid_Y = {12, 0, 0.2};
PID Pid_Z = {6, 0, 0};


u8 LOCK_FLAG = 0;		//锁定标志
u16 Accelerator = 1000;	//油门值

/******************************************************************
*函数名称:	CheckState
*函数功能:	检测四轴的状态,如果俯仰角绝对值大于40度,停止电机
*函数参数:	pitch:	俯仰角;roll: 横滚角
*返 回 值:	0:		状态正常
*			其他:	状态异常
*******************************************************************/
u8 CheckState(float pitch, float roll)
{
	if (((pitch > 45) || (pitch < -45)) || ((roll > 45) || (roll < -45)))	//绝对值大于45度
	{
		return 1;
	}
	
	return 0;
}


/******************************************************************
*函数名称:	PID_Control
*函数功能:	PID控制函数
*函数参数:	MPU6050数据
*返 回 值:	无
*******************************************************************/
void PID_Control(MPUDATA MpuData, MPUDATA MpuAS)
{
	XYZ_DATA OUT;
	static XYZ_DATA Error;			//误差值（比例）
	static XYZ_DATA Error_Add;		//累计误差值（积分）
	
	/*计算本次角度误差*/
	Error.X = MpuData.pitch;
	Error.Y = MpuData.roll;
	Error.Z = MpuData.yaw;
	
	/*计算角度积分*/
	Error_Add.X += Error.X;
	Error_Add.Y += Error.Y;
	Error_Add.Z += Error.Z;
	
	/*X轴角度积分限幅*/
	if(Error_Add.X > MAX_INTEGRAL)		 Error_Add.X = MAX_INTEGRAL;
	else if(Error_Add.X < -MAX_INTEGRAL) Error_Add.X = -MAX_INTEGRAL;

	/*Y轴角度积分限幅*/
	if(Error_Add.X > MAX_INTEGRAL)		 Error_Add.X = MAX_INTEGRAL;
	else if(Error_Add.X < -MAX_INTEGRAL) Error_Add.X = -MAX_INTEGRAL;
	
	/*PID计算*/
	OUT.X = Pid_X.P*Error.X + Pid_X.I*Error_Add.X + Pid_X.D*MpuAS.X_AngularSpeed;
	OUT.Y = Pid_Y.P*Error.Y + Pid_Y.I*Error_Add.Y + Pid_Y.D*MpuAS.Y_AngularSpeed;
	OUT.Z = Pid_Z.P*MpuAS.Z_AngularSpeed;
	
	/*PWM输出限幅*/
	if(OUT.X > X_PWM_MAX)		OUT.X = X_PWM_MAX;
	else if(OUT.X < -X_PWM_MAX)	OUT.X = -X_PWM_MAX;
	
	if(OUT.Y > Y_PWM_MAX)		OUT.Y = Y_PWM_MAX;
	else if(OUT.Y < -Y_PWM_MAX)	OUT.Y = -Y_PWM_MAX;
	
	if(OUT.Z > Z_PWM_MAX)		OUT.Z = Z_PWM_MAX;
	else if(OUT.Z < -Z_PWM_MAX)	OUT.Z = -Z_PWM_MAX;
	
	/*设置电机PWM波值*/
	PWM1 = Accelerator + OUT.X - OUT.Y - OUT.Z;
	PWM2 = Accelerator + OUT.X + OUT.Y + OUT.Z;
	PWM3 = Accelerator - OUT.X - OUT.Y + OUT.Z;
	PWM4 = Accelerator - OUT.X + OUT.Y - OUT.Z;
}


/******************************************************************
*函数名称:	Descend
*函数功能:	智能下降函数，防摔
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Descend()
{
	u8 t;
	
	TIM_Cmd(TIM1,DISABLE);	//关闭定时器
	
	PWM1 = THRESHOLD;
	PWM2 = THRESHOLD;
	PWM3 = THRESHOLD;
	PWM4 = THRESHOLD;
	
	for(t=0;t<10;t++)	//下降时间10s
	{
		delay_ms(1000);
	}

	PWM1 = 0;
	PWM2 = 0;
	PWM3 = 0;
	PWM4 = 0;
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//先占优先级1级
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
	static u16 time = 0;					//指示灯计数变量
	static MPUDATA MpuData = {0,0,0};		//欧拉角
	static MPUDATA MpuAS = {0,0,0};			//角速度
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源					
			
		/*PID调节*/
		if((LOCK_FLAG == 1)&&(Accelerator>1500))		//解锁状态
		{
			if(mpu_dmp_get_data(&MpuData.pitch,&MpuData.roll,&MpuData.yaw,
				&MpuAS.X_AngularSpeed,&MpuAS.Y_AngularSpeed,&MpuAS.Z_AngularSpeed)==0)	//成功读取数据
			{		
				if(CheckState(MpuData.pitch, MpuData.roll)==0)	//判断是否锁定和机身状态
				{
					PID_Control(MpuData, MpuAS);
				}
				else
				{
					PWM1 = 0;
					PWM2 = 0;
					PWM3 = 0;
					PWM4 = 0;
				}			
			}
		}
		else		//锁定状态
		{
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
		}
		
		//呼吸灯指示运行状态
		time++;
		
		if(time%100==0)
		{
			LED0 = !LED0;
		}
		
		/*防失联控制*/
		if(time > 599)				//3s时间
		{
			if(Nrf_Buff[4] == 1)	//未失联
			{			
				Nrf_Buff[4] = 0;	//清空连接标志
				LED1 = 1;
				printf("OK\r\n");
			}
			else					//四轴失联
			{
				Nrf_Buff[3] = 0;	//清空连接标志
				LOCK_FLAG = 0;		//锁定四轴
				LED1 = 0;			//亮红灯
				printf("NO\r\n");
			}
			time = 0;
		}
	}
}

