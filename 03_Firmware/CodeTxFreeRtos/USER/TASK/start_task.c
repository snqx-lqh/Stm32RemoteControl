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

//ң�������ݻ�ȡ�Ļ����ź���
SemaphoreHandle_t rcDataMutexSemaphore;	

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

	//������
	rcDataMutexSemaphore=xSemaphoreCreateMutex();
	//����
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
				
	xTaskCreate((TaskFunction_t)rc_data_task,          //������
                (const char *)"rc_data_task",          //��������
                (uint16_t)RC_DATA_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)RC_DATA_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&RC_DATATask_Handler); //������
	
	#if CONFIG_LETTER_SHELL_ENABLE
	xTaskCreate((TaskFunction_t)shell_task,          //������
                (const char *)"shell_task",          //��������
                (uint16_t)SHELL_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)SHELL_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&SHELLTask_Handler); //������
	#endif

	#if CONFIG_ANO_ENABLE
	xTaskCreate((TaskFunction_t)ano_task,          //������
                (const char *)"ano_task",          //��������
                (uint16_t)ANO_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)ANO_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&ANOTask_Handler); //������
	#endif
				
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
