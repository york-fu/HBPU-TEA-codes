#include "sys.h"
#include "usart.h"	
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "blance.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

USART_TypeDef* UARTN[UART_MAX]= {USART1, USART2, USART3, UART4, UART5}; //定义串口号

/***********************************************************************************************************
 *  @brief      I/O口初始化函数
 *  @since      v5.0
 ***********************************************************************************************************/
void port_init(GPIO_TypeDef* GPIO_TX, uint16_t GPIO_Pin_TX, GPIO_TypeDef* GPIO_RX, uint16_t GPIO_Pin_RX, uint8_t  USART_IRQn)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_TX; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIO_TX, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_RX;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIO_RX, &GPIO_InitStructure);//初始化GPIOA.10 

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
/***********************************************************************************************************
 *  @brief      串口初始化函数
 *  @since      v5.0
 ***********************************************************************************************************/
void uart_init(UARTn_e uratn, u32 bound)
{
	USART_InitTypeDef USART_InitStructure;	

	switch(uratn)
	{
	case UAR1:
		//GPIO端口设置
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		port_init(GPIOA,GPIO_Pin_9, GPIOA, GPIO_Pin_10,USART1_IRQn);	//初始化对应引脚
		break;
	case UAR2:
		//GPIO端口设置
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		port_init(GPIOA,GPIO_Pin_2, GPIOA, GPIO_Pin_3,USART2_IRQn);	//初始化对应引脚
		break;
	case UAR3:
		//GPIO端口设置
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART1，GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		port_init(GPIOB,GPIO_Pin_10, GPIOB,GPIO_Pin_11,USART3_IRQn);	//初始化对应引脚
		break;
//	case UAR4:
//		//GPIO端口设置
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能USART1，GPIOA时钟
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//		port_init(GPIOC,GPIO_Pin_10, GPIOC,GPIO_Pin_11,UART4_IRQn);	//初始化对应引脚
//		break;
//	case UAR5:
//		//GPIO端口设置
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能USART1，GPIOA时钟
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
//		port_init(GPIOC,GPIO_Pin_12, GPIOD, GPIO_Pin_2,UART5_IRQn);	//初始化对应引脚
//		break;
	default:
        break;	
	}
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(UARTN[uratn], &USART_InitStructure); //初始化串口uratn
	USART_ITConfig(UARTN[uratn], USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(UARTN[uratn], ENABLE);                    //使能串口uratn 
}

/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
//注意,读取USARTx->SR能避免莫名其妙的错误 

#if Open_UAR1   //如果打开了串口1

u8 USART1_RX_BUF[USART_REC_LEN];     	//接收缓冲,最大USART_REC_LEN个字节.   
u16 USART1_RX_STA=0;       				//接收状态标记 bit15，	接收完成标志
													//bit14，	接收到0x0d
													//bit13~0，	接收到的有效字节数目

/*****************************************************
 *  @brief      串口1中断服务函数
 *  @since      v5.0
 *****************************************************/
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART1_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART1_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else USART1_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*函数名称:	uart_init
*函数功能:	串口初始化
*函数参数: 	UARTn_e       	模块号UAR1~UART5
			bound			波特率
*返 回 值:	无
***********************************************************************/
#if Open_UAR2
u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.   
u16 USART2_RX_STA=0;       			//接收状态标记 bit15，	接收完成标志
												 //bit14，	接收到0x0d
												 //bit13~0，	接收到的有效字节数目

/********************************************
 *  @brief      串口2中断服务函数
 *  @since      v5.0
 ********************************************/
void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
	
/*****************************************************
 *  @brief      串口2打印函数
 *  @since      v5.0
 *****************************************************/
void u2_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART2_TX_BUF[50]; 	//定义发送数组，存发送数据
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);
	for(j=0;j<i;j++)
	{
		USART2->DR=USART2_TX_BUF[j];//把串口1的数据发送到串口2
			while((USART2->SR&0X40)==0);//等待发送结束
	}
}
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*函数名称:	uart_init
*函数功能:	串口初始化
*函数参数: 	UARTn_e       	模块号UAR1~UART5
			bound			波特率
*返 回 值:	无
***********************************************************************/
#if Open_UAR3
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.   
u16 USART3_RX_STA=0;       			//接收状态标记 bit15，	接收完成标志
												 //bit14，	接收到0x0d
												 //bit13~0，	接收到的有效字节数目

/********************************************
 *  @brief      串口3中断服务函数
 *  @since      v5.0
 ********************************************/
void USART3_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 


