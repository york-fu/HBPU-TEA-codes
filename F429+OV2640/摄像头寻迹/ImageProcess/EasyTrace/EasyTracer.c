#include "EasyTracer.h"
#include "lcd.h"
#include "timer.h"


#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))


//读取RBG格式颜色，唯一需要移植的函数
#define GUI_ReadBit16Point   LCD_ReadPoint


void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb)
{
	unsigned short C16;

	C16 = GUI_ReadBit16Point(x,y);

	Rgb->red   =	 (unsigned char)((C16&0xf800)>>8);
	Rgb->green =	 (unsigned char)((C16&0x07e0)>>3);
	Rgb->blue  =     (unsigned char)((C16&0x001f)<<3);
}



void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl)
{
    int h,s,l,maxVal,minVal,difVal;
	int r  = Rgb->red;
	int g  = Rgb->green;
    int b  = Rgb->blue;
	
	maxVal = max3v(r, g, b);
	minVal = min3v(r, g, b);
	
	difVal = maxVal-minVal;
	
	//计算亮度
    l = (maxVal+minVal)*240/255/2;
	
	if(maxVal == minVal)//若r=g=b
	{
		h = 0; 
		s = 0;
	}
	else
	{
		//计算色调
		if(maxVal==r)
		{
			if(g>=b)
				h = 40*(g-b)/(difVal);
			else
				h = 40*(g-b)/(difVal) + 240;
		}
		else if(maxVal==g)
			h = 40*(b-r)/(difVal) + 80;
		else if(maxVal==b)
			h = 40*(r-g)/(difVal) + 160;
		//计算饱和度
		if(l == 0)
			s = 0;
		else if(l<=120)
			s = (difVal)*240/(maxVal+minVal);
		else
			s = (difVal)*240/(480 - (maxVal+minVal));
	}
    Hsl->hue =        (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));
    Hsl->saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));
    Hsl->luminance =  (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));
}

//匹配颜色
static int ColorMatch(const COLOR_HSL *Hsl,const TARGET_CONDI *Condition)
{
	if( 
		Hsl->hue		>	Condition->H_MIN &&
		Hsl->hue		<	Condition->H_MAX &&
		Hsl->saturation	>	Condition->S_MIN &&
		Hsl->saturation	<   Condition->S_MAX &&
		Hsl->luminance	>	Condition->L_MIN &&
		Hsl->luminance	<   Condition->L_MAX 
    )
		return 1;
	else
		return 0;
}

//搜索腐蚀中心
static int SearchCentre(unsigned int *x,unsigned int *y,const TARGET_CONDI *Condition,const SEARCH_AREA *Area)
{
	unsigned int SpaceX,SpaceY,i,j,k,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	SpaceX = Condition->WIDTH_MIN/3;
	SpaceY = Condition->HIGHT_MIN/3;

	for(i=Area->Y_Start;i<Area->Y_End;i+=SpaceY)
	{
		for(j=Area->X_Start;j<Area->X_End;j+=SpaceX)
		{
			FailCount=0;
			for(k=0;k<SpaceX+SpaceY;k++)
			{
				if(k<SpaceX)
					ReadColor(j+k,i+SpaceY/2,&Rgb);				//x坐标递增，y不变
				else
					ReadColor(j+SpaceX/2,i+(k-SpaceX),&Rgb);	//y坐标递增，x不变
				
				RGBtoHSL(&Rgb,&Hsl);
				
				if(!ColorMatch(&Hsl,Condition))		//不匹配
					FailCount++;
				if(FailCount>((SpaceX+SpaceY)>>ALLOW_FAIL_PER))
					break;
			}
			if(k==SpaceX+SpaceY)
			{
				*x = j+SpaceX/2;
				*y = i+SpaceY/2;
				return 1;
			}
		}
	}
	return 0;
}

