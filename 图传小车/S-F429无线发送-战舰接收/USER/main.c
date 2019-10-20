#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "EasyTracer.h"
#include "key.h"
#include "24l01.h"


//	y x
u16 img_buf[240][320];

//无线发送BUF
u8 Txbuf[32]={"0123456789"};
u8 Txbufxx[32]={"9876543210"};
u8 txdata=0x66;

// .
RESULT Resu;           
TARGET_CONDI Condition={120,180,50,240,20,190,50,50,100,100};


int main(void)
{
	u8 key,n=0;
	u16 x, y;		
	
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
	KEY_Init();						//初始化KEY
    LCD_Init();	                    //初始化LCD 
	
	NRF24L01_Init();	//无线初始化
	while(NRF24L01_Check())
	{
		LCD_ShowString(60,30,lcddev.width,16,16,(u8*)"NRF24L01 Error!");
		delay_ms(300);				  
		LCD_Fill(60,30,lcddev.width,50,BLACK);//清除显示
		delay_ms(200);
		LED1=!LED1;
	}
	NRF24L01_TX_Mode();	//设置无线为发送模式
	
	while(OV2640_Init())//初始化OV2640
	{
		LCD_ShowString(60,50,lcddev.width,16,16,(u8*)"OV2640 Error!");
		delay_ms(300);				  
		LCD_Fill(60,50,lcddev.width,70,BLACK);//清除显示
		delay_ms(200);
		LED1=!LED1;
	}
	
	OV2640_RGB565_Mode();	//RGB565模式 
	OV2640_Light_Mode(0);	//自动模式
	OV2640_Color_Saturation(2);//色彩饱和度2
	OV2640_Brightness(2);	//亮度0
	OV2640_Contrast(2);		//对比度0
	
	OV2640_OutSize_Set(320,240);
	
	DCMI_Init();			//DCMI配置
	
	DCMI_DMA_Init((u32)img_buf,0,sizeof(img_buf)/4,DMA_MDATAALIGN_HALFWORD,DMA_MINC_ENABLE);//DCMI DMA配置
					
	DCMI_Start(); 			//启动传输 
	
    while(1)
    {
//		key = KEY_Scan(0);
//		if(key==KEY0_PRES)
//		{
//			txdata = 0x66;
//			NRF24L01_TxPacket(Txbuf);
//			LED1 = !LED1;
//		}
//		else if(key==KEY1_PRES)
//		{
//			txdata = 0x33;
//			NRF24L01_TxPacket(Txbufxx);
//			LED1 = !LED1;
//		}
		
		LCD_SetCursor(0, 0);
		LCD_WriteRAM_Prepare();		//开始写入GRAM
		
		for (y = 0; y < 240; y++)
		{
			for (x = 0; x < 320; x++)
			{
				// 显示原图
				LCD->LCD_RAM = img_buf[y][x];
									
				Txbuf[n] = (img_buf[y][x]>>8);
				Txbuf[n+1] = (img_buf[y][x]);
				n+=2;
				
				if(x==319)
				{				
					LCD_SetCursor(0, y+1);
					LCD_WriteRAM_Prepare();		//开始写入GRAM
				}
				
				if(n==32)
				{
					NRF24L01_TxPacket(Txbuf);
					n = 0;
					delay_us(780);
				}
			}
		}
		
		
		// 搜索小车
//		if(Trace(&Condition,&Resu))
//		{
//			LCD_DrawRectangle(Resu.x-Resu.w/2+2,Resu.y-Resu.h/2+2,Resu.x+Resu.w/2-2,Resu.y+Resu.h/2-2);
//			LCD_DrawRectangle(Resu.x-2,Resu.y-2,Resu.x+2,Resu.y+2);
//		}
//		
//		// 显示坐标数据
//		LCD_ShowxNum(280, 190, Resu.x, 3, 16, 0);
//		LCD_ShowxNum(280, 210, Resu.y, 3, 16, 0);
//		
//		// 发送数据
//		uart1_send_data(Resu.x, Resu.y, Resu.w, Resu.h);
	}
}
