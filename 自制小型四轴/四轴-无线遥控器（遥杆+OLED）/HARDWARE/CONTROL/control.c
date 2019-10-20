#include "control.h"
#include "main.h"

/*
 - 摇杆采集值
 - ADCX[0],ADCX[1] 左摇杆 X Y
 - ADCX[2],ADCX[3] 右摇杆 X Y 
*/
float ADCX[4]; 

/*
 - 油门值
 - 对应四轴THROTTLR加减量
 - 0-4  为减 -(5-1)
 - 6-10 为加 +(1-5)
*/
u16 THROTTLR;

u8 Before_after;  	// 前后标志  1-前进  2-后退
u8 Right_left;  	// 左右标志  1-左  2-右
u8 Rotation;  		// 旋转标志  1-左  2-右

/*
 - 锁定标志位
 - 0 - 锁定
 - 1 - 解锁
*/
u8  ARMED=0;


/***********************************
* NRFDate[0]：
*			 1--前
*			 2--后
* NRFDate[1]：
*			 1--左
*			 2--右
* NRFDate[2]：
*			 1--加速
*			 2--减速
* NRFDate[3]：
*			 9--锁定
*			 6--解锁
* NRFDate[3]：
*			 1--通信中
*			 其他--通信断开
************************************/
u8 NRFSendbuf[5] = {0,0,0,0,1};	// 发送数据缓存

void ADC_Gather(void)
{
	u8 i;
	
	for(i=0;i<4;i++)
	{
		ADCX[i] = Get_Adc(i);
		ADCX[i] /= 400.0;
	}
	
	THROTTLR = (u8)ADCX[1]; 		// 油门
	Rotation = (u8)ADCX[0];  		// 旋转
	Before_after = (u8)ADCX[3]; 	 //前后
	Right_left = (u8)(ADCX[2]+0.4);  //左右
}

void KEY_Control(void)
{
	u8 key=KEY_Scan(0);
	
	if(key==KEY0_PRES)ARMED=1;  // 解锁
	else if(key==KEY1_PRES)ARMED=0;  // 锁定
}

void NRF_SendData(void)
{
	if(Before_after>5)NRFSendbuf[0]=1;  // 前后
	else if(Before_after<5)NRFSendbuf[0]=2; 
	else NRFSendbuf[0]=0;
	
	if(Right_left<5)NRFSendbuf[1]=1;  // 左右
	else if(Right_left>5)NRFSendbuf[1]=2; 
	else NRFSendbuf[1]=0;
	
	if(THROTTLR>5)NRFSendbuf[2]=1;  // 油门
	else if(THROTTLR<5)NRFSendbuf[2]=2; 
	else NRFSendbuf[2]=0;
	
	if(ARMED)NRFSendbuf[3]=6;	// 解锁
	else NRFSendbuf[3]=9;		// 锁定
	
	NRF24L01_TxPacket(NRFSendbuf);	// 发送数据		
}

int myabs(int a)  // 绝对值函数
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a;
	return temp;
}



