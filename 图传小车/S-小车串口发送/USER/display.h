#ifndef _display_h_
#define _display_h_

#include "sys.h"

/*摄像头开窗大小*/
#define OvLcdWide 240
#define OvLcdHigh 320

#define buf_size OvLcdHigh*2*10
/*数据buf*/
extern u8* TX_buf;


/*图像数据处理*/
void Image_DataHandle(void);

#endif
