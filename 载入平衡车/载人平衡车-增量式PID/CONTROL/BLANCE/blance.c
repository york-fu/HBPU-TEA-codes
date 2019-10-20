#include "blance.h"
#include "usart.h"
#include "inv_mpu.h"

volatile float roll[2];		//欧拉角

u8 ARMED = 0;		//锁定解锁标志位
u8 Accelerator=1;	//速度档位：1，2，3，4

PID BlancePid	= {0.03, 0.6, 0.2};		//平衡环pid


/********************************************************************
*
* 得到绝对值
*
*********************************************************************/
float maybe(float pwm)				
{
	if(pwm<0)pwm = -pwm;
	
	if(pwm>7200)pwm = 7200;		//限幅，防止超出7200
	
	return pwm;
}


/********************************************************************
*
* 平衡环:PD控制
* PID增量式:IncresePwm = Kp*(E(k)-E(k-1))+Ki*E(k)+Kd*(E(k)-2*E(k-1)+E(k-2))
* PID位置式:	PosPwm = Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*
*********************************************************************/
float Blance(float pitch1, float gyro_balance)
{
	float blance_pwm;
	
	static float Gyro;				//当前角速度
	static float Last_Gyro;			//上次角速度
	static float Gyro_Change;		//角速度变化率（微分）
	
	Last_Gyro = Gyro;				//记录上次角速度
	Gyro = gyro_balance - 0;		//计算本次角速度
	Gyro_Change = Gyro - Last_Gyro;	//计算微分
	
	blance_pwm = BlancePid.P*gyro_balance + BlancePid.I*pitch1 + BlancePid.D*Gyro_Change;
	
	if (blance_pwm > 20) blance_pwm = 20;			//pwm限幅
	else if(blance_pwm < -20) blance_pwm = -20;
	
	return -blance_pwm;
}


/********************************************************************
*
* 开启电机
*
*********************************************************************/
void start_machine(short leftpwm, short rightpwm, float pitch_1,float pitch_2)
{
//	static u8 x=0;
//	static short SetSpeed=2;
	
	if(maybe(pitch_1)<50.0 && maybe(pitch_2)<50.0 && ARMED)		// 处于解锁  如果车没倒，则开启电机
	{
		if(leftpwm > 0) DIR_L = 1;	//左轮方向判断
		else DIR_L = 0;
		
		if(rightpwm > 0) DIR_R = 0;	//右轮方向判断
		else DIR_R = 1;
		
		leftpwm = maybe(leftpwm);	//得到pwm绝对值
		rightpwm = maybe(rightpwm);		
		
//		if(maybe(pitch_1)>5 && maybe(pitch_2)>5)
//		{	
//			if(x>1)
//			{
//				x=0;
//				
//				/*此程序可确保油门安全*/
//				switch(Accelerator)					
//				{
//					case 1:if(SetSpeed<300) SetSpeed += 1;
//						break;
//					case 2:if(SetSpeed<400) SetSpeed += 1;
//						break;
//					case 3:if(SetSpeed<500) SetSpeed += 1;
//						break;
//					case 4:if(SetSpeed<600) SetSpeed += 1;
//						break;
//					default:if(SetSpeed<400) SetSpeed += 1;
//				}
//			}
//			x++;
//			
//			leftpwm	+= SetSpeed;
//			rightpwm += SetSpeed;
//		}
//		else
//		{
//			
//			if(SetSpeed>0) SetSpeed -= 1;
//		}
		
		PWM_L = leftpwm;			//电机最终得到的pwm值
		PWM_R = rightpwm;
	}
	else	//关闭电机
	{
		PWM_L = 0;
		PWM_R = 0;
	}
}


/********************************************************************
*
* 中断处理函数
*
*********************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{	
		static short LeftPwm=0, RightPwm=0;
	
		if(ARMED)
		{
			LeftPwm  += Blance(roll[0],Gyro[0][0]);		//左轮平衡环
			RightPwm += Blance(roll[1],Gyro[1][0]);		//右轮平衡环
			
			/*输出限幅*/
			if(LeftPwm>1500) LeftPwm = 1500;
			else if(LeftPwm<-1500) LeftPwm = -1500;
			
			if(RightPwm>1500) RightPwm = 1500;
			else if(RightPwm<-1500) RightPwm = -1500;
			
			start_machine(LeftPwm, RightPwm, roll[0],roll[1]);	//开启电机
			
//			printf("\r\n%d\t%d\r\n",LeftPwm,RightPwm);
		}
		else	//关闭电机
		{
			PWM_L = 0;
			PWM_R = 0;
			
			LeftPwm = 0;
			RightPwm = 0;
		}
		
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
