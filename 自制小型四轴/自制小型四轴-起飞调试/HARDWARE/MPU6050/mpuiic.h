#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "sys.h"
#include "usart.h"

/*I2C相关IO外设定义*/
#define I2C_SDA_RCC		RCC_APB2Periph_GPIOC
#define I2C_SDA_GPIO	GPIOC
#define I2C_SDA_PIN		GPIO_Pin_13

#define I2C_SCL_RCC		RCC_APB2Periph_GPIOC 
#define I2C_SCL_GPIO	GPIOC 
#define I2C_SCL_PIN		GPIO_Pin_14

//IO方向设置
#define MPU_SDA_IN()  {GPIOC->CRH&=0XFF0FFFFF; GPIOC->CRH|=0x00800000;}
#define MPU_SDA_OUT() {GPIOC->CRH&=0XFF0FFFFF; GPIOC->CRH|=0x00300000;}	//PC13

//IO操作函数	 
#define MPU_IIC_SCL    PCout(14) 		//SCL
#define MPU_IIC_SDA    PCout(13) 		//SDA	 
#define MPU_READ_SDA   PCin(13) 		//输入SDA

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr, u8 addr);

#endif
