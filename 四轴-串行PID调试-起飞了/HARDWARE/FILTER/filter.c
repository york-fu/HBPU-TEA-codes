#include "filter.h"
#include "inv_mpu.h"

/******************
* 积分上限
* mode==0为内环积分限幅  
* mode==1为外环积分限幅
*******************/
void integral_limit(SITE *variable, char mode)     
{                                                
	SITE inner_I_max = {1000, 1000, 1000},
		 outer_I_max = {1000, 1000, 0};
	if(mode==0)  // 内环
	{
		if( variable->x > inner_I_max.x )
		{
			variable->x = inner_I_max.x;
		}
		if( variable->y > inner_I_max.y )
		{
			variable->y = inner_I_max.y;
		}
		if( variable->z > inner_I_max.z )
		{	
			variable->z = inner_I_max.z;
		}
		if( variable->x < -inner_I_max.x )  
		{
			variable->x = -inner_I_max.x;
		}
		if( variable->y < -inner_I_max.y )
		{
			variable->y = -inner_I_max.y;
		}
		if( variable->z < -inner_I_max.z )
		{
			variable->z = -inner_I_max.z;
		}
	}
	if(mode==1)  // 外环
	{
		if( variable->x > outer_I_max.x )
		{
			variable->x = outer_I_max.x;
		}
		if( variable->y > outer_I_max.y )
		{
			variable->y = outer_I_max.y;
		}
		if( variable->x < -outer_I_max.x )
		{
			variable->x = -outer_I_max.x;
		}
		if( variable->y < -outer_I_max.y )
		{
			variable->y = -outer_I_max.y;
		}
	}
}

/***************************
* mpu6050数据获取和一阶滤波
* 读取完毕 return 0；
* 读取错误 return 其他；
****************************/
u8 MPU6050_data_filter(SITE *angle, SITE *gyro)
{
	SITE MpuAngle1, MpuGyro1;
	if(mpu_dmp_get_data(&MpuAngle1, &MpuGyro1)==0)  // 读出欧拉角和陀螺仪数据
	{
		angle->x = angle->x * 0.7 + MpuAngle1.x * 0.3;
		angle->y = angle->y * 0.7 + MpuAngle1.y * 0.3;
		angle->z = angle->z * 0.7 + MpuAngle1.z * 0.3;

		gyro->x = gyro->x * 0.7 + MpuGyro1.x * 0.3;
		gyro->y = gyro->y * 0.7 + MpuGyro1.y * 0.3;
		gyro->z = gyro->z * 0.7 + MpuGyro1.z * 0.3;
		
		return 0;
	}
	return 1;
}

