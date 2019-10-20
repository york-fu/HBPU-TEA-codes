#include "control.h"
#include "filter.h"
#include "pwm.h"
#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "inv_mpu.h"

u8  ARMED=0;		// 电机正常标志  0 - 锁定  1 - 解锁
u8  FINISH;			// 读取数据标志  0 - 完成  1 - 失败
u16 THROTTLE=1672;	// 油门控制量  修改后的值，要实测
u8  TIME_flag;		// 时间标志

     
/*---- 内环 P, I, D ----*/
PID  INNER_X = {0.03, 0.01, 0.16},
     INNER_Y = {0.03, 0.01, 0.16},
     INNER_Z = {0.01, 0.01, 0.10};
/*---- 外环 P, I, D ----*/
PID  OUTER_X = {8, 0, 2},
     OUTER_Y = {8, 0, 2},
     OUTER_Z = {0, 0, 0};
     
	
SITE  Angle					= {0, 0, 0};	// 欧拉角
SITE  Expected_Angle		= {0, 0, 0};	// 期望欧拉角
SITE  Angle_Integral		= {0, 0, 0};	// 欧拉角积分
SITE  Palstance				= {0, 0, 0};	// 角速度
SITE  Expected_Palstance	= {0, 0, 0};	// 期望角速度
SITE  Palstance_Integral	= {0, 0, 0};	// 角速度积分



void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)  //5ms定时中断
	{   
		TIM1->SR&=~(1<<0);							// 清除定时器1中断标志位
	
		TIME_flag++;  								// 时间标志
		if(ARMED)  // 处于解锁状态
		{
			LED1_Flash(100);
			FINISH = MPU6050_data_filter(&Angle, &Palstance);	// 读取数据 已滤波处理  时间：2.76ms
			
			if(FINISH==0)  // 确保读数正确  并处于解锁状态  时间：52us
			{
				OUTER_CONTROL(Angle, Expected_Angle);  			// 外环控制  时间：22us
				INNER_CONTROL(Palstance, Expected_Palstance);	// 内环控制  时间：26us
			}
		}
		else  // 处于锁定状态
		{
			LED0_Flash(100);
			Run_Off();  // 关闭电机
		}
	}       	
} 


/*********************************************************************
* 函数功能：内环PID控制 -- 控制电机
* 入口参数：角速度值  期望角速度值
* 作    者：湖北理工电子技术协会
**********************************************************************/
void INNER_CONTROL(SITE palstance, SITE expected_palstance)  // 内环控制
{
	u16 moto1=0,moto2=0,moto3=0,moto4=0;
	static SITE Elast_palstance = {0, 0, 0};  // 储存上次角速度
	SITE Epalstance;	// 差值
	SITE Edpalstance;	// 误差微分
	MOTOR X, Y, Z;  	// PID输出数据
	
	/*-- 当前角速度误差 --*/
	Epalstance.x = palstance.x - expected_palstance.x;
	Epalstance.y = palstance.y - expected_palstance.y;
	Epalstance.z = palstance.z - expected_palstance.z;
	
	/*-- 当前角速度误差积分 --*/
	Palstance_Integral.x += Epalstance.x;  
	Palstance_Integral.y += Epalstance.y;
	Palstance_Integral.z += Epalstance.z;
	integral_limit(&Palstance_Integral, 0);  // 积分上限
	
	/*-- 当前角速度误差微分 --*/
	Edpalstance.x = Epalstance.x - Elast_palstance.x;  
	Edpalstance.y = Epalstance.y - Elast_palstance.y;
	Edpalstance.z = Epalstance.z - Elast_palstance.z;
	Elast_palstance.x = Epalstance.x;  // 储存上次角速度误差
	Elast_palstance.y = Epalstance.y;
	Elast_palstance.z = Epalstance.z;
	
	/*-- X轴 --*/
	X.pout = INNER_X.P * Epalstance.x;
	X.iout = INNER_X.I * Palstance_Integral.x;
	X.dout = INNER_X.D * Edpalstance.x;
	/*-- Y轴 --*/
	Y.pout = INNER_Y.P * Epalstance.y;
	Y.iout = INNER_Y.I * Palstance_Integral.y;
	Y.dout = INNER_Y.D * Edpalstance.y;
	/*-- Z轴 --*/
	Z.pout = INNER_Z.P * Epalstance.z;
	Z.iout = INNER_Z.I * Palstance_Integral.z;
	Z.dout = INNER_Z.D * Edpalstance.z;

	/*-- 三个轴输出和 --*/
	X.OUT = X.pout + X.iout + X.dout;
	Y.OUT = Y.pout + Y.iout + Y.dout;
	Z.OUT = Z.pout + Z.iout + Z.dout;

	if(THROTTLE>1670)  //油门小于1670不起飞
	{
		moto1 = THROTTLE + X.OUT - Y.OUT + Z.OUT;
		moto2 = THROTTLE + X.OUT + Y.OUT - Z.OUT;
		moto3 = THROTTLE - X.OUT + Y.OUT + Z.OUT;
		moto4 = THROTTLE - X.OUT - Y.OUT - Z.OUT;
	}
	else  // 关闭电机
	{
		moto1 = 1200; 
		moto2 = 1200; 
		moto3 = 1200; 
		moto4 = 1200; 
		Integral_Reset();  // 积分清零
	}
	
	if(Angle.x >= 45 || Angle.x <= -45 || Angle.y >= 45 || Angle.y <= -45)ARMED=0;  // 保护人生安全
	
	Moto_PwmRflash(moto1, moto2, moto3, moto4);  // 更新占空比
}