/*****************************************************
 *  @brief      串口3打印函数
 *  @since      v5.0
 *****************************************************/
void u3_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART3_TX_BUF[50]; 	//定义发送数组，存发送数据
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);
	for(j=0;j<i;j++)
	{
		USART3->DR=USART3_TX_BUF[j];//把串口1的数据发送到串口2
			while((USART3->SR&0X40)==0);//等待发送结束
	}
}
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*函数名称:	uart_init
*函数功能:	串口初始化
*函数参数: 	UARTn_e       	模块号UAR1~UART5
			bound			波特率
*返 回 值:	无
***********************************************************************/
#if Open_UAR4
u8 USART4_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.   
u16 USART4_RX_STA=0;       			//接收状态标记 bit15，	接收完成标志
												 //bit14，	接收到0x0d
												 //bit13~0，	接收到的有效字节数目

/********************************************
 *  @brief      串口4中断服务函数
 *  @since      v5.0
 ********************************************/
void UART4_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(UART4);	//读取接收到的数据
		
		if((USART4_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART4_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART4_RX_STA=0;//接收错误,重新开始
				else USART4_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART4_RX_STA|=0x4000;
				else
					{
					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=Res ;
					USART4_RX_STA++;
					if(USART4_RX_STA>(USART_REC_LEN-1))USART4_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 


/*****************************************************
 *  @brief      串口4打印函数
 *  @since      v5.0
 *****************************************************/
void u4_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART4_TX_BUF[50]; 	//定义发送数组，存发送数据
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART4_TX_BUF);
	for(j=0;j<i;j++)
	{
		UART4->DR=USART4_TX_BUF[j];//把串口1的数据发送到串口2
			while((UART4->SR&0X40)==0);//等待发送结束
	}
}
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*函数名称:	uart_init
*函数功能:	串口初始化
*函数参数: 	UARTn_e       	模块号UAR1~UART5
			bound			波特率
*返 回 值:	无
***********************************************************************/
#if Open_UAR5
u8 USART5_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.   
u16 USART5_RX_STA=0;       			//接收状态标记 bit15，	接收完成标志
												 //bit14，	接收到0x0d
												 //bit13~0，	接收到的有效字节数目

/********************************************
 *  @brief      串口4中断服务函数
 *  @since      v5.0
 ********************************************/
void UART5_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(UART5);	//读取接收到的数据
		
		if((USART5_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART5_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART5_RX_STA=0;//接收错误,重新开始
				else USART5_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART5_RX_STA|=0x4000;
				else
					{
					USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
					USART5_RX_STA++;
					if(USART5_RX_STA>(USART_REC_LEN-1))USART5_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 


/*****************************************************
 *  @brief      串口4打印函数
 *  @since      v5.0
 *****************************************************/
void u5_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART5_TX_BUF[50]; 	//定义发送数组，存发送数据
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART5_TX_BUF);
	for(j=0;j<i;j++)
	{
		UART5->DR=USART5_TX_BUF[j];//把串口1的数据发送到串口2
			while((UART5->SR&0X40)==0);//等待发送结束
	}
}
#endif

/*m^n函数，返回值:m^n次方*/
double Usart_Pow(double m,u8 n)
{
	double result=1;	 
	while(n--) result*=m;    
	return result;
}

/**********************************************************************
*函数名称:	Uart_DealDate
*函数功能:	串口处理数据
*函数参数: 	USARTx_RX_STA	接收状态标记
*返 回 值:	无
***********************************************************************/

void Uart_DealDate()
{	 
	u8 i,n,digit=0,scope=0;
	u8 usart_flag=0,point_flag=0;
	u8 x=0;
	u8 x1=0,x2=0,x3=0,x4=0,x5=0;
	u8 x6=0,x7=0,x8=0,x9=0,x10=0,x11=0;
	u8 buf_1[]="P1:";
	u8 buf_2[]="I1:";
	u8 buf_3[]="D1:";
	u8 buf_4[]="P2:";
	u8 buf_5[]="I2:";
	u8 buf_6[]="D2:";
	u8 buf_7[]="EN:";
	u8 buf_8[]="SS:";
	u8 buf_9[]="XX:";
	u8 buf_10[]="XX:";
	u8 buf_11[]="XX:";
	double temp=0;
	
	if((USART1_RX_STA&0x8000)==0x8000)	//数据接收完成
	{
		/*判断数据格式*/
		for(i=0;i<3;i++)		
		{
			if(USART1_RX_BUF[i]==buf_1[i])  x1++;
			if(USART1_RX_BUF[i]==buf_2[i])  x2++;
			if(USART1_RX_BUF[i]==buf_3[i])  x3++;
			if(USART1_RX_BUF[i]==buf_4[i])  x4++;
			if(USART1_RX_BUF[i]==buf_5[i])  x5++;
			if(USART1_RX_BUF[i]==buf_6[i])  x6++;
			if(USART1_RX_BUF[i]==buf_7[i])  x7++;
			if(USART1_RX_BUF[i]==buf_8[i])  x8++;
			if(USART1_RX_BUF[i]==buf_9[i])  x9++;
			if(USART1_RX_BUF[i]==buf_10[i]) x10++;
			if(USART1_RX_BUF[i]==buf_11[i]) x11++;
		}
		
		if(x1==3||x2==3||x3==3||x4==3||x5==3||x6==3||x7==3||x8==3||x9==3||x10==3||x11==3)	//格式正确,处理数据
		{
			if(x1==3)
			{
				usart_flag = 1;
				printf("P1: ");
			}	
			else if(x2==3)
			{
				usart_flag = 2;
				printf("I1: ");
			}
			else if(x3==3)
			{
				usart_flag = 3;
				printf("D1: ");
			}
			else if(x4==3)
			{
				usart_flag = 4;
				printf("P2: ");
			}
			else if(x5==3)
			{
				usart_flag = 5;
				printf("I2: ");
			}
			else if(x6==3)
			{
				usart_flag = 6;
				printf("D2: ");
			}
			else if(x7==3)
			{
				usart_flag = 7;
				printf("EN: ");
			}
			else if(x8==3)
			{
				usart_flag = 8;
				printf("SS: ");
			}
			else if(x9==3)
			{
				usart_flag = 9;
				printf("XX: ");
			}
			else if(x10==3)
			{
				usart_flag = 10;
				printf("格式正确！");
			}
			else if(x11==3)
			{
				usart_flag = 11;
				printf("格式正确！");
			}
			
			/*判断数据范围*/
			i = 3;
			while(USART1_RX_BUF[i]!='\0')
			{
				if((USART1_RX_BUF[i]>='0'&&USART1_RX_BUF[i]<='9') || USART1_RX_BUF[i]=='.')
				{
					x++;
				}
				i++;
			}
			if(x+3==i)	//范围正常
			{
				scope = 1;		//标记范围正常
			}
			else
			{
				printf("数据异常,请重新发送！！！\r\n");
				usart_flag = 0;
			}
			
			/*解码数据*/
			if(scope)
			{
				i = 3;
				while(USART1_RX_BUF[i]!='\0')
				{
					if(USART1_RX_BUF[i]=='.')  point_flag = i;	//标记小数点（加标示）
					i++;
				}
				digit = i;		//记录位数（加标示）
				
				if(point_flag!=0)	//有小数点
				{
					/*计算整数部分*/
					n = point_flag-1-3;
					temp = 0;
					for(i=3;i<point_flag;i++)	
					{
						temp += (USART1_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					/*计算小数部分*/
					n = 1;
					for(i=point_flag+1;i<digit;i++)
					{
						temp += (USART1_RX_BUF[i]-'0') * Usart_Pow(0.1,n++);
					}
					printf("%lf\t\r\n",temp);
				}
				else	//没有小数点
				{
					n = digit-1-3;
					temp = 0;
					for(i=3;i<digit;i++)	
					{
						temp += (USART1_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					printf("%lf\t\r\n",temp);	
				}	
			}
			
			switch(usart_flag)
			{
				/*PID调节*/
				case 1:BlancePid.P = temp;printf("BlancePid.P = %f\r\n",BlancePid.P);
					break;
				case 2:BlancePid.I = temp;printf("BlancePid.I = %f\r\n",BlancePid.I);
					break;
				case 3:BlancePid.D = temp;printf("BlancePid.D = %f\r\n",BlancePid.D);
					break;
				
				case 7:ARMED = temp;printf("ARMED = %d\r\n",ARMED);
					break;
				case 8:Accelerator = temp;printf("Accelerator = %d\r\n",Accelerator);
					break;
				
				default:break;
			}
		}
		else  printf("格式错误，请重新发送！！！\r\n");
		
		printf("\r\n");
		
		/*清除接收BUF*/
		i=0;
		while(USART1_RX_BUF[i]!='\0')
		{
			USART1_RX_BUF[i] = 0;
			i++;
		}
		USART1_RX_STA = 0;		//接收状态和数据位数清0
	}
}
