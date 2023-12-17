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

#define START_TASK_PRIO 1
#define START_STK_SIZE  128
static  TaskHandle_t    StartTask_Handler;

#define USER_TASK_PRIO 8
#define USER_STK_SIZE  512
static  TaskHandle_t   USERTask_Handler;

#define NRF_TASK_PRIO 6
#define NRF_STK_SIZE  512
static  TaskHandle_t   NRFTask_Handler;

#define GUI_TASK_PRIO 5
#define GUI_STK_SIZE  512
static  TaskHandle_t   GUITask_Handler;

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

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
