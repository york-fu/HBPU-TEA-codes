#include "handle.h"


/******************************************************************
*函数名称:	Sinistrogyration
*函数功能:	舵机左旋
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Handle_delay(u16 t)
{
	while(t--)
	{
		Camera_refresh();	//更新LCD显示
		delay_us(500);
	}
}


/******************************************************************
*函数名称:	Sinistrogyration
*函数功能:	舵机左旋
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Sinistrogyration(void)
{	
	TIM_SetCompare3(TIM8,1300);
	Handle_delay(500);
}


/******************************************************************
*函数名称:	Dextroversion
*函数功能:	舵机右旋
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Dextroversion(void)
{	
	TIM_SetCompare3(TIM8,550);
	Handle_delay(500);
}


/******************************************************************
*函数名称:	Steering_Reset
*函数功能:	舵机复位
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Steering_Reset(void)
{	
	TIM_SetCompare3(TIM8,1000);
}


/******************************************************************
*函数名称:	Conveyor_Advance
*函数功能:	传送带向前
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Conveyor_Advance(void)
{
	TIM_SetCompare1(TIM3,5400);
	TIM_SetCompare2(TIM3,0);
	
	Conveyor_Run = 1;	//标记传送带运行
}


/******************************************************************
*函数名称:	Conveyor_Recede
*函数功能:	传送带向后
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Conveyor_Recede(void)
{
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,5400);
	
	Conveyor_Run = 1;	//标记传送带运行
}


/******************************************************************
*函数名称:	Conveyor_Stop
*函数功能:	停止传送带
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Conveyor_Stop(void)
{
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	
	Conveyor_Run = 0;
}


/******************************************************************
*函数名称:	Push_1
*函数功能:	物品推向右边，1区
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Push_1(void)
{
	u16 Distance=0;		//传送带距离
	
	/*物品推向右边，1区*/
	Sinistrogyration();		//舵机左旋
	Distance = TIM4->CNT;	//记录传送带位移
	Conveyor_Advance();		//启动传送带
	while((TIM4->CNT < (Distance+Variate))&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//更新LCD显示
	}
	Conveyor_Stop();		//停止传送带
	Dextroversion();		//舵机右旋
	Steering_Reset();		//舵机复位
	Conveyor_Recede();		//传送带后退
	while((TIM4->CNT > Distance)&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//更新LCD显示
	}
	Conveyor_Stop();		//停止传送带
	Handle_delay(200);	
}


/******************************************************************
*函数名称:	Push_2
*函数功能:	物品推向前面，2区
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Push_2(void)
{
	u16 Distance=0;		//传送带距离
	
	/*物品推向前面，2区*/
	Sinistrogyration();		//舵机左旋
	Distance = TIM4->CNT;	//记录传送带位移
	Conveyor_Advance();		//启动传送带
	while((TIM4->CNT < (Distance+Variate+135))&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//更新LCD显示
	}
	Conveyor_Stop();		//停止传送带
	Handle_delay(300);
	Conveyor_Recede();		//传送带后退
	while((TIM4->CNT > (Distance))&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//更新LCD显示
	}
	Conveyor_Stop();		//停止传送带
	Steering_Reset();		//舵机复位	
	Handle_delay(200);	
}


/******************************************************************
*函数名称:	Push_3
*函数功能:	物品推向左边，3区
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Push_3(void)
{
	u16 Distance=0;		//传送带距离
	
	/*物品推向左边，3区*/
	Dextroversion();		//舵机右旋	
	Distance = TIM4->CNT;	//记录传送带位移
	Conveyor_Advance();		//启动传送带
	while((TIM4->CNT < (Distance+Variate))&&(TIM4->CNT<3000))
	{			
		Camera_refresh();	//更新LCD显示
	}
	Conveyor_Stop();		//停止传送带		
	Sinistrogyration();		//舵机左旋
	Steering_Reset();		//舵机复位
	Conveyor_Recede();		//传送带后退
	while((TIM4->CNT > Distance)&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//更新LCD显示
	}
	Conveyor_Stop();		//停止传送带
	Handle_delay(200);	
}


/******************************************************************
*函数名称:	Result_Handler
*函数功能:	结果处理函数,根据判定的颜色处理相关信息
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Result_Handler(u8 dat)
{	
	/*输出颜色结果*/
	switch(dat)
	{
		case 1:printf("白色\r\n");
			break;
		
		case 2:printf("黑色\r\n");
			break;
		
		case 3:printf("灰色\r\n");
			break;
		
		case 4:printf("红色\r\n" );
			break;
		
		case 5:printf("黄色\r\n");
			break;
		
		case 6:printf("绿色\r\n");
			break;
		
		case 7:printf("蓝色\r\n");
			break;
		
		default:printf("其他\r\n");	
	}
	printf("\r\n");
	
	if(dat==5)		//黄色
	{
		Push_1();
	}
	else if(dat==2)	//黑色
	{	
		Push_2();
	}
	else
	{	
		Push_2();
	}
}

