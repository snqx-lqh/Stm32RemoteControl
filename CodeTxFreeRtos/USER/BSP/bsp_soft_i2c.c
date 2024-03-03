/**
  ******************************************************************************
  * @file    bsp_soft_i2c.c 
  * @author  ����Ǳ��(snqx-lqh)
  * @version V
  * @date    2024-01-15
  * @brief   ���IICģ�飬ʹ��ģ�黯��̣��û�ʹ�õ�ʱ����Ҫ��дsoft_i2c_t�ṹ����
			 ����غ���
			 void (*soft_i2c_init)(void);     //��ʼ������
			 void (*soft_iic_sda_out)(void);  //SDA�������ģʽ
			 void (*soft_iic_sda_in)(void);   //SDA��������ģʽ
			 void (*iic_sda_high)(void);      //SDA����ߵ�ƽ
			 void (*iic_sda_low)(void);       //SDA����͵�ƽ
			 void (*iic_scl_high)(void);      //SCL����͵�ƽ
			 void (*iic_scl_low)(void);       //SCL����͵�ƽ
			 uint8_t (*iic_sda_read)(void);   //SDA��ƽ��ȡ
			 
			 soft_i2c_delay() �������ʱʱ����ݲ�ͬƽ̨�����޸�
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

//������ʼ�ź�
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

//����ֹͣ�ź�
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

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
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

//����Ӧ���ź�
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

//������Ӧ��
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

//����һ���ֽڣ�IIC�����������ֽ�Ϊ��λ���Ӹ�λ��ʼ���ͣ�
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

//��ȡһ���ֽڣ���������1ΪӦ��0Ϊ��Ӧ��
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
        soft_iic_nack(soft_i2c); //��Ӧ��
    else
        soft_iic_ack(soft_i2c); //Ӧ��

    return receive;
}

//��ָ��λ�ö�һ���ֽ�
uint8_t soft_iic_read_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg)
{
    uint8_t receive = 0;

    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);     //����Ѱַ�������߷���IIC�豸��ַ0XA0,��д����Ϊ0д����
    soft_iic_wait_ack(soft_i2c);                  //�ȴ�Ӧ�𣬽���ͨ������
    soft_iic_send_byte(soft_i2c,reg);             //���豸����Ҫ�����ĵ�ַ
    soft_iic_wait_ack(soft_i2c);
    soft_iic_start(soft_i2c);                     //��д����ı�ʱ����Ҫ���·�����ʼ�ź�
    soft_iic_send_byte(soft_i2c,(addr<<1)|1);     //��д����Ϊ1������
    soft_iic_wait_ack(soft_i2c);
    receive = soft_iic_read_byte(soft_i2c,0);     //����һ���ֽڣ����ͷ�Ӧ���ź�
    soft_iic_stop(soft_i2c);

    return receive;
}

//��ָ��λ��дһ���ֽ�
uint8_t soft_iic_write_dev_one_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t data)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);      //����Ѱַ�������߷���IIC�豸��ַ0XA0,��д����Ϊ0д����
    if(soft_iic_wait_ack(soft_i2c))                //�ȴ�Ӧ��
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg);              //���豸����Ҫ�����ĵ�ַ
    soft_iic_wait_ack(soft_i2c);
    soft_iic_send_byte(soft_i2c,data);             //����һ���ֽ�
    if(soft_iic_wait_ack(soft_i2c))                //�ȴ�Ӧ��
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_stop(soft_i2c);
    return 0;
}

//��ָ��λ�ö�len���ֽ�
uint8_t soft_iic_read_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);  //����������ַ+д����
    if(soft_iic_wait_ack(soft_i2c))          //�ȴ�Ӧ��
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg); //���豸����Ҫ�����ĵ�ַ
    soft_iic_wait_ack(soft_i2c);             //�ȴ�Ӧ��
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|1); //����������ַ+������
    soft_iic_wait_ack(soft_i2c);             //�ȴ�Ӧ��
    while(len)
    {
        if(len==1)*buf=soft_iic_read_byte(soft_i2c,0);//������,����nACK
        else *buf=soft_iic_read_byte(soft_i2c,1);		//������,����ACK
        len--;
        buf++;
    }
    soft_iic_stop(soft_i2c);                 //����һ��ֹͣ����
    return 0;
}

//��ָ��λ��дlen���ֽ�
uint8_t soft_iic_write_dev_len_byte(soft_i2c_t *soft_i2c,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    soft_iic_start(soft_i2c);
    soft_iic_send_byte(soft_i2c,(addr<<1)|0);  //����������ַ+д����
    if(soft_iic_wait_ack(soft_i2c))          //�ȴ�Ӧ��
    {
        soft_iic_stop(soft_i2c);
        return 1;
    }
    soft_iic_send_byte(soft_i2c,reg); //���豸����Ҫ�����ĵ�ַ
    soft_iic_wait_ack(soft_i2c);             //�ȴ�Ӧ��
    for(i=0; i<len; i++)
    {
        soft_iic_send_byte(soft_i2c,buf[i]);  //��������
        if(soft_iic_wait_ack(soft_i2c))          //�ȴ�Ӧ��
		{
			soft_iic_stop(soft_i2c);
			return 1;
		}
    }
    soft_iic_stop(soft_i2c);
    return 0;
}
