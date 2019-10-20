#include "spi.h"
#include "delay.h"


/*******************************
*连线说明：
*1--SDA <-----> 
*2--SCK <----->PB6 
*3--MOSI<----->PB3 
*4--MISO<----->PB5 
************************************/
void SIMUL_SPI_Init(void)  //模拟SPI IO口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//时钟使能

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_6;		//SCK,MOSI引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			//速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;        			//MISO输入管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      		//速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//普通输入              
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}   


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 Byte)
{		
	u8 bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)
	{
		if(Byte &0x80)                   	//for循环中对数据进行左移，并每次读取最高位.即依次读取数据
			MOSI_H;							//若该位为1则置1
		else
			MOSI_L;                     	//若该位为0则置0
		SCK_H;								//SCK置高发送数据
		Byte <<= 1;                    		//发送完后左移，准备下一位数据读取，并空出一位准备读取数据
		if(MISO_Read)						//读取MISO的电平（即数据）
			Byte |= 0x01;
		SCK_L;                          	//置低，结束本次数据读取 
	}
	return Byte;				    
}



