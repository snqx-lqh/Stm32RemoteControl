/**
  ******************************************************************************
  * @file    nrf_task.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-10
  * @brief   nrf24l01发送相关任务
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "nrf_task.h"

#include "nrf24L01_middle.h"
#include "nrf24L01_driver.h"
#include "utils.h"
#include "semphr.h"
#include "rc_data_task.h"

extern SemaphoreHandle_t rcDataMutexSemaphore;
extern nrf24L01_oper_t nrf24L01_oper;

int16_t check_nrf = 0;
uint8_t check_tx  = 0;

void nrf24l01_tx(void);

void nrf_task(void *pvParameters)
{
	nrf24L01_middle_init();
	check_nrf = NRF24L01_Check(&nrf24L01_oper);
	NRF24L01_TX_Mode(&nrf24L01_oper);
	while(1)
	{
		nrf24l01_tx();
		vTaskDelay(20);
	}
}

void nrf24l01_tx()
{
	int16_t  angle_temp;
	uint16_t check_sum=0;
	rc_data_t rc_data_temp;
	uint8_t nrf_tx_buff[32] = {0};
	
	// 等待互斥量，确保在写入共享数据时不会被其他任务中断
	if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
		// 更新共享数据
		rc_data_temp = rc_data;
		// 释放互斥量
		xSemaphoreGive(rcDataMutexSemaphore);
	}
	
	nrf_tx_buff[0] = 0xAA;
	nrf_tx_buff[1] = 0x55;
	
	nrf_tx_buff[2] = 0x20;
	
	//遥控器通道值
	nrf_tx_buff[3] = (rc_data_temp.lx_value & 0xFF00) >> 8;
 	nrf_tx_buff[4] = (rc_data_temp.lx_value & 0x00FF);
	nrf_tx_buff[5] = (rc_data_temp.ly_value & 0xFF00) >> 8;
 	nrf_tx_buff[6] = (rc_data_temp.ly_value & 0x00FF);
	nrf_tx_buff[7] = (rc_data_temp.rx_value & 0xFF00) >> 8;
 	nrf_tx_buff[8] = (rc_data_temp.rx_value & 0x00FF);
	nrf_tx_buff[9] = (rc_data_temp.ry_value & 0xFF00) >> 8;
 	nrf_tx_buff[10] = (rc_data_temp.ry_value & 0x00FF);
	//按键值
	nrf_tx_buff[11] = rc_data_temp.key_value;
	//陀螺仪原始值
	nrf_tx_buff[12] = (rc_data_temp.gyro[0] & 0xFF00) >> 8;
 	nrf_tx_buff[13] = (rc_data_temp.gyro[0] & 0x00FF);
	nrf_tx_buff[14] = (rc_data_temp.gyro[1] & 0xFF00) >> 8;
 	nrf_tx_buff[15] = (rc_data_temp.gyro[1] & 0x00FF);
	nrf_tx_buff[16] = (rc_data_temp.gyro[2] & 0xFF00) >> 8;
 	nrf_tx_buff[17] = (rc_data_temp.gyro[2] & 0x00FF);
	
	//加速度计原始值
	nrf_tx_buff[18] = (rc_data_temp.acc[0] & 0xFF00) >> 8;
 	nrf_tx_buff[19] = (rc_data_temp.acc[0] & 0x00FF);
	nrf_tx_buff[20] = (rc_data_temp.acc[1] & 0xFF00) >> 8;
 	nrf_tx_buff[21] = (rc_data_temp.acc[1] & 0x00FF);
	nrf_tx_buff[22] = (rc_data_temp.acc[2] & 0xFF00) >> 8;
 	nrf_tx_buff[23] = (rc_data_temp.acc[2] & 0x00FF);
	
	//角度原始值乘以100
	angle_temp = (int16_t)(rc_data_temp.angle[0]*100);
	nrf_tx_buff[24] = (angle_temp & 0xFF00) >> 8;
 	nrf_tx_buff[25] = (angle_temp & 0x00FF);
	angle_temp = (int16_t)(rc_data_temp.angle[1]*100);
	nrf_tx_buff[26] = (angle_temp & 0xFF00) >> 8;
 	nrf_tx_buff[27] = (angle_temp & 0x00FF);
	angle_temp = (int16_t)(rc_data_temp.angle[2]*100);
	nrf_tx_buff[28] = (angle_temp & 0xFF00) >> 8;
 	nrf_tx_buff[29] = (angle_temp & 0x00FF);
	
	//计算校验和
	for(int i=0;i<28;i++)
	{
		check_sum += nrf_tx_buff[i+2];
	}
	
	nrf_tx_buff[30] = (check_sum & 0xFF00) >> 8;
 	nrf_tx_buff[31] = (check_sum & 0x00FF);
	
	check_tx = NRF24L01_TxPacket(&nrf24L01_oper,nrf_tx_buff);
}


//void nrf24l01_tx()
//{
//	uchar_and_float_t uchar_and_float;
//	rc_data_t rc_data_temp;
//	uint8_t nrf_tx_buff[32] = {0};
//	
//	// 等待互斥量，确保在写入共享数据时不会被其他任务中断
//	if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
//		// 更新共享数据
//		rc_data_temp = rc_data;
//		// 释放互斥量
//		xSemaphoreGive(rcDataMutexSemaphore);
//	}
//	
//	nrf_tx_buff[0] = 0xA3;
//	
//	//遥控器通道值
//	nrf_tx_buff[1] = (rc_data_temp.lx_value & 0xFF00) >> 8;
// 	nrf_tx_buff[2] = (rc_data_temp.lx_value & 0x00FF);
//	nrf_tx_buff[3] = (rc_data_temp.ly_value & 0xFF00) >> 8;
// 	nrf_tx_buff[4] = (rc_data_temp.ly_value & 0x00FF);
//	nrf_tx_buff[5] = (rc_data_temp.rx_value & 0xFF00) >> 8;
// 	nrf_tx_buff[6] = (rc_data_temp.rx_value & 0x00FF);
//	nrf_tx_buff[7] = (rc_data_temp.ry_value & 0xFF00) >> 8;
// 	nrf_tx_buff[8] = (rc_data_temp.ry_value & 0x00FF);
//	//陀螺仪原始值
//	nrf_tx_buff[9]  = (rc_data_temp.gyro[0] & 0xFF00) >> 8;
// 	nrf_tx_buff[10] = (rc_data_temp.gyro[0] & 0x00FF);
//	nrf_tx_buff[11] = (rc_data_temp.gyro[1] & 0xFF00) >> 8;
// 	nrf_tx_buff[12] = (rc_data_temp.gyro[1] & 0x00FF);
//	nrf_tx_buff[13] = (rc_data_temp.gyro[2] & 0xFF00) >> 8;
// 	nrf_tx_buff[14] = (rc_data_temp.gyro[2] & 0x00FF);
//	//按键值
//	nrf_tx_buff[15] = rc_data_temp.key_value;
//	
//	uchar_and_float.float_data = rc_data_temp.angle[0];
// 	nrf_tx_buff[16] = uchar_and_float.uchar_data[0];
//	nrf_tx_buff[17] = uchar_and_float.uchar_data[1];
// 	nrf_tx_buff[18] = uchar_and_float.uchar_data[2];
//	nrf_tx_buff[19] = uchar_and_float.uchar_data[3];
//	
//	uchar_and_float.float_data = rc_data_temp.angle[1];
// 	nrf_tx_buff[20] = uchar_and_float.uchar_data[0];
//	nrf_tx_buff[21] = uchar_and_float.uchar_data[1];
// 	nrf_tx_buff[22] = uchar_and_float.uchar_data[2];
//	nrf_tx_buff[23] = uchar_and_float.uchar_data[3];
//	
//	uchar_and_float.float_data = rc_data_temp.angle[2];
// 	nrf_tx_buff[24] = uchar_and_float.uchar_data[0];
//	nrf_tx_buff[25] = uchar_and_float.uchar_data[1];
// 	nrf_tx_buff[26] = uchar_and_float.uchar_data[2];
//	nrf_tx_buff[27] = uchar_and_float.uchar_data[3];
//	
//	nrf_tx_buff[28] = 0x3A;
//	
//	check_tx = NRF24L01_TxPacket(&nrf24L01_oper,nrf_tx_buff);
//}
