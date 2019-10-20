#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "encoder.h"
#include "motor.h"
#include "led.h"
#include "inv_mpu.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


PID upPD = { 460, 0, 1 };			//直立PD参数
PID velocityPI = { 170, 0.8, 0 };	//速度PI参数
PID turnP = {1.2, 0, 0};				//转向P参数

s8 LeftSteer = 0;
s8 RightSteer = 0;
s8 Steer = 0;		//平衡车行驶变量
s16 TurnValue = 0;	//平衡车转向变量

static void GetEncoderNum(short *encoderLeftNum, short *encoderRightNum);
static short UprightPd(float pitch, short yAngularSpeed);
static short SpeedPi(short encoderLeftNum, short encoderRightNum, u8 mode);
static int TurnPID(int encoder_left,int encoder_right,float yaw);
static void LimitPwm(short *leftPwmValue, short *rightPwmValue);
static u8 CheckState(float pitch);
static void SetPwm(short leftPwmValue, short rightPwmValue);

/******************************************************************
*函数名称:	KeyStartAndStop
*函数功能:	按键用于开启和关闭小车
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void KeyStartAndStop(void)
{
	static bool flag = 0;
	
	if (KEY == 0)
	{
		delay_ms(10);
		
		if (KEY == 0)
		{
			flag = !flag;
			while(KEY == 0);
			
			if (flag)
			{
				TIM_Cmd(TIM1, ENABLE);		//使能TIM3
			}
			else
			{
				TIM_Cmd(TIM1, DISABLE);		//失能TIM3
				
				//关闭电机,灭灯
				AIN1 = 0;
				AIN2 = 0;
				BIN1 = 0;
				BIN2 = 0;
				LED = 1;
				
				//清空数据
				SaveAndRead(NULL, NULL, NULL, NULL, EMPTY_DATA);	//清空显示数据				
				SpeedPi(0, 0, EMPTY_INTEGRAL);						//清空积分
			}	
		}
	}
}

/***********************************************************************
*函数名称:	SaveAndRead
*函数功能:	用于保存和读取需要的数据(俯仰角,Y轴角速度,左边编码器的数,
*			右边编码器的数),避免了全局变量的使用,降低程序耦合度
*函数参数:	pitch:				指向俯仰角的指针
*			yAngularSpeed:		指向Y轴角速度的指针
*			encoderLeftNum:		指向左边编码器的数的指针
*			encoderRightNum:	指向右边编码器的数的指针
*			mode:				模式(SAVE_DATA为保存数据,
*								READ_DATA为读取数据,
*								EMPTY_DATA为清空数据)
*返 回 值:	无
************************************************************************/
void SaveAndRead(float *pitch, short *yAngularSpeed, short *encoderLeftNum, short *encoderRightNum, u8 mode)
{
	static float savePitch = 0.0;
	static short saveYAngularSpeed = 0;
	static short saveEncoderLeftNum = 0;
	static short saveEncoderRightNum = 0;
	
	if (mode == SAVE_DATA)
	{
		savePitch = *pitch;
		saveYAngularSpeed = *yAngularSpeed;
		saveEncoderLeftNum = *encoderLeftNum;
		saveEncoderRightNum = *encoderRightNum;
	}
	else if (mode == READ_DATA)
	{
		*pitch = savePitch;
		*yAngularSpeed = saveYAngularSpeed;
		*encoderLeftNum = saveEncoderLeftNum;
		*encoderRightNum = saveEncoderRightNum;
	}
	else if (mode == EMPTY_DATA)
	{
		savePitch = 0.0;
		saveYAngularSpeed = 0;
		saveEncoderLeftNum = 0;
		saveEncoderRightNum = 0;
	}
}

