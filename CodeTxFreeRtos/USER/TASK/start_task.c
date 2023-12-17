/**
  ************************************************************************
  * @file       start_task.c/h
  * @brief      �������񣬽�һ������������������Դ�������������ȼ�,
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
    taskENTER_CRITICAL();           //�����ٽ���

    xTaskCreate((TaskFunction_t)user_task,          //������
                (const char *)"user_task",          //��������
                (uint16_t)USER_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)USER_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&USERTask_Handler); //������
	
	xTaskCreate((TaskFunction_t)nrf_task,          //������
                (const char *)"nrf_task",          //��������
                (uint16_t)NRF_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)NRF_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&NRFTask_Handler); //������

	xTaskCreate((TaskFunction_t)gui_task,          //������
                (const char *)"gui_task",          //��������
                (uint16_t)GUI_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)GUI_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&GUITask_Handler); //������
				
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

void startTask(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}
