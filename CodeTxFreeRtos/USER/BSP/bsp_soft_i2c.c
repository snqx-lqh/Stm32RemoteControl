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
			 void (*iic_sda_high)(void);      //SDA输出高电平
			 void (*iic_sda_low)(void);       //SDA输出低电平
			 void (*iic_scl_high)(void);      //SCL输出低电平
			 void (*iic_scl_low)(void);       //SCL输出低电平
			 uint8_t (*iic_sda_read)(void);   //SDA电平读取
			 
			 soft_i2c_delay() 里面的延时时间根据不同平台自行修改
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "bsp_soft_i2c.h"

void soft_i2c_delay()
{
	//for(int i=3;i>0;i--);
}

void soft_i2c_init(soft_i2c_t *soft_i2c)
{
	soft_i2c->soft_i2c_init();
}

//产生起始信号
void soft_iic_start(soft_i2c_t *soft_i2c)
{
    soft_i2c->soft_iic_sda_out(); 

    soft_i2c->iic_sda_high();
    soft_i2c->iic_scl_high();
    soft_i2c_delay();
    soft_i2c->iic_sda_low(); 
    soft_i2c_delay();
    soft_i2c->iic_scl_low(); 
}

//产生停止信号
void soft_iic_stop(soft_i2c_t *soft_i2c)
{
    soft_i2c->soft_iic_sda_out();
    soft_i2c->iic_scl_low();
    soft_i2c->iic_sda_low();
    soft_i2c_delay();
    soft_i2c->iic_scl_high();
    soft_i2c->iic_sda_high(); 
    soft_i2c_delay();
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t soft_iic_wait_ack(soft_i2c_t *soft_i2c)
{
    uint8_t ucErrTime=0;
    soft_i2c->soft_iic_sda_in();      
    soft_i2c->iic_sda_high();
    soft_i2c_delay();
    soft_i2c->iic_scl_high();
    soft_i2c_delay();
    while(soft_i2c->iic_sda_read())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            soft_iic_stop(soft_i2c);
            return 1;
        }
    }
    soft_i2c->iic_scl_low();
    return 0;
}

//产生应答信号
void soft_iic_ack(soft_i2c_t *soft_i2c)
{
    soft_i2c->iic_scl_low();
    soft_i2c->soft_iic_sda_out();
    soft_i2c->iic_sda_low(); 
    soft_i2c_delay();
    soft_i2c->iic_scl_high();
    soft_i2c_delay();
    soft_i2c->iic_scl_low();
}

//不产生应答
void soft_iic_nack(soft_i2c_t *soft_i2c)
{
    soft_i2c->iic_scl_low();
    soft_i2c->soft_iic_sda_out();
    soft_i2c->iic_sda_high();
    soft_i2c_delay();
    soft_i2c->iic_scl_high();
    soft_i2c_delay();
    soft_i2c->iic_scl_low();
}

//发送一个字节（IIC发送数据以字节为单位，从高位开始发送）
void soft_iic_send_byte(soft_i2c_t *soft_i2c,uint8_t txd)
{
    uint8_t i;
    soft_i2c->soft_iic_sda_out();; 
    soft_i2c->iic_scl_low();
    for (i = 0; i < 8; i++)
    {
		if((txd&0x80)>>7)
			soft_i2c->iic_sda_high(); 
		else
			soft_i2c->iic_sda_low();
        txd<<=1;
        soft_i2c->iic_scl_high();
        soft_i2c_delay();
        soft_i2c->iic_scl_low();
        soft_i2c_delay();
    }
}

//读取一个字节，参数传入1为应答，0为非应答
uint8_t soft_iic_read_byte(soft_i2c_t *soft_i2c,uint8_t ack)
{
    uint8_t i,receive = 0; 

    soft_i2c->soft_iic_sda_in(); 

    for (i = 0; i < 8; i++)
    {
        soft_i2c->iic_scl_low();
        soft_i2c_delay();
        soft_i2c->iic_scl_high();
        receive <<= 1; 
        if (soft_i2c->iic_sda_read())
        {
            receive++; 
        }
        soft_i2c_delay();
    }

    if (!ack)
        soft_iic_nack(soft_i2c); //非应答
    else
        soft_iic_ack(soft_i2c); //应答

    return receive;
}

//从指定位置读一个字节
uint8_t soft_iic_read_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg)
{
    uint8_t receive = 0;

    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);     //总线寻址：向总线发送IIC设备地址0XA0,读写方向为0写数据
    soft_iic_wait_ack(soft_i2c);                  //等待应答，建立通信连接
    soft_iic_send_byte(soft_i2c,reg);             //向设备发送要操作的地址
    soft_iic_wait_ack(soft_i2c);
    soft_iic_start(soft_i2c);                     //读写方向改变时，需要重新发送起始信号
    soft_iic_send_byte(soft_i2c,(addr<<1)|1);     //读写方向为1读数据
    soft_iic_wait_ack(soft_i2c);
    receive = soft_iic_read_byte(soft_i2c,0);     //接收一个字节，发送非应答信号
    soft_iic_stop(soft_i2c);

    return receive;
}

//向指定位置写一个字节
uint8_t soft_iic_write_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t data)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);      //总线寻址：向总线发送IIC设备地址0XA0,读写方向为0写数据
    if(soft_iic_wait_ack(soft_i2c))                //等待应答
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg);              //向设备发送要操作的地址
    soft_iic_wait_ack(soft_i2c);
    soft_iic_send_byte(soft_i2c,data);             //发送一个字节
    if(soft_iic_wait_ack(soft_i2c))                //等待应答
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_stop(soft_i2c);
    return 0;
}

//从指定位置读len个字节
uint8_t soft_iic_read_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);  //发送器件地址+写命令
    if(soft_iic_wait_ack(soft_i2c))          //等待应答
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg); //向设备发送要操作的地址
    soft_iic_wait_ack(soft_i2c);             //等待应答
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|1); //发送器件地址+读命令
    soft_iic_wait_ack(soft_i2c);             //等待应答
    while(len)
    {
        if(len==1)*buf=soft_iic_read_byte(soft_i2c,0);//读数据,发送nACK
        else *buf=soft_iic_read_byte(soft_i2c,1);		//读数据,发送ACK
        len--;
        buf++;
    }
    soft_iic_stop(soft_i2c);                 //产生一个停止条件
    return 0;
}

//向指定位置写len个字节
uint8_t soft_iic_write_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);  //发送器件地址+写命令
    if(soft_iic_wait_ack(soft_i2c))          //等待应答
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg); //向设备发送要操作的地址
    soft_iic_wait_ack(soft_i2c);             //等待应答
    for(i=0; i<len; i++)
    {
        soft_iic_send_byte(soft_i2c,buf[i]);  //发送数据
        if(soft_iic_wait_ack(soft_i2c))          //等待应答
		{
			soft_iic_stop(soft_i2c);
			return 1;
		}
    }
    soft_iic_stop(soft_i2c);
    return 0;
}
