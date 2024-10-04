/**
  ******************************************************************************
  * @file    mpu6050_driver.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-3-31
  * @brief   mpu6050的驱动函数，使用的时候只需要重写6050的操作函数，如果有不同的
			 6050，只需要定义两个操作函数。
			 struct mpu6050_operations{
				 int (*iic_write_one_byte)(unsigned char reg,unsigned char data);
				 int (*iic_read_one_byte )(unsigned char reg,unsigned char *data);
				 int (*iic_read_len_byte )(unsigned char reg,unsigned char *data,unsigned char len);
				 void (*delay_ms)     (unsigned short ms);
			 };
			 需要复现这些操作函数，IIC写入读取返回0为正确读取。
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright {Year} LQH,China</center></h2>
  ******************************************************************************
  */
#include "mpu6050_driver.h"

/**
  * @brief   6050的初始化定义 
  * @param   
  * @retval  0 为成功初始化，1 为初始化失败
 **/
int mpu6050_init(struct mpu6050_operations *p_mpu6050_operations)
{
	uint8_t res = 0;
	p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
    p_mpu6050_operations->delay_ms(100);  
    p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT1_REG,0X00);//唤醒MPU6050
    mpu6050_set_gyro_fsr(p_mpu6050_operations,GYRO_2000DPS);					        	
    mpu6050_set_acc_fsr(p_mpu6050_operations,ACC_2G);					       	 	
    mpu6050_set_rate(p_mpu6050_operations,200);						       	 	
    p_mpu6050_operations->iic_write_one_byte(MPU_INT_EN_REG,0X01);   //数据中断
    p_mpu6050_operations->iic_write_one_byte(MPU_USER_CTRL_REG,0X00);//I2C主模式
    p_mpu6050_operations->iic_write_one_byte(MPU_FIFO_EN_REG,0X00);	 //关闭FIFO
    p_mpu6050_operations->iic_write_one_byte(MPU_INTBP_CFG_REG,0X9C);//选择中断电平，逻辑电平为0  50us高   查询立即自动清除
    p_mpu6050_operations->iic_read_one_byte (MPU_DEVICE_ID_REG,&res);//读取MPU6500的ID
    if(res==MPU6050_ADDR) //器件ID正确
    {
        p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT1_REG,0X01);  	//设置CLKSEL,PLL X轴为参考
        p_mpu6050_operations->iic_write_one_byte(MPU_PWR_MGMT2_REG,0X00);  	//加速度与陀螺仪都工作
    } else return 1;
    return 0;
}

/**
  * @brief   设置陀螺仪传感器满量程范围
  * @param   
  * @retval  0 为成功，1 为失败
 **/
uint8_t mpu6050_set_gyro_fsr(struct mpu6050_operations *p_mpu6050_operations,uint8_t fsr)
{
    return p_mpu6050_operations->iic_write_one_byte(MPU_GYRO_CFG_REG,(fsr<<3)|3);
}

/**
  * @brief   设置加速度传感器满量程范围
  * @param   
  * @retval  0 为成功，1 为失败
 **/
uint8_t mpu6050_set_acc_fsr(struct mpu6050_operations *p_mpu6050_operations,uint8_t fsr)
{
    return p_mpu6050_operations->iic_write_one_byte(MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
  * @brief   设置数字低通滤波器
  * @param   
  * @retval  0 为成功，1 为失败
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
  * @brief   设置MPU6050的采样率
  * @param   
  * @retval  0 为成功，1 为失败
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
  * @brief   读取MPU6050的温度值
  * @param   
  * @retval  温度值
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
  * @brief   读取MPU6050的gyro
  * @param   
  * @retval  0 为成功，1 为失败
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
  * @brief   读取MPU6050的acc
  * @param   
  * @retval  0 为成功，1 为失败
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