/*********************************************************************
* 函数功能：外环PID控制 -- 得出期望角速度
* 入口参数：角度值  期望角度值
* 作    者：湖北理工电子技术协会
**********************************************************************/
void OUTER_CONTROL(SITE angle, SITE expected_angle)  // 外环控制
{
	static SITE Eangle_angle = {0, 0, 0};  // 储存上次角度
	SITE Eangle;	// 差值
	SITE Edangle;	// 误差微分
	
	/*-- 当前角度误差 --*/
	Eangle.x = angle.x - expected_angle.x;
	Eangle.y = angle.y - expected_angle.y;
	Eangle.z = angle.z - expected_angle.z;

	/*-- 当前角度误差积分 --*/
	Angle_Integral.x += Eangle.x;
	Angle_Integral.y += Eangle.y;
	Angle_Integral.z += Eangle.z;
	integral_limit(&Angle_Integral, 1);  // 积分上限

	/*-- 当前角度误差微分 --*/
	Edangle.x = Eangle.x - Eangle_angle.x;
	Edangle.y = Eangle.y - Eangle_angle.y;
	Edangle.z = Eangle.z - Eangle_angle.z;
	Eangle_angle.x = Eangle.x; // 储存上次角度误差
	Eangle_angle.y = Eangle.y;
	Eangle_angle.z = Eangle.z;

	/*-- 得出期望角速度 --*/
	Expected_Palstance.x = -(Eangle.x*OUTER_X.P + Angle_Integral.x*OUTER_X.I + Edangle.x*OUTER_X.D);
	Expected_Palstance.y = -(Eangle.y*OUTER_Y.P + Angle_Integral.y*OUTER_Y.I + Edangle.y*OUTER_Y.D);
	Expected_Palstance.z = -(Eangle.z*OUTER_Z.P + Angle_Integral.z*OUTER_Z.I + Edangle.z*OUTER_Z.D);
}

void Moto_PwmRflash(u16 m1, u16 m2, u16 m3, u16 m4)	// 更新占空比
{
	if(m1>3500)m1=3500;  // 上限
	if(m2>3500)m2=3500;
	if(m3>3500)m3=3500;
	if(m4>3500)m4=3500;
	PWM1 = m1;
	PWM2 = m2;
	PWM3 = m3;
	PWM4 = m4;
}

void Run_Off(void)  // 关闭电机
{
	PWM1 = 1200, PWM2 = 1200, PWM3 = 1200, PWM4 = 1200;
	Integral_Reset();  // 积分清零
}

void Integral_Reset(void)  // 积分清零
{
	Angle_Integral.x = 0, Angle_Integral.y = 0, Angle_Integral.z = 0;  // 积分清零
	Palstance_Integral.x = 0, Palstance_Integral.y = 0, Palstance_Integral.z = 0;
}