/******************************************************************
*函数名称:	Tim3TimeIntInit
*函数功能:	初始化定时器3,用于5ms中断
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Tim1TimeIntInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 4999;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM1, DISABLE);
}

/******************************************************************
*函数名称:	TIM1_UP_IRQHandler
*函数功能:	定时器1的更新中断,5ms
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 time = 0;				//呼吸灯计数变量
	static float pitch = 0.0;			//俯仰角
	static float roll = 0;				//横滚角
	static float yaw = 0;				//航向角		
	static short yAngularSpeed = 0;		//Y轴角速度	
	static short zAngularSpeed = 0;		//Z轴角速度
	static short encoderLeftNum = 0;	//左边编码器的数
	static short encoderRightNum = 0;	//右边编码器的数
	static short upPwmValue = 0;		//直立PD调节的PWM波的值
	static short velocityPwmValue = 0;	//速度PI调节的PWM波的值
	static short turnPwmValue = 0;		//转向调节的PWM波的值
	static short leftPwmValue = 0;		//左边电机的PWM值
	static short rightPwmValue = 0;		//右边电机的PWM值
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//检查指定的TIM中断发生与否:TIM中断源
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//清除TIMx的中断待处理位:TIM中断源
		
		GetEncoderNum(&encoderLeftNum, &encoderRightNum);		//得到左右编码器的数值								
		
		mpu_dmp_get_data(&pitch, &roll, &yaw, &yAngularSpeed, &zAngularSpeed);	//更新位置
		
//		SaveAndRead(&pitch, &yAngularSpeed, &encoderLeftNum, &encoderRightNum, SAVE_DATA);	//保存数据
		
		upPwmValue = UprightPd(pitch, yAngularSpeed);								//直立PD调节
		velocityPwmValue = SpeedPi(encoderLeftNum, encoderRightNum, NORMAL_MODE);	//速度PI调节
		turnPwmValue = TurnPID(encoderLeftNum, encoderRightNum, zAngularSpeed);		//转向P调节
		
		leftPwmValue = upPwmValue + velocityPwmValue + turnPwmValue;		//左边电机PWM赋值
		rightPwmValue = upPwmValue + velocityPwmValue - turnPwmValue;		//右边电机PWM赋值
		
		LimitPwm(&leftPwmValue, &rightPwmValue);				//限幅
		if (CheckState(pitch) == 0)								//如果状态正常
		{
			SetPwm(leftPwmValue, rightPwmValue);				//设置电机的PWM值
		}
		
		//呼吸灯指示运行状态
		time++;
		if (time > 149)
		{
			time = 0;
			LED = !LED;
		}	
	}	
}

/******************************************************************
*函数名称:	GetEncoderNum
*函数功能:	得到左右编码器的数值
*函数参数:	encoderLeftNum:		左边编码器数的指针
*			encoderRightNum:	右边编码器数的指针
*返 回 值:	无
*******************************************************************/
static void GetEncoderNum(short *encoderLeftNum, short *encoderRightNum)
{
	u16 leftTemp;
	u16 rightTemp;	
	
	leftTemp = ReadEncoderValue(2);			//读取左编码器的值
	if (leftTemp > 32767)					//反转,向下计数
	{
		*encoderLeftNum = leftTemp - 65535;
	}
	else
	{
		*encoderLeftNum = leftTemp;
	}
	*encoderLeftNum = -*encoderLeftNum;		//左右编码器接口相反,故要对左边取反
	
	rightTemp = ReadEncoderValue(4);		//读取右编码器的值
	if (rightTemp > 32767)
	{
		*encoderRightNum = rightTemp - 65535;
	}
	else
	{
		*encoderRightNum = rightTemp;
	}
}

/******************************************************************
*函数名称:	UprightPd
*函数功能:	直立PD控制
*函数参数:	pitch:			俯仰角
*			yAngularSpeed:	Y轴角速度
*返 回 值:	upPwmValue:		PWM波的占空比值
*******************************************************************/
static short UprightPd(float pitch, short yAngularSpeed)
{
	short upPwmValue;
	float bias;		//偏差值
	
	bias = pitch - 0;		//0度平衡
	
	upPwmValue = upPD.kp * bias + upPD.kd * yAngularSpeed;
	
	return upPwmValue;
}

