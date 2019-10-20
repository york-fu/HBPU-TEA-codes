#include "sys.h"
#include "usart.h"
#include "control.h"


 
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
		
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
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
} 
#endif	



/*m^n函数，返回值:m^n次方*/
double Usart_Pow(double m,u8 n)
{
	double result=1;	 
	while(n--) result*=m;    
	return result;
}


/*串口调试参数函数*/
void Usart1_Dispose()	//串口数据处理
{
	u8 i,n,digit=0,scope=0;
	u8 usart_flag=0,point_flag=0;
	u8 x=0;
	u8 x1=0,x2=0,x3=0,x4=0,x5=0;
	u8 x6=0,x7=0,x8=0,x9=0,x10=0,x11=0;
	u8 buf_1[]="UP:";
	u8 buf_2[]="UD:";
	u8 buf_3[]="VP:";
	u8 buf_4[]="VI:";
	u8 buf_5[]="s1:";
	u8 buf_6[]="s2:";
	u8 buf_7[]="s3:";
	u8 buf_8[]="s4:";
	u8 buf_9[]="s5:";
	u8 buf_10[]="s6:";
	u8 buf_11[]="st:";
	double temp=0;
	static s8 Steertemp = 20;
	
	if((USART_RX_STA&0x8000)==0x8000)	//数据接收完成
	{
		/*判断数据格式*/
		for(i=0;i<3;i++)		
		{
			if(USART_RX_BUF[i]==buf_1[i])  x1++;
			if(USART_RX_BUF[i]==buf_2[i])  x2++;
			if(USART_RX_BUF[i]==buf_3[i])  x3++;
			if(USART_RX_BUF[i]==buf_4[i])  x4++;
			if(USART_RX_BUF[i]==buf_5[i])  x5++;
			if(USART_RX_BUF[i]==buf_6[i])  x6++;
			if(USART_RX_BUF[i]==buf_7[i])  x7++;
			if(USART_RX_BUF[i]==buf_8[i])  x8++;
			if(USART_RX_BUF[i]==buf_9[i])  x9++;
			if(USART_RX_BUF[i]==buf_10[i]) x10++;
			if(USART_RX_BUF[i]==buf_11[i]) x11++;
		}
		
		if(x1==3||x2==3||x3==3||x4==3||x5==3||x6==3||x7==3||x8==3||x9==3||x10==3||x11==3)	//格式正确,处理数据
		{
			if(x1==3)
			{
				usart_flag = 1;
				printf("UP: ");
			}	
			else if(x2==3)
			{
				usart_flag = 2;
				printf("UD: ");
			}
			else if(x3==3)
			{
				usart_flag = 3;
				printf("VP: ");
			}
			else if(x4==3)
			{
				usart_flag = 4;
				printf("VI: ");
			}
			else if(x5==3)
			{
				usart_flag = 5;
				printf("格式正确！");
			}
			else if(x6==3)
			{
				usart_flag = 6;
				printf("格式正确！");
			}
			else if(x7==3)
			{
				usart_flag = 7;
				printf("格式正确！");
			}
			else if(x8==3)
			{
				usart_flag = 8;
				printf("格式正确！");
			}
			else if(x9==3)
			{
				usart_flag = 9;
				printf("格式正确！");
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
			while(USART_RX_BUF[i]!='\0')
			{
				if((USART_RX_BUF[i]>='0'&&USART_RX_BUF[i]<='9') || USART_RX_BUF[i]=='.')
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
			}
			
			/*解码数据*/
			if(scope)
			{
				i = 3;
				while(USART_RX_BUF[i]!='\0')
				{
					if(USART_RX_BUF[i]=='.')  point_flag = i;	//标记小数点（加标示）
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
						temp += (USART_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					/*计算小数部分*/
					n = 1;
					for(i=point_flag+1;i<digit;i++)
					{
						temp += (USART_RX_BUF[i]-'0') * Usart_Pow(0.1,n++);
					}
					printf("%lf\t",temp);
				}
				else	//没有小数点
				{
					n = digit-1-3;
					temp = 0;
					for(i=3;i<digit;i++)	
					{
						temp += (USART_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					printf("%lf\t",temp);	
				}
				printf("\r\n");	
			}
			
			switch(usart_flag)
			{
				/*PID调节*/
				case 1:upPD.kp = temp;printf("UP = %f\r\n",upPD.kp);
					break;
				case 2:upPD.kd = temp;printf("UD = %f\r\n",upPD.kd);
					break;
				case 3:velocityPI.kp = temp;printf("VP = %f\r\n",velocityPI.kp);
					break;
				case 4:velocityPI.ki = temp;printf("VI = %f\r\n",velocityPI.ki);
					break;
				
				/*前后左右控制*/
				case 5:Steer = -Steertemp;		//平衡车前进
					break;
				case 6:Steer = Steertemp;		//平衡车后退
					break;
				case 7:TurnValue=-2000;		//平衡车转向角速度 
					break;
				case 8:TurnValue=2000;
					break;
				
				/*运动速度控制*/
				case 9:Steertemp+=10;
					if(Steertemp>100)  Steertemp = 100;
					break;
				case 10:Steertemp-=10;
					if(Steertemp<0)  Steertemp = 0;
					break;

				default:Steer = 0, TurnValue = 0;
					break;					
			}
		}
		else  printf("格式错误，请重新发送！！！\r\n");
		
		printf("\r\n");
		
		/*清除接收BUF*/
		i=0;
		while(USART_RX_BUF[i]!='\0')
		{
			USART_RX_BUF[i] = 0;
			i++;
		}
		USART_RX_STA = 0;		//接收状态和数据位数清0
	}
}

