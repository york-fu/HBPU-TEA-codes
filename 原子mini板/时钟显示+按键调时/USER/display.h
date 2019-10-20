#ifndef _display_h_
#define _display_h_

#include "sys.h"
#include "lcd.h"
#include "timer.h"
#include "math.h"

typedef struct
{
	float end_x;
	float end_y;
}End_Coord;

extern TimeStruct Time;

void Display_Time(TimeStruct Time);

void Analog_Timer_Display(void);

End_Coord Timer_Needle(float center_x, float center_y, float length, float time, u8 draw_flog);

void Add_Icon(u16 sx,u16 sy);

void Subtract_Icon(u16 sx,u16 sy);

#endif
