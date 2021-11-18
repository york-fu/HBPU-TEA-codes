#ifndef EASY_TRACER_H
#define EASY_TRACER_H

#include "sys.h"
#include "ov2640.h"

#define IMG_X 0	  //图片x坐标
#define IMG_Y 0	  //图片y坐标
#define IMG_W ImageSize_X //图片宽度
#define IMG_H ImageSize_Y //图片高度

#define ALLOW_FAIL_PER 3 //容错率，没1<<ALLOW_FAIL_PER个点允许出现一个错误点，容错率越大越容易识别，但错误率越大
#define ITERATE_NUM    9 //迭代次数，迭代次数越多识别越精确，但计算量越大

typedef struct{
    unsigned char  H_MIN;//目标最小色调
    unsigned char  H_MAX;//目标最大色调	
    
	unsigned char  S_MIN;//目标最小饱和度  
    unsigned char  S_MAX;//目标最大饱和度
	
	unsigned char  L_MIN;//目标最小亮度  
    unsigned char  L_MAX;//目标最大亮度
	
	unsigned int  WIDTH_MIN;//目标最小宽度
	unsigned int  HIGHT_MIN;//目标最小高度

	unsigned int  WIDTH_MAX;//目标最大宽度
	unsigned int  HIGHT_MAX;//目标最大高度

}TARGET_CONDI;//判定为的目标条件

typedef struct{
	unsigned int x;//目标的x坐标
	unsigned int y;//目标的y坐标
	unsigned int w;//目标的宽度
	unsigned int h;//目标的高度
}RESULT;//识别结果


typedef struct{
    unsigned char  red;             // [0,255]
    unsigned char  green;           // [0,255]
    unsigned char  blue;            // [0,255]
}COLOR_RGB;//RGB格式颜色

typedef struct{
    unsigned char hue;              // [0,240]
    unsigned char saturation;       // [0,240]
    unsigned char luminance;        // [0,240]
}COLOR_HSL;//HSL格式颜色

typedef struct{
    unsigned int X_Start;              
    unsigned int X_End;
	unsigned int Y_Start;              
    unsigned int Y_End;
}SEARCH_AREA;//区域

/*黑线边缘信息*/
typedef struct
{
    unsigned int Start_x;              
    unsigned int End_x; 
	unsigned int Center_x; 
}EDGE;


//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDI *Condition,RESULT *Resu);

void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb);
void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl);

/************************************************************************
函数功能：	识别颜色
入口参数：	无
返回参数：	无
************************************************************************/
void ColorDiscern(void);

/************************************************************************
函数功能：	识别点的颜色
入口参数：	x,y:点的坐标
返回参数：	无
************************************************************************/
void PointDiscern(u16 x,u16 y);

/************************************************************************
函数功能：	寻找中线
入口参数：	无
返回参数：	无
************************************************************************/
void Find_Midline(void);

#endif
