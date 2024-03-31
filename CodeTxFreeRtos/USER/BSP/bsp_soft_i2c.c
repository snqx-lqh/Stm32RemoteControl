/**
  ******************************************************************************
  * @file    bsp_soft_i2c.c 
  * @author  少年潜行(snqx-lqh)
  * @version V
  * @date    2024-01-15
  * @brief   软件IIC模块，使用模块化编程，用户使用的时候需要重写soft_i2c_t结构体中
			 的相关函数
			 void (*soft_i2c_init)(void);     //初始化引脚
			 void (*soft_iic_sda_out)(void);  //SDA引脚输出模式
			 void (*soft_iic_sda_in)(void);   //SDA引脚输入模式
			 void (*set_iic_sda_level)(unsigned char level);//SDA输出电平
			 void (*set_iic_scl_level)(unsigned char level);//SCL输出电平
			 uint8_t (*iic_sda_read)(void);   //SDA电平读取
			 void (*iic_delay)(void);         //延时
			 
			 关于读取和写入的函数，都是返回值为0时成功，为1失败
			 uint8_t soft_iic_write_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t data);
			 uint8_t soft_iic_read_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t *data);

			 uint8_t soft_iic_write_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
			 uint8_t soft_iic_read_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "bsp_soft_i2c.h"

/**
  * @brief  iic初始化
  * @param   
  * @retval
 **/
void soft_i2c_init(soft_i2c_t *soft_i2c)
{
	soft_i2c->soft_i2c_init();
}

/**
  * @brief  iic产生起始信号
  * @param   
  * @retval
 **/
void soft_iic_start(soft_i2c_t *soft_i2c)
{
    soft_i2c->soft_iic_sda_out(); 

    soft_i2c->set_iic_sda_level(1);
    soft_i2c->set_iic_scl_level(1);
    soft_i2c->iic_delay();
    soft_i2c->set_iic_sda_level(0); 
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(0); 
}

/**
  * @brief  iic产生停止信号
  * @param   
  * @retval
 **/
void soft_iic_stop(soft_i2c_t *soft_i2c)
{
    soft_i2c->soft_iic_sda_out();
    soft_i2c->set_iic_scl_level(0);
    soft_i2c->set_iic_sda_level(0);
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(1);
    soft_i2c->set_iic_sda_level(1); 
    soft_i2c->iic_delay();
}

/**
  * @brief  iic等待应答信号到来
  * @param   
  * @retval 0，接收应答成功 1，接收应答失败
 **/
uint8_t soft_iic_wait_ack(soft_i2c_t *soft_i2c)
{
    uint8_t ucErrTime=0;
    soft_i2c->soft_iic_sda_in();      
    soft_i2c->set_iic_sda_level(1);
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(1);
    soft_i2c->iic_delay();
    while(soft_i2c->iic_sda_read())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            soft_iic_stop(soft_i2c);
            return 1;
        }
    }
    soft_i2c->set_iic_scl_level(0);
    return 0;
}

/**
  * @brief  产生应答信号
  * @param   
  * @retval 
 **/
void soft_iic_ack(soft_i2c_t *soft_i2c)
{
    soft_i2c->set_iic_scl_level(0);
    soft_i2c->soft_iic_sda_out();
    soft_i2c->set_iic_sda_level(0); 
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(1);
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(0);
}

/**
  * @brief  不产生应答
  * @param   
  * @retval 
 **/
void soft_iic_nack(soft_i2c_t *soft_i2c)
{
    soft_i2c->set_iic_scl_level(0);
    soft_i2c->soft_iic_sda_out();
    soft_i2c->set_iic_sda_level(1);
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(1);
    soft_i2c->iic_delay();
    soft_i2c->set_iic_scl_level(0);
}

/**
  * @brief  发送一个字节
  * @param   
  * @retval 
 **/
