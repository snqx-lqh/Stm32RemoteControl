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

void oled_iic_sda_high(void)
{
	OLED_SDA_GPIO->BSRR = OLED_SDA_GPIO_PIN;
}

void oled_iic_sda_low(void)
{
	OLED_SDA_GPIO->BRR = OLED_SDA_GPIO_PIN;
}

void oled_iic_scl_high(void)
{
	OLED_SCL_GPIO->BSRR = OLED_SCL_GPIO_PIN;
}

void oled_iic_scl_low(void)
{
	OLED_SCL_GPIO->BRR = OLED_SCL_GPIO_PIN;
}

uint8_t oled_iic_sda_read(void)
{
	return GPIO_ReadInputDataBit(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN);
}

static soft_i2c_t oled_soft_iic={
	.soft_i2c_init = oled_soft_i2c_init,
	.soft_iic_sda_out = oled_soft_iic_sda_out, 
	.soft_iic_sda_in = oled_soft_iic_sda_in,
	.iic_sda_high = oled_iic_sda_high,
	.iic_sda_low = oled_iic_sda_low,
	.iic_scl_high = oled_iic_scl_high,
	.iic_scl_low = oled_iic_scl_low,
	.iic_sda_read = oled_iic_sda_read
};

void OLED_WR_Byte(uint8_t dat,uint8_t mode)
{	
	soft_iic_start(&oled_soft_iic);
    soft_iic_send_byte(&oled_soft_iic,0x78);
    soft_iic_wait_ack(&oled_soft_iic);
    if(mode) {
        soft_iic_send_byte(&oled_soft_iic,0x40);
    }
    else {
        soft_iic_send_byte(&oled_soft_iic,0x00);
    }
    soft_iic_wait_ack(&oled_soft_iic);
    soft_iic_send_byte(&oled_soft_iic,dat);
    soft_iic_wait_ack(&oled_soft_iic);
    soft_iic_stop(&oled_soft_iic);
}

void OLED_WR_Data_Bytes(u8 *dat)
{
	u8 n;
    soft_iic_start(&oled_soft_iic);
    soft_iic_send_byte(&oled_soft_iic,0x78);
    soft_iic_wait_ack(&oled_soft_iic);
	soft_iic_send_byte(&oled_soft_iic,0x40);
	soft_iic_wait_ack(&oled_soft_iic);
	for(n=0; n<128; n++)
	{
		soft_iic_send_byte(&oled_soft_iic,dat[n]);
		soft_iic_wait_ack(&oled_soft_iic);
	}
	soft_iic_stop(&oled_soft_iic);
}

int oled_write_byte(unsigned char data,unsigned char cmd)
{
	OLED_WR_Byte(data,cmd);
	return 0;
}

int oled_write_bytes(unsigned char *data)
{
	OLED_WR_Data_Bytes(data);
	return 0;
}

int oled_rst()
{
	return 0;
}

struct ssd1306_oled_operations ssd1306_oled_oper = {
	.write_byte  =     oled_write_byte,
	.iic_or_spi  =     IIC_MODE,                  
	.write_bytes =     oled_write_bytes,
	.oled_rst    =     oled_rst
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
void oled_soft_spi_sck_high(void)
{
	OLED_SCL_Set();
}
void oled_soft_spi_sck_low(void)
{
	OLED_SCL_Clr();
}
void oled_soft_spi_mosi_high(void)
{
	OLED_SDA_Set();
}
void oled_soft_spi_mosi_low(void)
{
	OLED_SDA_Clr();
}
uint8_t oled_soft_spi_miso_read(void)
{
	
}

struct soft_spi_operations oled_soft_spi = {
	.spi_init      = oled_soft_spi_init,
	.spi_sck_high  = oled_soft_spi_sck_high,
	.spi_sck_low   = oled_soft_spi_sck_low,
	.spi_mosi_high = oled_soft_spi_mosi_high,
	.spi_mosi_low  = oled_soft_spi_mosi_low,
	.spi_miso_read = oled_soft_spi_miso_read	 
};

int oled_write_byte(unsigned char data,unsigned char cmd)
{
	if(cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    SPI_ReadWriteByte_Mode3(&oled_soft_spi,data);
    OLED_CS_Set();
    OLED_DC_Set();
	return 0;
}

int oled_write_bytes(unsigned char *data)
{
	return 0;
}

int oled_rst()
{
	return 0;
}

struct ssd1306_oled_operations ssd1306_oled_oper = {
	.write_byte  =     oled_write_byte,
	.iic_or_spi  =     SPI_MODE,                  
	.oled_rst    =     oled_rst
};

#endif

void oled1306_middle_init()
{
	#ifdef IIC_USE
	//初始化IIC总线
	soft_i2c_init(&oled_soft_iic); 
	#else
	SOFT_SPI_Init(&oled_soft_spi);
	#endif
	//注册6050操作函数
	ssd1306_oled_operation_register(&ssd1306_oled_oper);
	OLED_Init();
}

void oled1306_example()
{
	while(1)
	{
		OLED_ShowString(0,0,"HELLO",16,1);
		OLED_Refresh();
	}
	
}
