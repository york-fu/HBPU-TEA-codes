#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h"


/************************************用户配置************************************/
#define Open_UAR1       1  		//打开串口1，1打开 0关闭，只有打开才能使用串口1中断服务函数
#define Open_UAR2      	0		//打开串口2，1打开 0关闭
#define Open_UAR3       0  		//打开串口3，1打开 0关闭
#define Open_UAR4      	0		//打开串口4，1打开 0关闭
#define Open_UAR5       0  		//打开串口5，1打开 0关闭
#define USART_REC_LEN  	200  	//定义最大接收字节数 200


/************************************内、外部调用************************************/
typedef enum
{
    UAR1,
    UAR2,
    UAR3,
    UAR4,
    UAR5,

    UART_MAX,
} UARTn_e;
/************************************外部调用************************************/	  	
extern u8  USART1_RX_BUF[USART_REC_LEN]; 	//接收到的数据在这里
extern u16 USART1_RX_STA;         			//接收状态标记
extern u8  USART2_RX_BUF[USART_REC_LEN]; 	//接收到的数据在这里
extern u16 USART2_RX_STA;         			//接收状态标记
extern u8  USART3_RX_BUF[USART_REC_LEN]; 	//接收到的数据在这里
extern u16 USART3_RX_STA;         			//接收状态标记	
extern u8  USART4_RX_BUF[USART_REC_LEN]; 	//接收到的数据在这里
extern u16 USART4_RX_STA; 					//接收状态标记
extern u8  USART5_RX_BUF[USART_REC_LEN]; 	//接收到的数据在这里
extern u16 USART5_RX_STA; 					//接收状态标记


/************************************函数外部调用***********************************/	
/**********************************************************************
*函数名称:	uart_init
*函数功能:	串口初始化
*函数参数: 	UARTn_e       	模块号UAR1~UART5
			bound			波特率
*返 回 值:	无
***********************************************************************/
void uart_init(UARTn_e uratn, u32 bound);	
/**********************************************************************
*函数名称:	u2_printf
*函数功能:	串口2打印函数
*函数参数: 	跟 printf 用法相同
*返 回 值:	无
***********************************************************************/
void u2_printf(char* fmt,...);	
void u3_printf(char* fmt,...);
void u4_printf(char* fmt,...);
void u5_printf(char* fmt,...);

/**********************************************************************
*函数名称:	Uart_DealDate
*函数功能:	串口处理数据
*函数参数: 	USARTx_RX_STA	接收状态标记
*返 回 值:	无
***********************************************************************/
void Uart_DealDate(void);//用户自行添加修改
#endif


