#include "discern.h"
#include "math.h"
#include "usart.h"


/*RGB565转换成RGB888*/
void RGB565_RGB888(u16 data,COLOR_RGB *Rgb)
{
	Rgb->red   = (unsigned char)((data&0xf800)>>8);
	Rgb->green = (unsigned char)((data&0x07e0)>>3);
	Rgb->blue  = (unsigned char)((data&0x001f));
}


/*RGB转换为HSL*/
COLOR_HSL REGtoHSL(COLOR_RGB rgb)
{
    int h,s,l;					//HSL颜色分量
	int maxVal,minVal,difVal;	//最大值，最小值，差值
	int r = rgb.red;
	int g = rgb.green;
    int b = rgb.blue;
	COLOR_HSL hsl;				//hsl颜色结构体
	
	maxVal = max3val(r, g, b);
	minVal = min3val(r, g, b);
	
	difVal = maxVal-minVal;
	
	/*计算亮度*/
    l = (maxVal+minVal)*240/255/2;
	
	if(maxVal == minVal)	//若r=g=b
	{
		h = 0; 
		s = 0;
	}
	else
	{
		/*计算色调*/
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
		
		/*计算饱和度*/
		if(l == 0)
			s = 0;
		else if(l<=120)
			s = (difVal)*240/(maxVal+minVal);
		else
			s = (difVal)*240/(480 - (maxVal+minVal));
	}
    hsl.hue 	   = (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));
    hsl.saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));
    hsl.luminance  = (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));
	
	return hsl;
}




/******************************************************************
*函数名称:	Identify_Color
*函数功能:	判别像素点的颜色
*函数参数:	RGB565像素点数据
*返 回 值:	无
*******************************************************************/
u8 Identify_Color(u16 color)
{
	COLOR_RGB Rgb;	//RGB颜色
	COLOR_HSL Hsl;	//HLS颜色
	u8 color_flag=0;
	
	/*RGB565转换成RGB888*/
	RGB565_RGB888(color,&Rgb);
	
	/*RGB转换为HSL*/
	Hsl = REGtoHSL(Rgb);
	
	printf("H: %d\tS: %d\tL: %d\r\n",Hsl.hue,Hsl.saturation,Hsl.luminance);
	
	/*根据HSL判定颜色*/
	if(Hsl.luminance>45 && Hsl.luminance<120)	//不是黑色和白色
	{
		if(Hsl.saturation>20)				//不是灰色
		{
			if(Hsl.hue<35 || Hsl.hue>220)	//判定为红色
			{
				color_flag=4;
			}
			
			if(Hsl.hue>=35 && Hsl.hue<50)	//判定为黄色
			{
				color_flag=5;
			}
			
			if(Hsl.hue>=50 && Hsl.hue<90)	//判定为绿色
			{
				color_flag=6;
			}
			
			if(Hsl.hue>=130 && Hsl.hue<180)	//判定为蓝色
			{
				color_flag=7;
			}
		}
		
		else
		{
			color_flag=3;			//判定为灰色
		}
	}
	
	else
	{
		if(Hsl.luminance>=120)		//判定为白色
		{
			color_flag=1;
		}
		else
		{
			color_flag=2;			//判定为黑色
		}
	}
	
	return color_flag;
}
