#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"

/*摄像头开窗大小*/
#define OvLcdWide 240
#define OvLcdHigh 320

/*数据接收BUF大小*/
#define RxSize OvLcdHigh*2*10

/*接收标记*/
extern u8 RX_sta;

/*数据接收BUF*/
extern u8* RX_buf;


void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//配置DMA1_CHx

//开启DMA传输
void MYDMA_Enable(void);//使能DMA

//关闭DMA传输
void MYDMA_Disable(void);

/*DMA通道7*/
void DMAChannel7_Config(u32 cpar,u32 cmar,u16 cndtr);
//开启DMA传输
void DMAChannel7_Enable(void);
//关闭DMA传输
void DMAChannel7_Disable(void);


//SRAM --> LCD_RAM dma配置
//caddr：数据源地址
//16位,从SRAM传输到LCD_RAM. 
void MYDMA_SRAMLCD_Init(u32 caddr);
//开启一次SRAM到LCD的DMA的传输
void MYDMA_SRAMLCD_Enable(void);
		   
#endif




