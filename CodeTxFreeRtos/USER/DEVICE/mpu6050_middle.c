/**
  ******************************************************************************
  * @file    mpu6050_middle.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-02-29
  * @brief   mpu6050的中间层，初始化操作函数
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */

#include "mpu6050_middle.h"
#include "mpu6050_driver.h"
#include "bsp_soft_i2c.h"
#include "bsp_delay.h"

void mpu_soft_i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //开启端口时钟
    RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC | IIC_SDA_PORT_RCC, ENABLE);

    //配置SCL、SDA引脚
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出
    GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);

    GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);
    GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN); //所有设备空闲，总线拉高电平
}

void mpu_soft_iic_sda_out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

void mpu_soft_iic_sda_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

void mpu_iic_sda_high(void)
{
	GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);
}

void mpu_iic_sda_low(void)
{
	GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN);
}

void mpu_iic_scl_high(void)
{
	GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN);
}

void mpu_iic_scl_low(void)
{
	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);
}

uint8_t mpu_iic_sda_read(void)
{
	return GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN);
}

//初始化MPU6050软件IIC操作函数
static soft_i2c_t mpu_soft_iic={
	.soft_i2c_init = mpu_soft_i2c_init,
	.soft_iic_sda_out = mpu_soft_iic_sda_out, 
	.soft_iic_sda_in = mpu_soft_iic_sda_in,
	.iic_sda_high = mpu_iic_sda_high,
	.iic_sda_low = mpu_iic_sda_low,
	.iic_scl_high = mpu_iic_scl_high,
	.iic_scl_low = mpu_iic_scl_low,
	.iic_sda_read = mpu_iic_sda_read
};

int mpu6050_write_one_byte(unsigned char reg,unsigned char data)
{
	soft_iic_write_dev_one_byte(&mpu_soft_iic,MPU6050_ADDR,reg,data);
	return 0;
}
int mpu6050_read_one_byte (unsigned char reg,unsigned char *data)
{
	*data = soft_iic_read_dev_one_byte(&mpu_soft_iic,MPU6050_ADDR,reg);
	return 0;
}
int mpu6050_read_bytes    (unsigned char reg,unsigned char *data,unsigned char len)
{
	soft_iic_read_dev_len_byte(&mpu_soft_iic,MPU6050_ADDR,reg,len,data);
	return 0;
}

//初始化MPU6050操作函数
struct mpu6050_operations mpu6050_oper = {
	.write_one_byte = mpu6050_write_one_byte,
	.read_one_byte  = mpu6050_read_one_byte,
	.read_bytes     = mpu6050_read_bytes,
	.delay_ms       = delay_ms
};

void mpu6050_middle_init()
{
	//初始化IIC总线
	soft_i2c_init(&mpu_soft_iic);     
	//注册6050操作函数
	mpu6050_operation_register(&mpu6050_oper);
	mpu6050_init();
}

short gyro[3];

void mpu6050_example()
{
	mpu6050_middle_init();
	
	while(1)
	{
		mpu6050_get_gyro(&gyro[0],&gyro[1],&gyro[2]);
		
	}
}

