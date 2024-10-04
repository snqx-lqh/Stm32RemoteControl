/**
  ************************************************************************
  * @file       start_task.c/h
  * @brief      启动任务，将一个个任务开启，分配资源，给定任务优先级,
  ************************************************************************
**/
#include "start_task.h"

#include "user_task.h"
#include "nrf_task.h"
#include "gui_task.h"
#include "rc_data_task.h"

#include "queue.h"
#include "semphr.h"

#define START_TASK_PRIO 1
#define START_STK_SIZE  128
TaskHandle_t    StartTask_Handler;

#define USER_TASK_PRIO 5
#define USER_STK_SIZE  128
TaskHandle_t   USERTask_Handler;

#define NRF_TASK_PRIO  5
#define NRF_STK_SIZE   128
TaskHandle_t   NRFTask_Handler;

#define GUI_TASK_PRIO  5
#define GUI_STK_SIZE   256
TaskHandle_t   GUITask_Handler;

#define RC_DATA_TASK_PRIO 7
#define RC_DATA_STK_SIZE  256
TaskHandle_t      RC_DATATask_Handler;

#if CONFIG_LETTER_SHELL_ENABLE
#include "shell_task.h"
#define SHELL_TASK_PRIO    5
#define SHELL_STK_SIZE     256
TaskHandle_t         SHELLTask_Handler;
#endif

#if CONFIG_ANO_ENABLE
#include "ano_task.h"
#define ANO_TASK_PRIO    5
#define ANO_STK_SIZE     256
TaskHandle_t         ANOTask_Handler;
#endif

//遥控器数据获取的互斥信号量
SemaphoreHandle_t rcDataMutexSemaphore;	

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

	//互斥量
	rcDataMutexSemaphore=xSemaphoreCreateMutex();
	//任务
    xTaskCreate((TaskFunction_t)user_task,          //任务函数
                (const char *)"user_task",          //任务名称
                (uint16_t)USER_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)USER_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&USERTask_Handler); //任务句柄
		
	xTaskCreate((TaskFunction_t)nrf_task,          //任务函数
                (const char *)"nrf_task",          //任务名称
                (uint16_t)NRF_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)NRF_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&NRFTask_Handler); //任务句柄

	xTaskCreate((TaskFunction_t)gui_task,          //任务函数
                (const char *)"gui_task",          //任务名称
                (uint16_t)GUI_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)GUI_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&GUITask_Handler); //任务句柄
				
	xTaskCreate((TaskFunction_t)rc_data_task,          //任务函数
                (const char *)"rc_data_task",          //任务名称
                (uint16_t)RC_DATA_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)RC_DATA_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&RC_DATATask_Handler); //任务句柄
	
	#if CONFIG_LETTER_SHELL_ENABLE
	xTaskCreate((TaskFunction_t)shell_task,          //任务函数
                (const char *)"shell_task",          //任务名称
                (uint16_t)SHELL_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)SHELL_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&SHELLTask_Handler); //任务句柄
	#endif

	#if CONFIG_ANO_ENABLE
	xTaskCreate((TaskFunction_t)ano_task,          //任务函数
                (const char *)"ano_task",          //任务名称
                (uint16_t)ANO_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)ANO_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&ANOTask_Handler); //任务句柄
	#endif
				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

void startTask(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}