/****************************************************************************************
*函数名称:	SpeedPi
*函数功能:	速度PI控制,当车停止时,用 EMPTY_INTEGRAL 模式清空积分
*函数参数:	encoderLeftNum:		左编码器的数
*			encoderRightNum:	右编码器的数
*			mode:				模式(NORMAL_MODE为正常积分,EMPTY_INTEGRAL为清空积分)
*返 回 值:	velocityPwmValue:	PWM波的占空比值
******************************************************************************************/
static short SpeedPi(short encoderLeftNum, short encoderRightNum, u8 mode)
{
	int velocityPwmValue;
	static float bias;		//偏差值
	static float lastBias;	//上一次偏差值
	static float integral;	//积分值
	
	if (mode == EMPTY_INTEGRAL)
	{
		integral = 0;
		
		return 0;
	}
	
	bias = (encoderLeftNum + encoderRightNum) - 0;				//目标值为0
	bias = bias * 0.2 + lastBias * 0.8;	//一阶低通滤波

	integral = integral+bias+Steer;
	
	//积分限幅
	if (integral > MAX_INTEGRAL)
	{
		integral = MAX_INTEGRAL;
	}
	else if (integral < -MAX_INTEGRAL)
	{
		integral = -MAX_INTEGRAL;
	}
	
	velocityPwmValue = velocityPI.kp * bias + velocityPI.ki * integral;
	
	//PI速度环限幅
	if (velocityPwmValue > MAX_PI)
	{
		velocityPwmValue = MAX_PI;
	}
	else if (velocityPwmValue < -MAX_PI)
	{
		velocityPwmValue = -MAX_PI;
	}
	
	lastBias = bias;	//记录上次误差
	
	return (short)velocityPwmValue;
}

/**************************************************************************
函数功能：转向控制  
入口参数：左轮编码器、右轮编码器、Z轴陀螺仪
返回  值：转向控制PWM
**************************************************************************/
static int TurnPID(int encoder_left,int encoder_right,float zAngularSpeed)//转向控制
{
	float value;
	static float bias;
	
	bias = zAngularSpeed - TurnValue;
	
	value = turnP.kp*bias;
	
	return (int)value;
}

/******************************************************************
*函数名称:	LimitPwm
*函数功能:	限制PWM波的幅值
*函数参数:	leftPwmValue:	左边电机的PWM值的指针
*			rightPwmValue:	右边电机的PWM值的指针
*返 回 值:	无
*******************************************************************/
static void LimitPwm(short *leftPwmValue, short *rightPwmValue)
{
	if (*leftPwmValue > PWM_MAX_VALUE)
	{
		*leftPwmValue = PWM_MAX_VALUE;
	}
	else if (*leftPwmValue < -PWM_MAX_VALUE)
	{
		*leftPwmValue = -PWM_MAX_VALUE;
	}
	
	if (*rightPwmValue > PWM_MAX_VALUE)
	{
		*rightPwmValue = PWM_MAX_VALUE;
	}
	else if (*rightPwmValue < -PWM_MAX_VALUE)
	{
		*rightPwmValue = -PWM_MAX_VALUE;
	}
}

/******************************************************************
*函数名称:	CheckState
*函数功能:	检测小车的状态,如果俯仰角绝对值大于40度,停止电机
*函数参数:	pitch:	俯仰角
*返 回 值:	0:		状态正常
*			其他:	状态异常
*******************************************************************/
static u8 CheckState(float pitch)
{
	if ((pitch > 40) || (pitch < -40))	//俯仰角绝对值大于40度
	{
		//停止电机
		AIN1 = 0;
		AIN2 = 0;
		BIN1 = 0;
		BIN2 = 0;
		
		SpeedPi(0, 0, EMPTY_INTEGRAL);	//清空积分
		
		return 1;
	}
	
	return 0;
}

/******************************************************************
*函数名称:	SetPwm
*函数功能:	调节左右电机的PWM波
*函数参数:	leftPwmValue:	左边电机的PWM值
*			rightPwmValue:	右边电机的PWM值
*返 回 值:	无
*******************************************************************/
static void SetPwm(short leftPwmValue, short rightPwmValue)
{
	if (leftPwmValue < 0)
	{
		AIN1 = 1;
		AIN2 = 0;
		
		PWMA = -leftPwmValue;
	}
	else
	{
		AIN1 = 0;
		AIN2 = 1;
		
		PWMA = leftPwmValue;
	}
	
	if (rightPwmValue < 0)
	{
		BIN1 = 1;
		BIN2 = 0;
		
		PWMB = -rightPwmValue;
	}
	else
	{
		BIN1 = 0;
		BIN2 = 1;
		
		PWMB = rightPwmValue;
	}
}
