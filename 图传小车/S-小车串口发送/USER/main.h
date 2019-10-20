#ifndef MAIN_H_
#define MAIN_H_

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h" 
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "control.h"
#include "led.h"
#include "motor.h"
#include "key.h"
#include "24l01.h"
#include "ov7670.h"
#include "lcd.h"
#include "exti.h"
#include "timer.h"
#include "dma.h"
#include "malloc.h"
#include "display.h"


typedef enum
{
	LED_0,
	LED_1,
}LEDx;


#endif
