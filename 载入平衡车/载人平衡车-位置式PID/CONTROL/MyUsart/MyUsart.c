#include "myusart.h"
#include "usart.h"
#include "blance.h"

//void USART_Adjust(void)  								//串口修改pid
//{
//	if(USART1_RX_STA&0x8000)
//	{				
//		u8 i;
//		
//		for(i=2; i<=7; i++ )  
//		{
//			USART1_RX_BUF[i] = USART1_RX_BUF[i] - 0x30;  // 数字字符转数字
//		}
//		if(USART1_RX_BUF[0] == 'B')
//		{
//			if(USART1_RX_BUF[1] == 'P')  
//			{
//				BlancePid.P = (float)( USART1_RX_BUF[2]*1000 + USART1_RX_BUF[3]*100 + USART1_RX_BUF[4]*10 + USART1_RX_BUF[6] ) / 10;
//				printf("B-P:%2.2f\r\n",BlancePid.P);
//			}
//			else if(USART1_RX_BUF[1] == 'D')  
//			{
//				BlancePid.D = (float)( USART1_RX_BUF[2]*100 + USART1_RX_BUF[4]*10 + USART1_RX_BUF[5] ) / 100;
//				printf("B-I:%2.2f\r\n",BlancePid.D);
//			}
//		}
//		if(USART1_RX_BUF[0] == 'V')
//		{
//			if(USART1_RX_BUF[1] == 'P')  
//			{
//				VelocityPid.P = (float)( USART1_RX_BUF[2]*1000 + USART1_RX_BUF[3]*100 + USART1_RX_BUF[4]*10 + USART1_RX_BUF[6] ) / 10;
//				printf("V-P:%2.2f\r\n",VelocityPid.P);
//			}
//			else if(USART1_RX_BUF[1] == 'I')  
//			{
//				VelocityPid.I = (float)( USART1_RX_BUF[2] + USART1_RX_BUF[4]*0.1 + USART1_RX_BUF[5] *0.01 + USART1_RX_BUF[6] *0.001);
//				printf("V-I:%.3f\r\n",VelocityPid.I);
//			}
//			else if(USART1_RX_BUF[1] == 'D')  
//			{
//				VelocityPid.D = (float)( USART1_RX_BUF[2]*100 + USART1_RX_BUF[4]*10 + USART1_RX_BUF[5] ) / 10;
//				printf("V-D:%2.2f\r\n",VelocityPid.D);
//			}
//		}/*		
//		if(USART1_RX_BUF[0] == 'P')
//		{
//			if(USART1_RX_BUF[1] == 'i')
//			{
//				Pitch_tagre = USART1_RX_BUF[3] + USART1_RX_BUF[5]*0.1 + USART1_RX_BUF[6]*0.01;
//				printf("Pitch_tagre %f\r\n",Pitch_tagre);
//			}
//		}*/
//		
//		if(USART1_RX_BUF[0] == 'C')
//		{
//			printf("\r\nBP %2.2f\r\n",BlancePid.P);
//			printf("BD %2.3f\r\n",BlancePid.D);
//			printf("vP %2.2f\r\n",VelocityPid.P);
//			printf("vI %.3f\r\n",VelocityPid.I);
//		}
//		USART1_RX_STA=0;
//	}
//}


