#include "user_task.h"

#include "keyscan.h"
#include "mytime.h"
#include "mpu6050.h"
#include "ahrs.h"

float gyro[3]   = {0};
float acc[3]    = {0};
float angle[3]  = {0};

void user_task(void *pvParameters)
{
	
	while(1)
	{
		KeyScan(0);
		MPU_Get_Gyro(&gyrox,&gyroy,&gyroz);
		MPU_Get_Acc(&accx,&accy,&accz);
		gyro[0] = gyrox * MPU6050_GYRO_2000_SEN;
		gyro[1] = gyroy * MPU6050_GYRO_2000_SEN;
		gyro[2] = gyroz * MPU6050_GYRO_2000_SEN;
		acc[0]  = accx  * MPU6050_ACCEL_2G_SEN;
		acc[1]  = accy  * MPU6050_ACCEL_2G_SEN;
		acc[2]  = accz  * MPU6050_ACCEL_2G_SEN;
		MahonyImuUpdate(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2],angle);
		
		MyTimeSecondRun();
		MyTimeFun();
		vTaskDelay(5);
	}
}
