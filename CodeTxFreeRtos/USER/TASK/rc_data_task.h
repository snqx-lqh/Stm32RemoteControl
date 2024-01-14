#ifndef _RC_DATA_TASK_H
#define _RC_DATA_TASK_H

#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct{
	int16_t gyro[3];	
	int16_t acc[3];
	float   angle[3];

	uint8_t key_value;
	
	uint16_t lx_value;
	uint16_t ly_value;
	uint16_t rx_value;
	uint16_t ry_value;
	uint16_t power_value;
}rc_data_t;

extern rc_data_t rc_data;

void rc_data_task(void *pvParameters);
void rc_data_init(rc_data_t *rc_data);



#endif

