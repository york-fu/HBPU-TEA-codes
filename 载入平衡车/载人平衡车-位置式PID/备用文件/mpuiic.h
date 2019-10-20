#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniSTM32F103开发板
//MPU6050 IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/4/18
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	   		   
//IO方向设置
#define MPU1_SDA_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
#define MPU1_SDA_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;}

//IO操作函数	 
#define MPU1_IIC_SCL    PAout(12) 		//SCL
#define MPU1_IIC_SDA    PAout(11) 		//SDA	 
#define MPU1_READ_SDA   PAin(11) 		//输入SDA 
///////////////////////////////////////////////////////
//IO方向设置
#define MPU2_SDA_IN()  {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=8<<16;}
#define MPU2_SDA_OUT() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=3<<16;}

//IO操作函数	 
#define MPU2_IIC_SCL    PAout(5) 		//SCL
#define MPU2_IIC_SDA    PAout(4) 		//SDA	 
#define MPU2_READ_SDA   PAin(4) 		//输入SDA 

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口

void MPU_IIC_Start(u8 witch);				//发送IIC开始信号
void MPU_IIC_Stop(u8 witch);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd, u8 witch);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack, u8 witch);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(u8 witch); 				//IIC等待ACK信号
void MPU_IIC_Ack(u8 witch);					//IIC发送ACK信号
void MPU_IIC_NAck(u8 witch);				//IIC不发送ACK信号


//void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















