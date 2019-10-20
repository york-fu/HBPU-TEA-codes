#ifndef _USART_H
#define _USART_H

#include "sys.h"
#include "stdio.h"	


/*数据接收BUF大小*/
#define RxSize 640*60	
	
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

void uart1_send_data(u16 data1, u16 data2, u16 data3, u16 data4);


/*串口2初始化*/
//bound:波特率
void MyUSART2_Init(u32 bound);


/*串口发送数据*/
void MyUSART_SendBit(u8 dat);

/*串口发送数据*/
void MyUSART_SendData(u8* pbuf,u16 len);

void My_NOP(void);

#endif
