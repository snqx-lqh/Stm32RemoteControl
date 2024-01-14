/**
  ******************************************************************************
  * @file    ano_task.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-13
  * @brief   匿名助手使用
  ******************************************************************************
  * @attention 
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "ano_task.h"

#include "semphr.h"

#include "rc_data_task.h"
#include "AnoPTv8.h"
#include "math.h"

void AnoPTv8TxFrameF1(uint8_t _u8, uint16_t _u16, int16_t _s16, float _f);
void AnoPTv8TxFrameF2(void);
void AnoPTv8TxFrame01(int16_t acc[3], int16_t gyro[3], uint8_t shock_state);
void AnoPTv8TxFrame0A(float angle[3]);

extern SemaphoreHandle_t rcDataMutexSemaphore;

void ano_task(void *pvParameters)
{
	static uint16_t sendCount = 0;
	static int16_t _testcnt = 0;
	rc_data_t rc_data_temp;
	
	while(1)
	{	
		sendCount ++;
		_testcnt++;
		if(sendCount > 5)
		{
			//AnoPTv8TxFrameF1(_testcnt,_testcnt,_testcnt,_testcnt/10);
			//AnoPTv8TxFrameF2();
			// 等待互斥量，确保在写入共享数据时不会被其他任务中断
			if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
				// 更新共享数据
				rc_data_temp = rc_data;
				// 释放互斥量
				xSemaphoreGive(rcDataMutexSemaphore);
			}
			//AnoPTv8TxFrame01(rc_data_temp.acc,rc_data_temp.gyro,0);
			AnoPTv8TxFrame0A(rc_data_temp.angle);
			sendCount = 0;
		}
		AnoPTv8HwTrigger1ms();
		vTaskDelay(1);
	}
}

void AnoPTv8TxFrame01(int16_t acc[3], int16_t gyro[3], uint8_t shock_state)
{
	uint8_t databuf[13];
	databuf[0]  = BYTE0(acc[0]);
	databuf[1]  = BYTE1(acc[0]);
	databuf[2]  = BYTE0(acc[1]);
	databuf[3]  = BYTE1(acc[1]);
	databuf[4]  = BYTE0(acc[2]);
	databuf[5]  = BYTE1(acc[2]);
	databuf[6]  = BYTE0(gyro[0]);
	databuf[7]  = BYTE1(gyro[0]);
	databuf[8]  = BYTE0(gyro[1]);
	databuf[9]  = BYTE1(gyro[1]);
	databuf[10] = BYTE0(gyro[2]);
	databuf[11] = BYTE1(gyro[2]);
	databuf[12] = BYTE0(shock_state);
	
	AnoPTv8SendBuf(ANOPTV8DEVID_SWJ, 0x01, databuf, 13);
}

void AnoPTv8TxFrame0A(float angle[3])
{
	uint8_t databuf[6];
	int16_t angle_int[3] = {0};
	
	angle_int[0] = (int16_t) angle[0];
	angle_int[1] = (int16_t) angle[1];
	angle_int[2] = (int16_t) angle[2];
	
	databuf[0]  = BYTE0(angle_int[0]);
	databuf[1]  = BYTE1(angle_int[0]);
	databuf[2]  = BYTE0(angle_int[1]);
	databuf[3]  = BYTE1(angle_int[1]);
	databuf[4]  = BYTE0(angle_int[2]);
	databuf[5]  = BYTE1(angle_int[2]);
	
	AnoPTv8SendBuf(ANOPTV8DEVID_SWJ, 0x0A, databuf, 6);
}

//void AnoPTv8TxFrame0A(float angle[3])
//{
//	uint8_t databuf[12];

//	databuf[0]  = BYTE0(angle[0]);
//	databuf[1]  = BYTE1(angle[0]);
//	databuf[2]  = BYTE2(angle[0]);
//	databuf[3]  = BYTE3(angle[0]);
//	databuf[4]  = BYTE0(angle[1]);
//	databuf[5]  = BYTE1(angle[1]);
//	databuf[6]  = BYTE2(angle[1]);
//	databuf[7]  = BYTE3(angle[1]);
//	databuf[8]  = BYTE0(angle[2]);
//	databuf[9]  = BYTE1(angle[2]);
//	databuf[10]  = BYTE2(angle[2]);
//	databuf[11]  = BYTE3(angle[2]);
//	
//	AnoPTv8SendBuf(ANOPTV8DEVID_SWJ, 0x0A, databuf, 12);
//}

void AnoPTv8TxFrameF1(uint8_t _u8, uint16_t _u16, int16_t _s16, float _f)
{
	uint8_t databuf[9];
	databuf[0] = _u8;
	databuf[1] = BYTE0(_u16);
	databuf[2] = BYTE1(_u16);
	databuf[3] = BYTE0(_s16);
	databuf[4] = BYTE1(_s16);
	databuf[5] = BYTE0(_f);
	databuf[6] = BYTE1(_f);
	databuf[7] = BYTE2(_f);
	databuf[8] = BYTE3(_f);
	
	AnoPTv8SendBuf(ANOPTV8DEVID_SWJ, 0xF1, databuf, 9);
}

void AnoPTv8TxFrameF2(void)
{
	static uint16_t scnt = 0;
	scnt++;
	if(scnt >= 360)
		scnt = 0;
	
	uint16_t v1 = scnt;
	float v2 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
	float v3 = sin((double)(scnt+120) / 180 * 3.14159) * 180 + 180;
	float v4 = sin((double)(scnt+240) / 180 * 3.14159) * 180 + 180;
	
	
	uint8_t databuf[14];
	databuf[0] = BYTE0(v1);
	databuf[1] = BYTE1(v1);
	databuf[2] = BYTE0(v2);
	databuf[3] = BYTE1(v2);
	databuf[4] = BYTE2(v2);
	databuf[5] = BYTE3(v2);
	databuf[6] = BYTE0(v3);
	databuf[7] = BYTE1(v3);
	databuf[8] = BYTE2(v3);
	databuf[9] = BYTE3(v3);
	databuf[10] = BYTE0(v4);
	databuf[11] = BYTE1(v4);
	databuf[12] = BYTE2(v4);
	databuf[13] = BYTE3(v4);
	
	AnoPTv8SendBuf(ANOPTV8DEVID_SWJ, 0xF2, databuf, 14);
}
