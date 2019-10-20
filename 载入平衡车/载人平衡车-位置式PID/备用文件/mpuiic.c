#include "mpuiic.h"
#include "delay.h"
 
  //MPU IIC 延时函数
void MPU_IIC_Delay(void)
{
	delay_us(2);
}

//初始化IIC
void MPU_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//先使能外设IO PORTC时钟 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	
  GPIO_SetBits(GPIOA,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5);						 //PB10,PB11 输出高	
 
}
//产生IIC起始信号
void MPU_IIC_Start(u8 witch)
{
	if(witch == 1)
	{
		MPU1_SDA_OUT();     //sda线输出
		MPU1_IIC_SDA=1;	  	  
		MPU1_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU1_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
		MPU_IIC_Delay();
		MPU1_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	}else if(witch == 2)
	{
		MPU2_SDA_OUT();     //sda线输出
		MPU2_IIC_SDA=1;	  	  
		MPU2_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU2_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
		MPU_IIC_Delay();
		MPU2_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	}
	
}	  
//产生IIC停止信号
void MPU_IIC_Stop(u8 witch)
{
	if(witch == 1)
	{
		MPU1_SDA_OUT();//sda线输出
		MPU1_IIC_SCL=0;
		MPU1_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		MPU_IIC_Delay();
		MPU1_IIC_SCL=1;  
		MPU1_IIC_SDA=1;//发送I2C总线结束信号
		MPU_IIC_Delay();
	}else if(witch == 2)
	{	
		MPU2_SDA_OUT();//sda线输出
		MPU2_IIC_SCL=0;
		MPU2_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		MPU_IIC_Delay();
		MPU2_IIC_SCL=1;  
		MPU2_IIC_SDA=1;//发送I2C总线结束信号
		MPU_IIC_Delay();
	}
								   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MPU_IIC_Wait_Ack(u8 witch)
{
	u8 ucErrTime=0;
	if(witch == 1)
	{
		MPU1_SDA_IN();      //SDA设置为输入  
		MPU1_IIC_SDA=1;MPU_IIC_Delay();	   
		MPU1_IIC_SCL=1;MPU_IIC_Delay();	 
		while(MPU1_READ_SDA)
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				MPU_IIC_Stop(1);
				return 1;
			}
		}
		MPU1_IIC_SCL=0;//时钟输出0 	   
		return 0; 
		
	}else if(witch == 2)
	{
		MPU2_SDA_IN();      //SDA设置为输入  
		MPU2_IIC_SDA=1;MPU_IIC_Delay();	   
		MPU2_IIC_SCL=1;MPU_IIC_Delay();	 
		while(MPU2_READ_SDA)
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				MPU_IIC_Stop(2);
				return 1;
			}
		}
		MPU2_IIC_SCL=0;//时钟输出0 	   
		return 0;  
	}	 
} 
//产生ACK应答
void MPU_IIC_Ack(u8 witch)
{
	if(witch == 1)
	{
		MPU1_IIC_SCL=0;
		MPU1_SDA_OUT();
		MPU1_IIC_SDA=0;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=0;
	}else if(witch == 2)
	{
		MPU2_IIC_SCL=0;
		MPU2_SDA_OUT();
		MPU2_IIC_SDA=0;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=0;
	}
	
}
//不产生ACK应答		    
void MPU_IIC_NAck(u8 witch)
{
	if(witch == 1)
	{
		MPU1_IIC_SCL=0;
		MPU1_SDA_OUT();
		MPU1_IIC_SDA=1;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=0;
	}else if(witch == 2)
	{
		MPU2_IIC_SCL=0;
		MPU2_SDA_OUT();
		MPU2_IIC_SDA=1;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=0;
	}
	
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MPU_IIC_Send_Byte(u8 txd, u8 witch)
{  
	u8 t; 
	if(witch == 1)
	{  
		MPU1_SDA_OUT(); 	    
		MPU1_IIC_SCL=0;//拉低时钟开始数据传输
		for(t=0;t<8;t++)
		{              
			MPU1_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			MPU1_IIC_SCL=1;
			MPU_IIC_Delay(); 
			MPU1_IIC_SCL=0;	
			MPU_IIC_Delay();
		}
	}else if(witch == 2)
	{   
		MPU2_SDA_OUT(); 	    
		MPU2_IIC_SCL=0;//拉低时钟开始数据传输
		for(t=0;t<8;t++)
		{              
			MPU2_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			MPU2_IIC_SCL=1;
			MPU_IIC_Delay(); 
			MPU2_IIC_SCL=0;	
			MPU_IIC_Delay();
		}	
	}
    	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack, u8 witch)
{
	unsigned char i,receive=0;
	if(witch == 1)
	{
		MPU1_SDA_IN();//SDA设置为输入
		for(i=0;i<8;i++ )
		{
			MPU1_IIC_SCL=0; 
			MPU_IIC_Delay();
			MPU1_IIC_SCL=1;
			receive<<=1;
			if(MPU1_READ_SDA)receive++;   
			MPU_IIC_Delay(); 
		}					 
		if (!ack)
			MPU_IIC_NAck(1);//发送nACK
		else
			MPU_IIC_Ack(1); //发送ACK   
		return receive;
	}else if(witch == 2)
	{
		MPU2_SDA_IN();//SDA设置为输入
		for(i=0;i<8;i++ )
		{
			MPU2_IIC_SCL=0; 
			MPU_IIC_Delay();
			MPU2_IIC_SCL=1;
			receive<<=1;
			if(MPU2_READ_SDA)receive++;   
			MPU_IIC_Delay(); 
		}					 
		if (!ack)
			MPU_IIC_NAck(2);//发送nACK
		else
			MPU_IIC_Ack(2); //发送ACK   
		return receive;
	}
	
}

















