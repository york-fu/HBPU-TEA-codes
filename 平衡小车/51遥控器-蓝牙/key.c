#include "key.h"


unsigned char KEY_Scan()
{
	unsigned char key_flag = 0;

	if(P2!=0xff)		//右边有按键按下
	{
		delay_ms(5);
		if(P2!=0xff)	//右边有按键按下
		{
			switch (P2)
			{
				case 0xfe:
					key_flag = 1;
					break;
				case 0xfd:
					key_flag = 2;
					break;
				case 0xfb:
					key_flag = 3;	//右下
					break;
				case 0xf7:
					key_flag = 4;	//右右
					break;
				case 0xef:
					key_flag = 5;	//右左
					break;
				case 0xdf:
					key_flag = 6;	//右上
					break;
				default:key_flag = 0;	
			}
		}
	}

	if(P3!=0xff)		//左边有按键按下
	{
		delay_ms(5);	//左边有按键按下
		if(P3!=0xff)
		{
			switch (P3)
			{
				case 0x7f:
					key_flag = 7;	//左下
					break;
				case 0xbf:
					key_flag = 8;	//左左
					break;
				case 0xdf:
					key_flag = 9; 	//左右
					break;
				case 0xef:
					key_flag = 10;	//左上
					break;
				default:key_flag = 0;
			}
		}
	}

	return key_flag;
}