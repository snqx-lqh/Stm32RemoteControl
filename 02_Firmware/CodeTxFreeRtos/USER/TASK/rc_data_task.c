/**
  ******************************************************************************
  * @file    rc_data_task.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-10
  * @brief   遥控器数据获取
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "rc_data_task.h"

#include "semphr.h"

#include "bsp_adc.h"
#include "mpu6050_middle.h"
#include "mpu6050_driver.h"
#include "bsp_key.h"
#include "keyscan.h"
#include "ahrs.h"
#include "filter.h"
#include "math.h"

extern SemaphoreHandle_t rcDataMutexSemaphore;

rc_data_t rc_data;

void rc_data_task(void *pvParameters)
{
	rc_data_t rc_data_temp;
	angle_kalman_filter_t angle_kalman_filter_x ;
	angle_kalman_filter_t angle_kalman_filter_y ;
	float gyro[3]   = {0};
	float acc[3]    = {0};
	float gyroy_temp,gyrox_temp;
	float accy_temp,accx_temp;
	rc_data_init(&rc_data_temp);
	rc_data_init(&rc_data);
	kalman_filter_v1_init(&angle_kalman_filter_x);
	kalman_filter_v1_init(&angle_kalman_filter_y);
	mpu6050_middle_init();
	
	while(1)
	{
		rc_data_temp.lx_value     = get_lx_value();
		rc_data_temp.ly_value     = get_ly_value();
		rc_data_temp.rx_value     = get_rx_value();
		rc_data_temp.ry_value     = get_ry_value();
		rc_data_temp.power_value  = get_power_value();

		rc_data_temp.key_value = (KEY_UP << 7)|(KEY_DOWN << 6)|(KEY_LEFT << 5)|(KEY_RIGHT << 4)| \
								 (KEY_YG_L << 3)|(KEY_YG_R << 2)|(KEY_BM_L << 1)|(KEY_BM_R << 0);
		//读取按键的值
		KeyScan(0);
		//读取陀螺仪的值
		mpu6050_get_gyro(&mpu6050_oper,&rc_data_temp.gyro[0],&rc_data_temp.gyro[1],&rc_data_temp.gyro[2]);
		mpu6050_get_acc(&mpu6050_oper,&rc_data_temp.acc[0]  ,&rc_data_temp.acc[1] ,&rc_data_temp.acc[2]);
		
		gyro[0] = rc_data_temp.gyro[0] * MPU6050_GYRO_2000_SEN;
		gyro[1] = rc_data_temp.gyro[1] * MPU6050_GYRO_2000_SEN;
		gyro[2] = rc_data_temp.gyro[2] * MPU6050_GYRO_2000_SEN;
		acc[0]  = rc_data_temp.acc[0]  * MPU6050_ACCEL_2G_SEN;
		acc[1]  = rc_data_temp.acc[1]  * MPU6050_ACCEL_2G_SEN;
		acc[2]  = rc_data_temp.acc[2]  * MPU6050_ACCEL_2G_SEN;
		
//		accy_temp =atan2(rc_data_temp.acc[0],rc_data_temp.acc[2])*180/PI;                 //计算倾角	
//		gyroy_temp=-rc_data_temp.gyro[1]/16.4;                             //陀螺仪量程转换	
//		
//		kalman_filter_v1(&angle_kalman_filter_y,&accy_temp,&gyroy_temp,&rc_data_temp.angle[1]);
//		
//		accx_temp =atan2(rc_data_temp.acc[1],rc_data_temp.acc[2])*180/PI;                 //计算倾角	
//		gyrox_temp=-rc_data_temp.gyro[0]/16.4;                             //陀螺仪量程转换	
//		
//		kalman_filter_v1(&angle_kalman_filter_x,&accx_temp,&gyrox_temp,&rc_data_temp.angle[0]);
		
		//计算角度值
		MahonyImuUpdate(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2],rc_data_temp.angle);
		
		// 等待互斥量，确保在写入共享数据时不会被其他任务中断
        if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
            // 更新共享数据
            rc_data = rc_data_temp;
            // 释放互斥量
            xSemaphoreGive(rcDataMutexSemaphore);
        }
		vTaskDelay(5);
	}
}

void rc_data_init(rc_data_t *rc_data)
{
	rc_data->lx_value     = 0;
	rc_data->ly_value     = 0;
	rc_data->rx_value     = 0;
	rc_data->ry_value     = 0;
	rc_data->power_value  = 0;
	rc_data->gyro[0]      = 0;
	rc_data->gyro[1]      = 0;
	rc_data->gyro[2]      = 0;
	rc_data->acc[0]       = 0;
	rc_data->acc[1]       = 0;
	rc_data->acc[2]       = 0;
	rc_data->angle[0]     = 0;
	rc_data->angle[1]     = 0;
	rc_data->angle[2]     = 0;
}

