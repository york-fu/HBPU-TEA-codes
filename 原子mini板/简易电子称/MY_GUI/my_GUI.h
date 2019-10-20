#ifndef __MY_GUI_H
#define __MY_GUI_H

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h" 
#include "lcd.h" 
#include "touch.h"
#include "usart.h"



void Basic_Interface(void);  //基本界面
void LCD_Keyboard(void);     //键盘
void LCD_Touch(void);        //触摸屏
void LCD_Trends(void);       //按键动态显示
void LCD_Input(void);        //触摸按键输入
void LCD_Count(s16 date);  //输入电压值计算质量



#endif


