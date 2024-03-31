/**
  ******************************************************************************
  * @file    mpu6050_middle.c 
  * @author  ����Ǳ��(snqx-lgh)
  * @version V
  * @date    2024-02-29
  * @brief   mpu6050���м�㣬��ʼ����������
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

    //�����˿�ʱ��
    RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC | IIC_SDA_PORT_RCC, ENABLE);

    //����SCL��SDA����
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //�������
    GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //�������
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);

    GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);
    GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN); //�����豸���У��������ߵ�ƽ
}

void mpu_soft_iic_sda_out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //�������
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

void mpu_soft_iic_sda_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
    GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

void mpu_set_iic_sda_level(unsigned char level)
{
	if(0 == level)
		IIC_SDA_PORT->BRR = IIC_SDA_PIN;
	else
		IIC_SDA_PORT->BSRR = IIC_SDA_PIN;
}

void mpu_set_iic_scl_level(unsigned char level)
{
	if(0 == level)
		IIC_SCL_PORT->BRR = IIC_SCL_PIN;
	else
		IIC_SCL_PORT->BSRR = IIC_SCL_PIN;
}

uint8_t mpu_iic_sda_read(void)
{
	return GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN);
}

void mpu_iic_delay()
{
	
}

soft_i2c_t mpu_soft_iic={
	.soft_i2c_init = mpu_soft_i2c_init,
	.soft_iic_sda_out = mpu_soft_iic_sda_out, 
	.soft_iic_sda_in = mpu_soft_iic_sda_in,
	.set_iic_sda_level = mpu_set_iic_sda_level,
	.set_iic_scl_level = mpu_set_iic_scl_level,
	.iic_sda_read = mpu_iic_sda_read,
	.iic_delay    = mpu_iic_delay
};

int mpu6050_write_one_byte(unsigned char reg,unsigned char data)
{
	soft_iic_write_dev_one_byte(&mpu_soft_iic,MPU6050_ADDR,reg,data);
	return 0;
}
int mpu6050_read_one_byte (unsigned char reg,unsigned char *data)
{
	return soft_iic_read_dev_one_byte(&mpu_soft_iic,MPU6050_ADDR,reg,data);;
}
int mpu6050_read_bytes    (unsigned char reg,unsigned char *data,unsigned char len)
{
	soft_iic_read_dev_len_byte(&mpu_soft_iic,MPU6050_ADDR,reg,len,data);
	return 0;
}

//��ʼ��MPU6050��������
struct mpu6050_operations mpu6050_oper = {
	.iic_write_one_byte = mpu6050_write_one_byte,
	.iic_read_one_byte  = mpu6050_read_one_byte,
	.iic_read_len_byte  = mpu6050_read_bytes,
	.delay_ms       = delay_ms
};

void mpu6050_middle_init()
{
	//��ʼ��IIC����
	soft_i2c_init(&mpu_soft_iic);     
	//ע��6050��������
	mpu6050_init(&mpu6050_oper);
}

short gyro[3];

void mpu6050_example()
{
	mpu6050_middle_init();
	
	while(1)
	{
		mpu6050_get_gyro(&mpu6050_oper,&gyro[0],&gyro[1],&gyro[2]);
		
	}
}

