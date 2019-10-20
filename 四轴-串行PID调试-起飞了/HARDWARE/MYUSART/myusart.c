#include "myusart.h"
#include "usart.h"
#include "control.h"

u8 uSendBuf[UartDataNum*2]={0};
u8 FreeCarsDataNum=UartDataNum*2;

void USendOneByte(u8 date)
{
	USART1->DR=date;
	while((USART1->SR&0X40)==0);
}

void sendDataToScope(void)  //  数据发送协议
{
	s8 i,sum=0;
	USendOneByte(251);
	USendOneByte(109);
	USendOneByte(37);
	sum+=(251);
	sum+=(109);
	sum+=(37);
	for(i=0;i<FreeCarsDataNum;i++) 
	{ 
		USendOneByte(uSendBuf[i]); 
		sum+=uSendBuf[i];
	}
	USendOneByte(sum);  
}

/*
  向上位机某个通道缓冲区填充数据
  chanel：通道 
  data：  数据 16位
*/ 
void push(u8 chanel,u16 data) 
{ 
	uSendBuf[chanel*2]=data/256; 
	uSendBuf[chanel*2+1]=data%256; 
}

void PC_data(void)  // 给上位机发送欧拉角数据
{
	push(0,(s16)Angle.x*10);
	push(1,(s16)Angle.y*10);
	push(2,(s16)Angle.z*10);
	push(3,(s16)Palstance.x);
	sendDataToScope();  // 数据发送协议
}

/*
  串口数据处理

  -->> 输入格式 
				
  修改PID参数：
	-- PR0.00 -- 
	前两位表示要修改的数
	后面为要修改的值

  调节油门：
	-- T+ --
	油门加10
	-- T- --
	油门加10
	-- TH1200 --
	油门值

  开关电机：
	-- O -- 开 
	-- F -- 关

  查看数据：
    -- C --
*/
void USART_Adjust(void)  
{
	if(USART_RX_STA&0x8000)
	{				
		u8 i;
		for(i=2; i<=5; i++ )  // 数字字符转数字
		{
			USART_RX_BUF[i] = USART_RX_BUF[i] - 0x30;
		}
		if(USART_RX_BUF[0] == 'P')
		{
			if(USART_RX_BUF[1] == 'R')  // PID_ROL
			{
				INNER_X.P = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
				INNER_Y.P = INNER_X.P;
                printf("\r\nPID_ROL -P:%4.2f\r\n",INNER_X.P);
			}
			else if(USART_RX_BUF[1] == 'Y')  // PID_YAW
			{
				INNER_Z.P = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
                printf("\r\nPID_YAW -P:%4.2f\r\n",INNER_Z.P);
			}
			else if(USART_RX_BUF[1] == 'O')  // PID_ROL  // 外环
			{
				OUTER_X.P = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
				OUTER_Y.P = OUTER_X.P;
                printf("\r\nPID_ROL -D:%4.2f\r\n",OUTER_X.P);
			}
		}
		else if(USART_RX_BUF[0] == 'I')
		{
			if(USART_RX_BUF[1] == 'R')  // PID_ROL  // 内环
			{
				INNER_X.I = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
				INNER_Y.I = INNER_X.I;
                printf("\r\nPID_ROL -I:%4.2f\r\n",INNER_X.I);
			}
			else if(USART_RX_BUF[1] == 'Y')  // PID_YAW
			{
				INNER_Z.I = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
                printf("\r\nPID_YAW -I:%4.2f\r\n",INNER_Z.I);
			}
			else if(USART_RX_BUF[1] == 'O')  // PID_ROL  // 外环
			{
				OUTER_X.I = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
				OUTER_Y.I = OUTER_X.I;
                printf("\r\nPID_ROL -I:%4.2f\r\n",OUTER_X.I);
			}
		}
		else if(USART_RX_BUF[0] == 'D')
		{
			if(USART_RX_BUF[1] == 'R')  // PID_ROL  // 内环
			{
				INNER_X.D = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
				INNER_Y.D = INNER_X.D;
                printf("\r\nPID_ROL -D:%4.2f\r\n",INNER_X.D);
			}
			else if(USART_RX_BUF[1] == 'Y')  // PID_YAW
			{
				INNER_Z.D = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
                printf("\r\nPID_YAW -D:%4.2f\r\n",INNER_Z.D);
			}
			else if(USART_RX_BUF[1] == 'O')  // PID_ROL  // 外环
			{
				OUTER_X.D = (float)( USART_RX_BUF[2]*100 + USART_RX_BUF[4]*10 + USART_RX_BUF[5] ) / 100;
				OUTER_Y.D = OUTER_X.D;
                printf("\r\nPID_ROL -D:%4.2f\r\n",OUTER_X.D);
			}
		}
		else if(USART_RX_BUF[0] == 'C')  // 数据查看
		{
			printf("\r\n");
			printf("内环：\r\n");
			printf("PID_ROL -P:%2.2f\r\n        -I:%2.2f\r\n        -D:%2.2f\r\n",INNER_X.P, INNER_X.I, INNER_X.D);
			printf("PID_PIT -P:%2.2f\r\n        -I:%2.2f\r\n        -D:%2.2f\r\n",INNER_Y.P, INNER_Y.I, INNER_Y.D);
			printf("PID_YAW -P:%2.2f\r\n        -I:%2.2f\r\n        -D:%2.2f\r\n",INNER_Z.P, INNER_Z.I, INNER_Z.D);
			printf("外环：\r\n");
			printf("PID_ROL -P:%2.2f\r\n        -I:%2.2f\r\n        -D:%2.2f\r\n",OUTER_X.P, OUTER_X.I, OUTER_X.D);
			printf("PID_PIT -P:%2.2f\r\n        -I:%2.2f\r\n        -D:%2.2f\r\n",OUTER_Y.P, OUTER_Y.I, OUTER_Y.D);
			printf("PID_YAW -P:%2.2f\r\n        -I:%2.2f\r\n        -D:%2.2f\r\n",OUTER_Z.P, OUTER_Z.I, OUTER_Z.D);
		}
		USART_RX_STA=0;
	}
}

