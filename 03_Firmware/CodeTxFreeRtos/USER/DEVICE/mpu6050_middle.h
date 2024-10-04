#ifndef _MPU6050_MIDDLE_H
#define _MPU6050_MIDDLE_H

#include "main.h"

/*  IIC_SCL时钟端口、引脚定义 */
#define IIC_SCL_PORT 			GPIOB
#define IIC_SCL_PIN 			(GPIO_Pin_6)
#define IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOB

/*  IIC_SDA时钟端口、引脚定义 */
#define IIC_SDA_PORT 			GPIOB
#define IIC_SDA_PIN 			(GPIO_Pin_7)
#define IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOB

//IO位带操作
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA，GPIOx_ODR寄存器 
#define READ_SDA   PBin(7)  //输入SDA，GPIOx_IDR寄存器

void mpu6050_middle_init(void);
void mpu6050_example(void);

extern struct mpu6050_operations mpu6050_oper;

#endif
