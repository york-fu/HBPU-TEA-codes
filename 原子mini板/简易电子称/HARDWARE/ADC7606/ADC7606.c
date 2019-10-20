#include "ADC7606.h"
#include "lcd.h"

int16_t s_adc_now[8]={0};
int16_t s_volt[8]={0};
int16_t s_dat[8]={0};
/*********************************************************************************************************
*	函 数 名: AD7606_ConfigGPIO
*	功能说明: 配置GPIO。 不包括 SCK  MOSI  MISO 共享的SPI总线。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void  AD7606_ConfigGPIO( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		/* PB3默认为JTDO功能。将PB3口复用为普通IO口 */
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag , 开启swd
	
	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_CS | RCC_RANGE | RCC_BUSY | RCC_RESET | RCC_CONVST, ENABLE);	
	
	/* 配置几个推完输出IO */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */

	GPIO_InitStructure.GPIO_Pin = PIN_RESET;
	GPIO_Init(PORT_RESET, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CONVST;
	GPIO_Init(PORT_CONVST, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RANGE;
	GPIO_Init(PORT_RANGE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);

	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO 设置为输入上拉 */

	GPIO_InitStructure.GPIO_Pin = PIN_BUSY;
	GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPIBus
*	功能说明: 配置SPI总线。 只包括 SCK、 MOSI、 MISO口线的配置。不包括片选CS，也不包括外设芯片特有的INT、BUSY等
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);	

	/* 配置SPI引脚SCK、MISO 和 MOSI为复用推挽模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
	GPIO_Init(PORT_MOSI, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO 设置为输入上拉 */
	GPIO_InitStructure.GPIO_Pin = PIN_MISO;
	GPIO_Init(PORT_MISO, &GPIO_InitStructure);


}

/*
*********************************************************************************************************
*	函 数 名: AD7606_SetInputRange
*	功能说明: 配置AD7606模拟信号输入量程。
*	形    参: _ucRange : 0 表示正负5V   1表示正负10V
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		AD_RANGE_5V();	/* 设置为正负5V */
	}
	else
	{
		AD_RANGE_10V();	/* 设置为正负10V */
	}
}


/*
*********************************************************************************************************
*	函 数 名: AD7606_RESET
*	功能说明: 硬件复位
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	/* CS  SCLK拉高 */
#ifdef SOFT_SPI  
		AD_CS_1();
		//SCK_1();
#endif

 	/* AD7606是高电平复位，要求最小脉宽50ns */
	AD_RESET_LOW();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_LOW();

}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitAD7606
*	功能说明: 初始化AD7606 SPI口线
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spi_InitAD7606(void)
{	
	AD7606_SetInputRange(0);	/* 设置采样量程 */

	/* 设置过采样模式 */
	//ad7606_SetOS(0);
		
	AD_CONVST_HIGH();			/* CONVST脚设置为高电平 */
	
	AD7606_Reset();				/* 硬件复位复AD7606 */
}
/*
*********************************************************************************************************
*	函 数 名: bsp_spiRead1
*	功能说明: 从SPI总线接收8个bit数据。  SCK上升沿采集数据, SCK空闲时为高电平
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{
	uint8_t i;
	uint8_t read = 0;
	
	for (i = 0; i < 8; i++)
	{
		SCK_0();//时钟拉低
		bsp_spiDelay();//延时
		read = read << 1;
		if (MISO_IS_HIGH())//如果MISO=1
		{
			read|=0x01;
		}
		SCK_1();//时钟拉高
		bsp_spiDelay();//延时
	}
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_scan
*	功能说明: 扫描调用本函数，用于读取AD转换器数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Scan(void) 		/* 此函数代码按照时序编写 */
{
	uint8_t i;			
	
	AD7606_StartConv();	/* 给开始信号 */
	
	/* BUSY = 0 时.ad7606处于空闲状态ad转换结束 */	
	while (!BUSY_IS_LOW());	   
	
	AD_CS_0(); /* SPI片选 = 0 */

	for (i = 0; i < CH_NUM; i++)
	{
		s_adc_now[i] = bsp_spiRead1();
		
		s_adc_now[i] = s_adc_now[i] * 256 + bsp_spiRead1(); /* 读数据 */	
	}

	AD_CS_1(); /* SPI片选 = 1 */
}	  


/*
*********************************************************************************************************
*	函 数 名: ad7606_StartConv
*	功能说明: 启动AD7606的ADC转换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_StartConv(void)
{
	/* 上升沿开始转换，低电平持续时间至少25ns  */
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();	/* 连续执行2次，低电平约50ns */
	
	AD_CONVST_HIGH();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiDelay
*	功能说明: 时序延迟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiDelay(void)
{
	uint32_t i;

	for (i = 0; i < 2; i++);
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_ReadAdc
*	功能说明: 数组中读取一路ADC值
*	形    参：_ch : 读取转换ADC一路结果
*	返 回 值: sAdc：返回读取的结果
*********************************************************************************************************
*/
int16_t AD7606_ReadAdc(uint8_t _ch)
{
	int16_t sAdc;
	
	//DISABLE_INT();	
	sAdc = s_adc_now[_ch];
	//ENABLE_INT();

	return sAdc;
}

//多阶滞后滤波
//对周期性干扰具有良好的抑制
//适于波动频率较高场合
#define fit_a 250  
u16 OneFactorialFiler_1( u16 now_value )
{  
	static  u16 last_value = 0;
	u16 fit_NewValue = 0;
	u32 ReturnValue = 0;
	fit_NewValue = now_value;
	ReturnValue = (255-fit_a)*fit_NewValue + fit_a*last_value;
	ReturnValue /= 255;
	last_value = fit_NewValue;
	return( (u16)ReturnValue );
}
u16 OneFactorialFiler_2( u16 now_value )
{  
	static  u16 last_value = 0;
	u16 fit_NewValue = 0;
	u32 ReturnValue = 0;
	fit_NewValue = now_value;
	ReturnValue = (255-fit_a)*fit_NewValue + fit_a*last_value;
	ReturnValue /= 255;	
	last_value = 	fit_NewValue;
	return( (u16)ReturnValue );
}

/*
*********************************************************************************************************
*	函 数 名: AD7606_Mak
*	功能说明: 处理采样后的数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Count(void)
{
	uint8_t i;
	int16_t adc;

	for (i = 0;i < CH_NUM; i++)
	{	
		s_dat[i] = AD7606_ReadAdc(i);
	/* 
		32767 = 5V , 这是理论值，实际可以根据5V基准的实际值进行公式矫正 
		volt[i] = ((int16_t)dat[i] * 5000) / 32767;	计算实际电压值（近似估算的），如需准确，请进行校准            
		volt[i] = dat[i] * 0.3051850947599719
	*/
		
		adc = s_dat[i];
//		s_volt[i] = ((adc*10000) / 33960)*5;
		s_volt[i] = ((adc*1000) / 32767)*5;
		
		s_volt[i] = OneFactorialFiler_1(s_volt[i]);
		s_volt[i] = OneFactorialFiler_2(s_volt[i]);
	}
	
}

void AD7606_Init(void)  //  ad7606模块初始化
{
	bsp_InitSPIBus();
	AD7606_ConfigGPIO();	  /* 配置GPIO */
	bsp_spi_InitAD7606();	  /* 配置AD7606所用的GPIO */
}

