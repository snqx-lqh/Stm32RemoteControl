/**
  ******************************************************************************
  * @file    ssd1306_oled_middle.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-02-29
  * @brief   ssd1306_oled的中间层，初始化操作函数，自行修改，这里只是示例
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */

#include "ssd1306_oled_middle.h"
#include "ssd1306_oled_driver.h"
#include "bsp_soft_i2c.h"
#include "bsp_soft_spi.h"
#include "bsp_delay.h"

#ifdef IIC_USE

void oled_soft_i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //开启端口时钟
    RCC_APB2PeriphClockCmd(OLED_RCC_SCL | OLED_RCC_SDA, ENABLE);

    //配置SCL、SDA引脚
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
    GPIO_Init(OLED_SCL_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OLED_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
    GPIO_Init(OLED_SDA_GPIO, &GPIO_InitStructure);

    GPIO_SetBits(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN);
    GPIO_SetBits(OLED_SCL_GPIO,OLED_SCL_GPIO_PIN); //所有设备空闲，总线拉高电平
}

void oled_soft_iic_sda_out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = OLED_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出
    GPIO_Init(OLED_SDA_GPIO, &GPIO_InitStructure);
}

void oled_soft_iic_sda_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = OLED_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_Init(OLED_SDA_GPIO, &GPIO_InitStructure);
}

void oled_set_iic_sda_level(unsigned char level)
{
	if(0 == level)
		OLED_SDA_GPIO->BRR = OLED_SDA_GPIO_PIN;
	else
		OLED_SDA_GPIO->BSRR = OLED_SDA_GPIO_PIN;
}

void oled_set_iic_scl_level(unsigned char level)
{
	if(0 == level)
		OLED_SCL_GPIO->BRR = OLED_SCL_GPIO_PIN;
	else
		OLED_SCL_GPIO->BSRR = OLED_SCL_GPIO_PIN;
}

uint8_t oled_iic_sda_read(void)
{
	return GPIO_ReadInputDataBit(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN);
}

void oled_iic_delay()
{
	
}

soft_i2c_t oled_soft_iic={
	.soft_i2c_init = oled_soft_i2c_init,
	.soft_iic_sda_out = oled_soft_iic_sda_out, 
	.soft_iic_sda_in = oled_soft_iic_sda_in,
	.set_iic_sda_level = oled_set_iic_sda_level,
	.set_iic_scl_level = oled_set_iic_scl_level,
	.iic_sda_read = oled_iic_sda_read,
	.iic_delay    = oled_iic_delay
};

int oled_write_byte(unsigned char reg,unsigned char data)
{
	soft_iic_write_dev_one_byte(&oled_soft_iic,0x78>>1,reg,data);
	return 0;
}

int oled_write_bytes(unsigned char reg,unsigned char *data,unsigned int len)
{
	soft_iic_write_dev_len_byte(&oled_soft_iic,0x78>>1,reg,len,data);
	return 0;
}

ssd1306_oled_oper_t ssd1306_oled_oper = {
	.iic_write_one_byte  =     oled_write_byte,
	.iic_write_len_byte  =     oled_write_bytes,
	.iic_or_spi          =     IIC_MODE
};

#else

void oled_soft_spi_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(OLED_RCC_SCL|OLED_RCC_SDA|OLED_RCC_RES|OLED_RCC_DC|OLED_RCC_CS, ENABLE);	 //使能A端口时钟

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Pin = OLED_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_SCL_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(OLED_SCL_GPIO,OLED_SCL_GPIO_PIN);

    GPIO_InitStructure.GPIO_Pin = OLED_SDA_GPIO_PIN;
    GPIO_Init(OLED_SDA_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN);

    GPIO_InitStructure.GPIO_Pin = OLED_RES_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_RES_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(OLED_RES_GPIO,OLED_RES_GPIO_PIN);

    GPIO_InitStructure.GPIO_Pin = OLED_DC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_DC_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(OLED_DC_GPIO,OLED_DC_GPIO_PIN);
}

void oled_set_spi_sck_level(unsigned char level)
{
	if(0 == level)
		OLED_SCL_Clr();
	else
		OLED_SCL_Set();
}

void oled_set_spi_mosi_level(unsigned char level)
{
	OLED_SDA_Set();
	if(0 == level)
		OLED_SDA_Clr();
	else
		OLED_SDA_Set();
}

uint8_t oled_soft_spi_miso_read(void)
{
	
}

soft_spi_t oled_soft_spi = {
	.spi_init             = oled_soft_spi_init,
	.set_spi_sck_level    = oled_set_spi_sck_level,
	.set_spi_mosi_level   = oled_set_spi_mosi_level,
	.spi_miso_read        = oled_soft_spi_miso_read,
	.spi_mode             = 3
};

int oled_spi_write_one_byte(unsigned char data)
{
	soft_spi_read_write_byte(&oled_soft_spi,data);
	return 0;
}

int oled_spi_write_len_byte(unsigned char *data,unsigned int len)
{
	int i = 0;
	for(i = 0;i < 128;i++)
		soft_spi_read_write_byte(&oled_soft_spi,data[i]);
	return 0;
}

void oled_set_dc_level(unsigned char level)
{
	if(0 == level)
		OLED_DC_Clr();
	else
		OLED_DC_Set();
}

void oled_set_cs_level(unsigned char level)
{
	if(0 == level)
		OLED_CS_Clr();
	else
		OLED_CS_Set();
}

void oled_set_res_level(unsigned char level)
{
	if(0 == level)
		OLED_RES_Clr();
	else
		OLED_RES_Set();
}

ssd1306_oled_oper_t ssd1306_oled_oper = {
	.spi_write_one_byte  =     oled_spi_write_one_byte,
	.spi_write_len_byte  =     oled_spi_write_len_byte,                  
	.set_dc_level        =     oled_set_dc_level,
	.set_cs_level        =     oled_set_cs_level,
	.set_res_level       =     oled_set_res_level,
	.delay_ms            =     delay_ms,

	.iic_or_spi          =     SPI_MODE
};
#endif

void oled1306_middle_init()
{
	#ifdef IIC_USE
	//初始化IIC总线
	soft_i2c_init(&oled_soft_iic); 
	#else
	soft_spi_init(&oled_soft_spi);
	#endif
	OLED_Init(&ssd1306_oled_oper);
}

void oled1306_example()
{
	OLED_ShowString(&ssd1306_oled_oper,0,0,"HELLO",16,1);
	OLED_Refresh(&ssd1306_oled_oper);
}
