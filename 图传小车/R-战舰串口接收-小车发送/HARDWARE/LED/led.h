#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

#define Bluetooth_P PBin(0)	//蓝牙端口


/*LED端口初始化*/
void LED_Init(void);//初始化

/*蓝牙端口初始化*/
void BluetoothIO_init(void);
		 				    
#endif
