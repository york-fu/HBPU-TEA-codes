#include "usart.h"
#include "delay.h"
#include "lcd.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
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

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量(使用回调函数处理中断需要调用该函数）
  
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
	}

}


 


//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART1_Handler);	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif	


void uart1_send_data(u16 data1, u16 data2, u16 data3, u16 data4)
{
	u8 TX_BUF[11];
	
	TX_BUF[0] = 0xff;
	TX_BUF[1] = data1>>8;
	TX_BUF[2] = data1&0x0f;
	TX_BUF[3] = data2>>8;
	TX_BUF[4] = data2&0x0f;
	TX_BUF[5] = data3>>8;
	TX_BUF[6] = data3&0x0f;
	TX_BUF[7] = data4>>8;
	TX_BUF[8] = data4&0x0f;
	TX_BUF[9] = '\r';
	TX_BUF[10] = '\n';
	
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)TX_BUF,11,1000);	//发送数据
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
}


/*********************************************************************/
//自行添加代码

UART_HandleTypeDef UART2_Handle;	//串口2句柄

/*串口2初始化*/
//bound:波特率
void MyUSART2_Init(u32 bound)
{	
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
	__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	
	GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2

	GPIO_Initure.Pin=GPIO_PIN_3;			//PA3
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
		
	//UART 初始化设置
	UART2_Handle.Instance=USART2;					    //USART2
	UART2_Handle.Init.BaudRate=bound;				    //波特率
	UART2_Handle.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	UART2_Handle.Init.StopBits=UART_STOPBITS_1;			//一个停止位
	UART2_Handle.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	UART2_Handle.Init.Mode=UART_MODE_TX_RX;		    	//收发模式
	HAL_UART_Init(&UART2_Handle);					    //HAL_UART_Init()会使能UART1
	
	__HAL_UART_DISABLE_IT(&UART2_Handle,UART_IT_TC);	//关闭发送中断
//	__HAL_UART_DISABLE_IT(&UART2_Handle,UART_IT_RXNE);	//关闭接收中断
	__HAL_UART_ENABLE_IT(&UART2_Handle,UART_IT_RXNE);	//开启接收中断
	HAL_NVIC_EnableIRQ(USART2_IRQn);					//使能USART2中断通道
	HAL_NVIC_SetPriority(USART2_IRQn,0,1);				//抢占优先级2，子优先级2
}

/*空函数*/
void My_NOP()
{}

u8 U2Rx_sta = 0;	//串口2接收标记
	
/*接收BUF*/
u8 RX_buf[RxSize]={0};

//串口2中断服务程序
void USART2_IRQHandler(void)                	
{ 
	static u16 i=0;
		
	if(USART2->SR & (1<<5))  //接收中断
	{
		RX_buf[i] = USART2->DR;
		i++;
		
		if(i==RxSize)
		{	
			
			for(i=0;i<RxSize;i+=2)
			{
				LCD->LCD_RAM = (RX_buf[i]<<8)+RX_buf[i+1];
			}			
			i = 0;
			USART2->DR = 0xf0;
			while(!(USART2->SR & (1<<6)));	//等待发送完成
		}
	}
	USART2->SR &= ~(1<<5);	//清除中断标志位
}




/*串口发送数据*/
void MyUSART_SendData(u8* pbuf,u16 len)
{
	u16 x;
	for(x=0;x<len;x++)
	{
		USART2->DR = *pbuf;
		pbuf++;
		while(!(USART2->SR & (1<<6)));	//等待发送完成
	}
}

