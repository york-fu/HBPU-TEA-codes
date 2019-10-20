#ifndef __OLED_H
#define	__OLED_H

#include "sys.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78


//IO方向设置
#define OLED_SDA_IN()  {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=0X00080000;}
#define OLED_SDA_OUT() {GPIOB->CRH&=0XFFF0FFFF;GPIOB->CRH|=0X00030000;}

//IO操作函数	 
#define OLED_IIC_SCL    PBout(13) 		//SCL
#define OLED_IIC_SDA    PBout(12) 		//SDA	 
#define OLED_READ_SDA   PBin(12) 		//输入SDA 


void OLED_IIC_Init(void);
u8 OLED_Write_Byte(u8 reg,u8 data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
u32 OLED_Pow(u8 m,u8 n);

#endif

