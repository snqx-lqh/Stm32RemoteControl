/**
  ******************************************************************************
  * @file    bsp_soft_i2c.h 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#ifndef _BSP_SOFT_IIC_H
#define _BSP_SOFT_IIC_H

#include <stdint.h>

typedef struct{
	void (*soft_i2c_init)(void);
	void (*soft_iic_sda_out)(void);
	void (*soft_iic_sda_in)(void);
	void (*set_iic_sda_level)(unsigned char level);
	void (*set_iic_scl_level)(unsigned char level);
	uint8_t (*iic_sda_read)(void);
	void (*iic_delay)(void);
}soft_i2c_t;

void soft_i2c_init(soft_i2c_t *soft_i2c);

void soft_iic_start(soft_i2c_t *soft_i2c);
void soft_iic_stop(soft_i2c_t *soft_i2c);
uint8_t soft_iic_wait_ack(soft_i2c_t *soft_i2c);
void soft_iic_ack(soft_i2c_t *soft_i2c);
void soft_iic_nack(soft_i2c_t *soft_i2c);

void soft_iic_send_byte(soft_i2c_t *soft_i2c,uint8_t txd);
uint8_t soft_iic_read_byte(soft_i2c_t *soft_i2c,uint8_t ack);

uint8_t soft_iic_write_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t data);
uint8_t soft_iic_read_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t *data);

uint8_t soft_iic_write_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t soft_iic_read_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);

#endif
