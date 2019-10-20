#include "display.h"
#include "ov7670.h"
#include "24l01.h"
#include "exti.h"
#include "timer.h"
#include "control.h"
#include "usart.h"
#include "delay.h"
#include "dma.h"

u8 TX_sta=1;

/*数据buf*/
u8* TX_buf;

/*图像数据处理*/
void Image_DataHandle(void)
{
	u32 n;
	u16 PixCnt=0;
	
	if(ov_sta==2)
	{		
		NRF24L01_TxPacket(&TX_sta);	//开始下一帧
		
 		OV7670_RRST=0;			//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H; 
		OV7670_RCK_L;
		OV7670_RRST=1;			//复位读指针结束 
		OV7670_RCK_H; 
		
		for(n=0;n<(OvLcdWide*OvLcdHigh);n++)
		{	    
			OV7670_RCK_L;
			TX_buf[PixCnt]=OV7670_DATA;		//读数据,高八位
			OV7670_RCK_H; 
			 
			OV7670_RCK_L;
			TX_buf[PixCnt+1]=OV7670_DATA;	//读数据,低八位	
			OV7670_RCK_H; 
			
			PixCnt+=2;
			if(PixCnt==buf_size)
			{
				while((USART3_flag&0xf0)!=0xf0);//等待应答
				DMA1_Channel2_Enable();			//开启DMA传输
				USART3_flag&=0x0f;				//清除应答
				PixCnt = 0;
			}
		}
					  
		OV7670_RCK_L;
		OV7670_RCK_H; 
		ov_sta=0;				//开始下一次采集
// 		ov_frame++;  				 	 
	} 
}


