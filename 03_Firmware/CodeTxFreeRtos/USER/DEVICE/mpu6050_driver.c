/**
  ******************************************************************************
  * @file    mpu6050_driver.c 
  * @author  ����Ǳ��(snqx-lgh)
  * @version V
  * @date    2024-3-31
  * @brief   mpu6050������������ʹ�õ�ʱ��ֻ��Ҫ��д6050�Ĳ�������������в�ͬ��
			 6050��ֻ��Ҫ������������������
			 struct mpu6050_operations{
				 int (*iic_write_one_byte)(unsigned char reg,unsigned char data);
				 int (*iic_read_one_byte )(unsigned char reg,unsigned char *data);
				 int (*iic_read_len_byte )(unsigned char reg,unsigned char *data,unsigned char len);
				 void (*delay_ms)     (unsigned short ms);
			 };
			 ��Ҫ������Щ����������IICд���ȡ����0Ϊ��ȷ��ȡ��
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright {Year} LQH,China</center></h2>
  ******************************************************************************
  */
#include "mpu6050_driver.h"

/**
  * @brief   6050�ĳ�ʼ������ 
  * @param   
  * @retval  0 Ϊ�ɹ���ʼ����1 Ϊ��ʼ��ʧ��
 **/
int mpu6050_init(struct mpu6050_operations *p_mpu6050_operations)
{
	uint8_t res = 0;
	p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
    p_mpu6050_operations->delay_ms(100);  
    p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT1_REG,0X00);//����MPU6050
    mpu6050_set_gyro_fsr(p_mpu6050_operations,GYRO_2000DPS);					        	
    mpu6050_set_acc_fsr(p_mpu6050_operations,ACC_2G);					       	 	
    mpu6050_set_rate(p_mpu6050_operations,200);						       	 	
    p_mpu6050_operations->iic_write_one_byte(MPU_INT_EN_REG,0X01);   //�����ж�
    p_mpu6050_operations->iic_write_one_byte(MPU_USER_CTRL_REG,0X00);//I2C��ģʽ
    p_mpu6050_operations->iic_write_one_byte(MPU_FIFO_EN_REG,0X00);	 //�ر�FIFO
    p_mpu6050_operations->iic_write_one_byte(MPU_INTBP_CFG_REG,0X9C);//ѡ���жϵ�ƽ���߼���ƽΪ0  50us��   ��ѯ�����Զ����
    p_mpu6050_operations->iic_read_one_byte (MPU_DEVICE_ID_REG,&res);//��ȡMPU6500��ID
    if(res==MPU6050_ADDR) //����ID��ȷ
    {
        p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT1_REG,0X01);  	//����CLKSEL,PLL X��Ϊ�ο�
        p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT2_REG,0X00);  	//���ٶ��������Ƕ�����
    } else return 1;
    return 0;
}

/**
  * @brief   ���������Ǵ����������̷�Χ
  * @param   
  * @retval  0 Ϊ�ɹ���1 Ϊʧ��
 **/
uint8_t mpu6050_set_gyro_fsr(struct mpu6050_operations *p_mpu6050_operations,uint8_t fsr)
{
    return p_mpu6050_operations->iic_write_one_byte(MPU_GYRO_CFG_REG,(fsr<<3)|3);
}

/**
  * @brief   ���ü��ٶȴ����������̷�Χ
  * @param   
  * @retval  0 Ϊ�ɹ���1 Ϊʧ��
 **/
uint8_t mpu6050_set_acc_fsr(struct mpu6050_operations *p_mpu6050_operations,uint8_t fsr)
{
    return p_mpu6050_operations->iic_write_one_byte(MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
  * @brief   �������ֵ�ͨ�˲���
  * @param   
  * @retval  0 Ϊ�ɹ���1 Ϊʧ��
 **/
uint8_t mpu6050_set_lpf(struct mpu6050_operations *p_mpu6050_operations,uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return p_mpu6050_operations->iic_write_one_byte(MPU_CFG_REG,data);
}

/**
  * @brief   ����MPU6050�Ĳ�����
  * @param   
  * @retval  0 Ϊ�ɹ���1 Ϊʧ��
 **/
uint8_t mpu6050_set_rate(struct mpu6050_operations *p_mpu6050_operations,uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=p_mpu6050_operations->iic_write_one_byte(MPU_SAMPLE_RATE_REG,data);	
    return mpu6050_set_lpf(p_mpu6050_operations,rate/2);	
}

/**
  * @brief   ��ȡMPU6050���¶�ֵ
  * @param   
  * @retval  �¶�ֵ
 **/
short mpu6050_get_temperature(struct mpu6050_operations *p_mpu6050_operations)
{
    uint8_t buf[2];
    short raw;
    p_mpu6050_operations->iic_read_len_byte(MPU_TEMP_OUTH_REG,buf,2);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    return raw;
}

/**
  * @brief   ��ȡMPU6050��gyro
  * @param   
  * @retval  0 Ϊ�ɹ���1 Ϊʧ��
 **/
uint8_t mpu6050_get_gyro(struct mpu6050_operations *p_mpu6050_operations,short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;
    res=p_mpu6050_operations->iic_read_len_byte(MPU_GYRO_XOUTH_REG,buf,6);
    if(res==0)
    {
        *gx=(((uint16_t)buf[0]<<8)|buf[1]);
        *gy=(((uint16_t)buf[2]<<8)|buf[3]);
        *gz=(((uint16_t)buf[4]<<8)|buf[5]);
    }
    return res;
}

/**
  * @brief   ��ȡMPU6050��acc
  * @param   
  * @retval  0 Ϊ�ɹ���1 Ϊʧ��
 **/
uint8_t mpu6050_get_acc(struct mpu6050_operations *p_mpu6050_operations,short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;
    res=p_mpu6050_operations->iic_read_len_byte(MPU_ACCEL_XOUTH_REG,buf,6);
    if(res==0)
    {
        *ax=(((uint16_t)buf[0]<<8)|buf[1]);
        *ay=(((uint16_t)buf[2]<<8)|buf[3]);
        *az=(((uint16_t)buf[4]<<8)|buf[5]);
    }
    return res;
}

