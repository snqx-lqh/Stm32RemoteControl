#include "mpu6050_driver.h"

static struct mpu6050_operations *p_mpu6050_operations;

void mpu6050_operation_register(struct mpu6050_operations *mpu6050_oper)
{
	p_mpu6050_operations = mpu6050_oper;
}

int mpu6050_init()
{
	uint8_t res = 0;
	p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
    p_mpu6050_operations->delay_ms(100);  //延时100ms
    p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT1_REG,0X00);//唤醒MPU6050
    mpu6050_set_gyro_fsr(GYRO_2000DPS);					        	
    mpu6050_set_acc_fsr(ACC_2G);					       	 	
    mpu6050_set_rate(200);						       	 	
    p_mpu6050_operations->write_one_byte(MPU_INT_EN_REG,0X01);   //数据中断
    p_mpu6050_operations->write_one_byte(MPU_USER_CTRL_REG,0X00);//I2C主模式
    p_mpu6050_operations->write_one_byte(MPU_FIFO_EN_REG,0X00);	 //关闭FIFO
    p_mpu6050_operations->write_one_byte(MPU_INTBP_CFG_REG,0X9C);//选择中断电平，逻辑电平为0  50us高   查询立即自动清除
    p_mpu6050_operations->read_one_byte (MPU_DEVICE_ID_REG,&res);//读取MPU6500的ID
    if(res==MPU6050_ADDR) //器件ID正确
    {
        p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT1_REG,0X01);  	//设置CLKSEL,PLL X轴为参考
        p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT2_REG,0X00);  	//加速度与陀螺仪都工作
        mpu6050_set_rate(200);						       	//设置采样率为200Hz
    } else return 1;
    return 0;
}

//设置陀螺仪传感器满量程范围
uint8_t mpu6050_set_gyro_fsr(uint8_t fsr)
{
    return p_mpu6050_operations->write_one_byte(MPU_GYRO_CFG_REG,(fsr<<3)|3);//设置陀螺仪满量程范围
}

//设置加速度传感器满量程范围
uint8_t mpu6050_set_acc_fsr(uint8_t fsr)
{
    return p_mpu6050_operations->write_one_byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围
}

//设置数字低通滤波器
uint8_t mpu6050_set_lpf(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return p_mpu6050_operations->write_one_byte(MPU_CFG_REG,data);//设置数字低通滤波器
}

//设置MPU9250的采样率
uint8_t mpu6050_set_rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=p_mpu6050_operations->write_one_byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
    return mpu6050_set_lpf(rate/2);	//自动设置LPF为采样率的一半
}

short mpu6050_get_temperature(void)
{
    uint8_t buf[2];
    short raw;
    p_mpu6050_operations->read_bytes(MPU_TEMP_OUTH_REG,buf,2);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    return raw;
}

uint8_t mpu6050_get_gyro(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;
    res=p_mpu6050_operations->read_bytes(MPU_GYRO_XOUTH_REG,buf,6);
    if(res==0)
    {
        *gx=(((uint16_t)buf[0]<<8)|buf[1]);
        *gy=(((uint16_t)buf[2]<<8)|buf[3]);
        *gz=(((uint16_t)buf[4]<<8)|buf[5]);
    }
    return res;
}

uint8_t mpu6050_get_acc(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;
    res=p_mpu6050_operations->read_bytes(MPU_ACCEL_XOUTH_REG,buf,6);
    if(res==0)
    {
        *ax=(((uint16_t)buf[0]<<8)|buf[1]);
        *ay=(((uint16_t)buf[2]<<8)|buf[3]);
        *az=(((uint16_t)buf[4]<<8)|buf[5]);
    }
    return res;
}

