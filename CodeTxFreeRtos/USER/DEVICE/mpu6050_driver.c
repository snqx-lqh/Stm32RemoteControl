#include "mpu6050_driver.h"

static struct mpu6050_operations *p_mpu6050_operations;

void mpu6050_operation_register(struct mpu6050_operations *mpu6050_oper)
{
	p_mpu6050_operations = mpu6050_oper;
}

int mpu6050_init()
{
	uint8_t res = 0;
	p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
    p_mpu6050_operations->delay_ms(100);  //��ʱ100ms
    p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT1_REG,0X00);//����MPU6050
    mpu6050_set_gyro_fsr(GYRO_2000DPS);					        	
    mpu6050_set_acc_fsr(ACC_2G);					       	 	
    mpu6050_set_rate(200);						       	 	
    p_mpu6050_operations->write_one_byte(MPU_INT_EN_REG,0X01);   //�����ж�
    p_mpu6050_operations->write_one_byte(MPU_USER_CTRL_REG,0X00);//I2C��ģʽ
    p_mpu6050_operations->write_one_byte(MPU_FIFO_EN_REG,0X00);	 //�ر�FIFO
    p_mpu6050_operations->write_one_byte(MPU_INTBP_CFG_REG,0X9C);//ѡ���жϵ�ƽ���߼���ƽΪ0  50us��   ��ѯ�����Զ����
    p_mpu6050_operations->read_one_byte (MPU_DEVICE_ID_REG,&res);//��ȡMPU6500��ID
    if(res==MPU6050_ADDR) //����ID��ȷ
    {
        p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT1_REG,0X01);  	//����CLKSEL,PLL X��Ϊ�ο�
        p_mpu6050_operations->write_one_byte(MPU_PWR_MGMT2_REG,0X00);  	//���ٶ��������Ƕ�����
        mpu6050_set_rate(200);						       	//���ò�����Ϊ200Hz
    } else return 1;
    return 0;
}

//���������Ǵ����������̷�Χ
uint8_t mpu6050_set_gyro_fsr(uint8_t fsr)
{
    return p_mpu6050_operations->write_one_byte(MPU_GYRO_CFG_REG,(fsr<<3)|3);//���������������̷�Χ
}

//���ü��ٶȴ����������̷�Χ
uint8_t mpu6050_set_acc_fsr(uint8_t fsr)
{
    return p_mpu6050_operations->write_one_byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ
}

//�������ֵ�ͨ�˲���
uint8_t mpu6050_set_lpf(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return p_mpu6050_operations->write_one_byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���
}

//����MPU9250�Ĳ�����
uint8_t mpu6050_set_rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=p_mpu6050_operations->write_one_byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
    return mpu6050_set_lpf(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
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

