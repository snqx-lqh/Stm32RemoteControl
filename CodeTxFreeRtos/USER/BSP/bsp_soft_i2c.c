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
			 void (*set_iic_sda_level)(unsigned char level);//SDA�����ƽ
			 void (*set_iic_scl_level)(unsigned char level);//SCL�����ƽ
			 uint8_t (*iic_sda_read)(void);   //SDA��ƽ��ȡ
			 void (*iic_delay)(void);         //��ʱ
			 
			 ���ڶ�ȡ��д��ĺ��������Ƿ���ֵΪ0ʱ�ɹ���Ϊ1ʧ��
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
  * @brief  iic��ʼ��
  * @param   
  * @retval
 **/
void soft_i2c_init(soft_i2c_t *soft_i2c)
{
	soft_i2c->soft_i2c_init();
}

/**
  * @brief  iic������ʼ�ź�
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
  * @brief  iic����ֹͣ�ź�
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
  * @brief  iic�ȴ�Ӧ���źŵ���
  * @param   
  * @retval 0������Ӧ��ɹ� 1������Ӧ��ʧ��
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
  * @brief  ����Ӧ���ź�
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
  * @brief  ������Ӧ��
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
  * @brief  ����һ���ֽ�
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
  * @brief   ��ȡһ���ֽ�
  * @param   ack 1ΪӦ��0Ϊ��Ӧ��
  * @retval  ��ȡ���ֽ�
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
        soft_iic_nack(soft_i2c); //��Ӧ��
    else
        soft_iic_ack(soft_i2c); //Ӧ��

    return receive;
}

/**
  * @brief   ��ָ��λ�ö�һ���ֽ�
  * @param   addr �豸��ַ��reg �Ĵ�����ַ��data ��ȡ���ݱ���λ��
  * @retval  0 �ɹ� 1 ʧ��
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
  * @brief   ��ָ��λ��дһ���ֽ�
  * @param   addr �豸��ַ��reg �Ĵ�����ַ��data ����
  * @retval  0 �ɹ� 1 ʧ��
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
  * @brief   ��ָ��λ�ö�len���ֽ�
  * @param   addr �豸��ַ��reg �Ĵ�����ַ��len ��ȡ��ַ���ȣ�buf ���ݶ�ȡ����λ��
  * @retval  0 �ɹ� 1 ʧ��
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
  * @brief   ��ָ��λ��дlen���ֽ�
  * @param   addr �豸��ַ��reg �Ĵ�����ַ��len ��ȡ��ַ���ȣ�buf д������
  * @retval  0 �ɹ� 1 ʧ��
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