void soft_iic_send_byte(soft_i2c_t *soft_i2c,uint8_t txd)
{
    uint8_t i;
    soft_i2c->soft_iic_sda_out();; 
    soft_i2c->set_iic_scl_level(0);
    for (i = 0; i < 8; i++)
    {
		if((txd&0x80)>>7)
			soft_i2c->set_iic_sda_level(1); 
		else
			soft_i2c->set_iic_sda_level(0);
        txd<<=1;
        soft_i2c->set_iic_scl_level(1);
        soft_i2c->iic_delay();
        soft_i2c->set_iic_scl_level(0);
        soft_i2c->iic_delay();
    }
}

/**
  * @brief   读取一个字节
  * @param   ack 1为应答，0为非应答
  * @retval  读取的字节
 **/
uint8_t soft_iic_read_byte(soft_i2c_t *soft_i2c,uint8_t ack)
{
    uint8_t i,receive = 0; 

    soft_i2c->soft_iic_sda_in(); 

    for (i = 0; i < 8; i++)
    {
        soft_i2c->set_iic_scl_level(0);
        soft_i2c->iic_delay();
        soft_i2c->set_iic_scl_level(1);
        receive <<= 1; 
        if (soft_i2c->iic_sda_read())
        {
            receive++; 
        }
        soft_i2c->iic_delay();
    }

    if (!ack)
        soft_iic_nack(soft_i2c); //非应答
    else
        soft_iic_ack(soft_i2c); //应答

    return receive;
}

/**
  * @brief   从指定位置读一个字节
  * @param   addr 设备地址，reg 寄存器地址，data 读取数据保存位置
  * @retval  0 成功 1 失败
 **/
uint8_t soft_iic_read_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t *data)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);     
    if(soft_iic_wait_ack(soft_i2c))               
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg);             
    if(soft_iic_wait_ack(soft_i2c))               
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_start(soft_i2c);                     
    soft_iic_send_byte(soft_i2c,(addr<<1)|1);     
    if(soft_iic_wait_ack(soft_i2c))               
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    *data = soft_iic_read_byte(soft_i2c,0);    
    soft_iic_stop(soft_i2c);

    return 0;
}

/**
  * @brief   向指定位置写一个字节
  * @param   addr 设备地址，reg 寄存器地址，data 数据
  * @retval  0 成功 1 失败
 **/
uint8_t soft_iic_write_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t data)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);      
    if(soft_iic_wait_ack(soft_i2c))               
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg);             
    if(soft_iic_wait_ack(soft_i2c))                
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,data);             
    if(soft_iic_wait_ack(soft_i2c))                
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_stop(soft_i2c);
    return 0;
}

/**
  * @brief   从指定位置读len个字节
  * @param   addr 设备地址，reg 寄存器地址，len 读取地址长度，buf 数据读取保存位置
  * @retval  0 成功 1 失败
 **/
uint8_t soft_iic_read_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);  
    if(soft_iic_wait_ack(soft_i2c))          
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg); 
    if(soft_iic_wait_ack(soft_i2c))                
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|1); 
    if(soft_iic_wait_ack(soft_i2c))                
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    while(len)
    {
        if(len==1)*buf=soft_iic_read_byte(soft_i2c,0);
        else *buf=soft_iic_read_byte(soft_i2c,1);		
        len--;
        buf++;
    }
    soft_iic_stop(soft_i2c);                
    return 0;
}

/**
  * @brief   从指定位置写len个字节
  * @param   addr 设备地址，reg 寄存器地址，len 读取地址长度，buf 写入数据
  * @retval  0 成功 1 失败
 **/
uint8_t soft_iic_write_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);  
    if(soft_iic_wait_ack(soft_i2c))         
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg); 
    if(soft_iic_wait_ack(soft_i2c))               
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    for(i=0; i<len; i++)
    {
        soft_iic_send_byte(soft_i2c,buf[i]);  
        if(soft_iic_wait_ack(soft_i2c))          
		{
			soft_iic_stop(soft_i2c);
			return 1;
		}
    }
    soft_iic_stop(soft_i2c);
    return 0;
}