//从腐蚀中心向外腐蚀，得到新的腐蚀中心
static int Corrode(unsigned int oldx,unsigned int oldy,const TARGET_CONDI *Condition,RESULT *Resu)
{
	unsigned int Xmin,Xmax,Ymin,Ymax,i,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	for(i=oldx;i>IMG_X;i--)			//向左腐蚀
	{
		ReadColor(i,oldy,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Xmin=i;
	FailCount=0;
	
	for(i=oldx;i<IMG_X+IMG_W;i++)	//向右腐蚀
	{
		ReadColor(i,oldy,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Xmax=i;
	FailCount=0;
	
	for(i=oldy;i>IMG_Y;i--)			//向上腐蚀
	{
		ReadColor(oldx,i,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Ymin=i;
	FailCount=0;
	
	for(i=oldy;i<IMG_Y+IMG_H;i++)	//向下腐蚀
	{
		ReadColor(oldx,i,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Ymax=i;
	FailCount=0;
	
	Resu->x	= (Xmin+Xmax)/2;
	Resu->y	= (Ymin+Ymax)/2;
	Resu->w	= Xmax-Xmin;
	Resu->h	= Ymax-Ymin;

	if(((Xmax-Xmin)>(Condition->WIDTH_MIN)) && ((Ymax-Ymin)>(Condition->HIGHT_MIN)) &&\
	   ((Xmax-Xmin)<(Condition->WIDTH_MAX)) && ((Ymax-Ymin)<(Condition->HIGHT_MAX)) )
		return 1;	
	else
		return 0;	
}

//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDI *Condition,RESULT *Resu)
{
	unsigned int i;
	static unsigned int x0,y0,flag=0;
	static SEARCH_AREA Area={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H};
	RESULT Result;	
	

	if(flag==0)
	{
		if(SearchCentre(&x0,&y0,Condition,&Area))	//搜寻到中心
			flag=1;
		else										//重新搜寻
		{
			Area.X_Start= IMG_X	;
			Area.X_End  = IMG_X+IMG_W  ;
			Area.Y_Start= IMG_Y		;
			Area.Y_End  = IMG_Y+IMG_H;

			if(SearchCentre(&x0,&y0,Condition,&Area))	
			{
				flag=0;
				return 0;
			}	
		}
	}
	Result.x = x0;
	Result.y = y0;
	
	for(i=0;i<ITERATE_NUM;i++)
		Corrode(Result.x,Result.y,Condition,&Result);
		
	if(Corrode(Result.x,Result.y,Condition,&Result))
	{
		x0=Result.x;
		y0=Result.y;
		Resu->x=Result.x;
		Resu->y=Result.y;
		Resu->w=Result.w;
		Resu->h=Result.h;
		flag=1;

		Area.X_Start= Result.x - ((Result.w)>>1);
		Area.X_End  = Result.x + ((Result.w)>>1);
		Area.Y_Start= Result.y - ((Result.h)>>1);
		Area.Y_End  = Result.y + ((Result.h)>>1);


		return 1;
	}
	else
	{
		flag=0;
		return 0;
	}

}


/********************************************************************************************************/

/*颜色识别范围*/
#define CWIDTH_MIN 155
#define CWIDTH_MAX 165
#define CHIGHT_MIN 115
#define CHIGHT_MAX 125

#define StepSize 1

/*结果判别阀值*/
#define Threshold (((CWIDTH_MAX-CWIDTH_MIN)/StepSize)*((CHIGHT_MAX-CHIGHT_MIN)/StepSize))*0.6

/************************************************************************
函数功能：	识别颜色
入口参数：	无
返回参数：	无
************************************************************************/
void ColorDiscern(void)
{
	u16 x,y;
	u16 color[7]={0};
	COLOR_RGB RGB;
	COLOR_HSL HSL;
	
	/*绘制识别区域*/
	POINT_COLOR=RED;		//红色
	LCD_DrawRectangle(CWIDTH_MIN,CHIGHT_MIN,CWIDTH_MAX,CHIGHT_MAX);
	POINT_COLOR=WHITE;		//白色
		
	for(y=CHIGHT_MIN;y<CHIGHT_MAX;y+=StepSize)  // 处理区域中数据
	{
		for(x=CWIDTH_MIN;x<CWIDTH_MAX;x+=StepSize)
		{
			ReadColor(x,y,&RGB);	//读取颜色
			RGBtoHSL(&RGB,&HSL);
			
			if(HSL.luminance<=25)		color[0]++;  //黑色
			else if(HSL.luminance>=140)	color[1]++;  //白色
			else
			{
				if(HSL.saturation<=40)	color[2]++;  //灰色
				else 
				{
					if((HSL.hue<=30)||(HSL.hue>200))		color[3]++;  //红色
					else if((HSL.hue>30)&&(HSL.hue<=55))	color[4]++;  //黄色
					else if((HSL.hue>55)&&(HSL.hue<=120))	color[5]++;  //绿色
					else if((HSL.hue>120)&&(HSL.hue<=200))	color[6]++;  //蓝色
				}
			}
		}	
	}
	for(x=0;x<7;x++)
	{
		if(color[x]>Threshold) break;
	}
	
//	switch(x)
//	{
//		case 0:printf("黑色\n");break;
//		case 1:printf("白色\n");break;
//		case 2:printf("灰色\n");break;
//		case 3:printf("红色\n");break;
//		case 4:printf("黄色\n");break;
//		case 5:printf("绿色\n");break;
//		case 6:printf("蓝色\n");break;
//		default:printf("未识别出\n");break;
//	}
		
	LCD_ShowxNum(10,300,x,3,24,0);
}




/************************************************************************
函数功能：	识别点的颜色
入口参数：	x,y:点的坐标
返回参数：	无
************************************************************************/
void PointDiscern(u16 x,u16 y)
{
	COLOR_RGB RGB;
	COLOR_HSL HSL;
		
	ReadColor(x,y,&RGB);	//读取颜色
	RGBtoHSL(&RGB,&HSL);
	
	/*绘制识别区域*/
	POINT_COLOR=RED;		//红色
	LCD_DrawRectangle(x-2,y-2,x+2,y+2);
	POINT_COLOR=WHITE;		//白色
	
	/*打印颜色值*/
	printf("L:%d\tS:%d\tH:%d\r\n",HSL.luminance,HSL.saturation,HSL.hue);
	
//	LCD_ShowxNum(10,300,HSL.luminance,3,24,0);
//	LCD_ShowxNum(10,330,HSL.saturation,3,24,0);
//	LCD_ShowxNum(10,360,HSL.hue,3,24,0);
}




/************************************************************************
函数功能：	寻找中线
入口参数：	无
返回参数：	无
************************************************************************/
void Find_Midline()
{
	u8 flag=0;
	u16 x,y,k;
	u16 FailCount=0;
	COLOR_RGB RGB;
	COLOR_HSL HSL;
	EDGE Edge;		/*黑线信息*/
	
	for(y=90;y<150;y+=5)
	{
		for(x=(IMG_X+15);x<(IMG_W-25);x+=5)
		{				
			if(flag==0)		//检测第一条边缘
			{ 
				FailCount = 0;
				for(k=x;k<(x+10);k+=1)
				{
					ReadColor(k,y,&RGB);	//读取颜色
					RGBtoHSL(&RGB,&HSL);
					
					if(HSL.luminance<25)
					{
						FailCount++;
						if(FailCount>4)
						{
							Edge.Start_x = x+5;
							flag = 1;
							break;
						}
					}
				}
			}
			
			if(flag==1)		//检测第二条边缘
			{
				FailCount = 0;
				for(k=x;k<(x+10);k+=1)
				{
					ReadColor(k,y,&RGB);	//读取颜色
					RGBtoHSL(&RGB,&HSL);
					
					if(HSL.luminance>80)
					{
						FailCount++;
						if(FailCount>4)
						{
							Edge.End_x = x;
							Edge.Center_x = (Edge.Start_x+Edge.End_x)/2;
							if((y==115)||(y==120)||(y==125))
							{
								MidlinePos = Edge.Center_x;
								Center_flag = 1;
							}
							/*显示中线*/
							LCD_DrawLine(Edge.Center_x-1,y,Edge.Center_x+1,y);
							goto GOTO;
						}
					}
				}
			}
		}
		GOTO:
		flag = 0;
	}
}


