#ifndef __FILTER_H
#define __FILTER_H
#include "sys.h"
#include "control.h"


void integral_limit(SITE *variable, char mode);		//积分上限  
u8 MPU6050_data_filter(SITE *angle, SITE *gyro);	// mpu6050数据获取和一阶滤波


#endif

