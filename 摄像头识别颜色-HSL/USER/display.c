#include "display.h"
#include "ov7670.h"
#include "lcd.h"
#include "timer.h"
#include "exti.h"
#include "usart.h"
#include "key.h"
#include "tpad.h"
#include "discern.h"

u8 Discern_Flag=0;	//颜色识别标志
u8 over=0;			//判定结束标示
u8 COLOR=0;			//判定结果

/******************************************************************
*函数名称:	Camera_refresh
*函数功能:	更新摄像头显示
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Camera_refresh(void)
{
	u32 i;
 	u16 color;								//摄像头数据
	u16 Color_1[Sampling_Number];			//存放每个像素数据
	static u16 Color_2[Sampling_Number];	//存放每帧数据
	static u16 Result[Verdict_Nunber];		//存放判断结果
	static u8 Frame_Number=0;				//记录采集帧数
	static u8 Discern_Number=0;				//识别次数
	
	if(ov_sta)	//有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		
		for(i=0;i<76800;i++)
		{
			/*读取摄像头的一个像素点*/
			READ_FIFO_PIXEL(RGB565);
			LCD->LCD_RAM=color; 	//写入数据

			if(Discern_Flag)		//颜色识别标志
			{
				/*采集特定像素点颜色*/
				switch(i)
				{
					case 35350:Color_1[0]=color;break;	//坐标(110,150)
					case 35360:Color_1[1]=color;break;	//坐标(110,160)
					case 35370:Color_1[2]=color;break;	//坐标(110,170)
					
					case 36950:Color_1[3]=color;break;	//坐标(115,150)
					case 36960:Color_1[4]=color;break;	//坐标(115,160)
					case 36970:Color_1[5]=color;break;	//坐标(115,170)
					
					case 38550:Color_1[6]=color;break;	//坐标(120,150)
					case 38560:Color_1[7]=color;break;	//坐标(120,160)
					case 38570:Color_1[8]=color;break;	//坐标(120,170)
					
					case 40150:Color_1[9]=color;break;	//坐标(125,150)
					case 40160:Color_1[10]=color;break;	//坐标(125,160)
					case 40170:Color_1[11]=color;break;	//坐标(125,170)
					
					case 41750:Color_1[12]=color;break;	//坐标(130,150)
					case 41760:Color_1[13]=color;break;	//坐标(130,160)
					case 41770:Color_1[14]=color;break;	//坐标(130,170)
				}
			}
		}
				
		LCD_DrawRectangle(140,100,180,140);	//显示红框
		
		if(Discern_Flag)	//颜色识别
		{			
			Result[Discern_Number++] = Identify_Color(Average_Filter(Color_1));	//存放每帧数据
			
			if(Discern_Number > Verdict_Nunber-1)	//判断Verdict_Nunber次
			{
				for(i=0;i<Verdict_Nunber;i++) printf("%d\t",Result[i]);
				printf("\r\n");
				
				COLOR = Maximum(Result);
				
				Discern_Number = 0;		//判断次数清0
				Discern_Flag = 0;		//识别标志	
				over = 1;				//判断结束
			}
		}
		
 		ov_sta=0;					//清零帧中断标记
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向
		
		if(Discern_Flag)	//颜色识别
		{
			LCD_ShowString(60,270,120,24,24,(u8*)"Discern...");	//识别提示
		}
	} 
}

/******************************************************************
*函数名称:	Average_Filter
*函数功能:	中值平均滤波
*函数参数:	无
*返 回 值:	无
*******************************************************************/
u32 Average_Filter(u16* array)
{
	u8 i;
	u32 temp=0;
	
	Data_Sort1(array);	//冒泡法排序，从小到大	
	
	for(i=3; i<Sampling_Number-3; i++)
	{	
		temp += array[i];
	}	
		
	return temp/(Sampling_Number-6);
}
/******************************************************************
*函数名称:	Data_Sort
*函数功能:	冒泡法排序，从小到大
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Data_Sort1(u16* array)
{
	u8 m, n;
	u32 temp;
	
	for(m=0; m<Sampling_Number-1; m++)
	{
		for(n=0; n<Sampling_Number-1-m; n++)
		{
			if(array[n] > array[n+1])
			{
				temp = array[n];
				array[n] = array[n+1];
				array[n+1] = temp;
			}
		}
	}
}


/******************************************************************
*函数名称:	Data_Sort2
*函数功能:	排序，从小到大
*函数参数:	无
*返 回 值:	无
*******************************************************************/
u16 Data_Sort2(u16* array,u16 number)
{
	u8 m, n;
	u32 temp;
	
	for(m=0; m<number-1; m++)
	{
		
		for(n=m+1; n<number; n++)
		{
			if(array[m] > array[n])
			{
				temp = array[m];
				array[m] = array[n];
				array[n] = temp;
			}
		}
	}
	
	return array[number];
}


u16 Maximum(u16* array)
{
	u16 m,n;
	u16 x[7];
	
	for(n=0;n<Verdict_Nunber-1;n++)
	{
		if(array[n]==1) x[0]++;
		else if(array[n]==2) x[1]++;
		else if(array[n]==3) x[2]++;
		else if(array[n]==4) x[3]++;
		else if(array[n]==5) x[4]++;
		else if(array[n]==6) x[5]++;
		else if(array[n]==7) x[6]++;
	}
	
	return Data_Sort2(x,7);
}


/******************************************************************
*函数名称:	Set_OV7670
*函数功能:	按键设置OV7670
*函数参数:	无
*返 回 值:	无
*******************************************************************/
void Set_OV7670()
{
 	const u8* LMODE_TBL[6] ={(u8*)"Auto",(u8*)"Sunny",(u8*)"Cloudy",(u8*)"Office",(u8*)"Home"};		//5种光照模式

	u8 key;	 
	static u8 msgbuf[15];				//消息缓存区
	static u8 tm=0;
	
	key = KEY_Scan(0);//不支持连按
	if(key)
	{
		tm=10;
		switch(key)
		{				    
			case KEY0_PRES:	//灯光模式Light Mode
				lightmode++;
				if(lightmode>4)lightmode=0;
				OV7670_Light_Mode(lightmode);
				sprintf((char*)msgbuf,"%s",LMODE_TBL[lightmode]);
				break;
			
			case KEY1_PRES:	//饱和度Saturation
				saturation++;
				if(saturation>4)saturation=0;
				OV7670_Color_Saturation(saturation);
				sprintf((char*)msgbuf,"Saturation:%d",(signed char)saturation-2);
				break;
			
			case KEY2_PRES:	//亮度Brightness				 
				brightness++;
				if(brightness>4)brightness=0;
				OV7670_Brightness(brightness);
				sprintf((char*)msgbuf,"Brightness:%d",(signed char)brightness-2);
				break;
			
			case WKUP_PRES:	//对比度Contrast			    
				contrast++;
				if(contrast>4)contrast=0;
				OV7670_Contrast(contrast);
				sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-2);
				break;
		}
	}	 
	
	if(tm)
	{
		LCD_ShowString((lcddev.width-240)/2+30,(lcddev.height-320)/2+60,200,16,16,msgbuf);
		tm--;
	}
}
