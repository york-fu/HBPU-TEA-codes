#ifndef _discern_h_
#define _discern_h_

#include "sys.h"

#define pi 180 

/*REG888结构体*/
typedef struct
{
    unsigned char red;		// [0,255]
    unsigned char green;	// [0,255]
    unsigned char blue;		// [0,255]
}COLOR_RGB;//RGB格式颜色

/*HSL结构体*/
typedef struct
{
    unsigned int hue;			// [0,240]
    unsigned int saturation;	// [0,240]
    unsigned int luminance;	// [0,240]
}COLOR_HSL;//HSL格式颜色

#define min3val(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
#define max3val(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

void RGB565_RGB888(u16 data,COLOR_RGB *Rgb);
COLOR_HSL REGtoHSL(COLOR_RGB reb);
u8 Identify_Color(u16 color);		//判别像素点的颜色

#endif
