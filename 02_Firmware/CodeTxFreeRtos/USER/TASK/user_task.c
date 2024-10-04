/**
  ******************************************************************************
  * @file    user_task.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-10
  * @brief   用户普通任务，拿来做一些检测性质的任务
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */#include "user_task.h"

extern TaskHandle_t   USERTask_Handler;
extern TaskHandle_t   NRFTask_Handler;
extern TaskHandle_t   GUITask_Handler;
extern TaskHandle_t   RC_DATATask_Handler;

void userTaskGetStackHighWaterMark(void);

void user_task(void *pvParameters)
{
	
	while(1)
	{	
		//userTaskGetStackHighWaterMark();
		vTaskDelay(5);
	}
}

void userTaskGetStackHighWaterMark()
{
	UBaseType_t uxHighWaterMark;
	static uint8_t count = 0;
	count ++;
	if(count > 200)
	{
		count = 0;
		uxHighWaterMark = uxTaskGetStackHighWaterMark( USERTask_Handler ); 
		printf("USERTask_Handler:%lu\r\n",uxHighWaterMark); 
		uxHighWaterMark = uxTaskGetStackHighWaterMark( NRFTask_Handler ); 
		printf("NRFTask_Handler:%lu\r\n",uxHighWaterMark); 
		uxHighWaterMark = uxTaskGetStackHighWaterMark( GUITask_Handler ); 
		printf("GUITask_Handler:%lu\r\n",uxHighWaterMark); 
		uxHighWaterMark = uxTaskGetStackHighWaterMark( RC_DATATask_Handler ); 
		printf("RC_DATATask_Handler:%lu\r\n",uxHighWaterMark); 
	//	uxHighWaterMark = uxTaskGetStackHighWaterMark( USERTask_Handler ); 
	//    printf("%lu\r\n",uxHighWaterMark); 
	}
}
