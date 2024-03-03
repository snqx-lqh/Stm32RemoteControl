/**
  ******************************************************************************
  * @file    gui_task.c 
  * @author  ����Ǳ��(snqx-lgh)
  * @version V
  * @date    2024-01-10
  * @brief   oled�˵���ʾ���
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "gui_task.h"

#include "GUI.h"
#include "ssd1306_oled_driver.h"
#include "ssd1306_oled_middle.h"
#include "bmp.h"

#include "semphr.h"
#include "rc_data_task.h"
#include "game_task.h"

extern TaskHandle_t   GUITask_Handler;
extern struct Menu_t *menuPoint;
extern SemaphoreHandle_t rcDataMutexSemaphore;

void userMainSet(void);
void userGuiDataDisplayRefresh(struct Menu_t *menuPoint);

void gui_task(void *pvParameters)
{
	oled1306_middle_init();
	GuiInit();
	while(1)
	{
		GuiControl();
		userGuiDataDisplayRefresh(menuPoint);
		vTaskDelay(20);
	}
}

//��UI
struct MenuProperty_t MainUIProperty={1,0};
struct Menu_t MainUI=
{&MainUIProperty,"MainUI","MainUI" ,NULL,NULL,NULL};

//���˵�
struct MenuProperty_t menuMainProperty={4,0};
struct Menu_t menuMain[4]=
{
	{&menuMainProperty,"menuMain","last menu     ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"menuMain","adc value     ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"menuMain","state         ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"menuMain","game          ", NULL,NULL, &MainUI,NULL}
};
//adc value���Ӳ˵�
struct MenuProperty_t setMenu1Property={6,2};
struct Menu_t setMenu1[6]=
{
	{&setMenu1Property,"setMenu1","last menu     ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"setMenu1","lx            ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"setMenu1","ly            ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"setMenu1","rx            ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"setMenu1","ry            ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"setMenu1","power         ",NULL,NULL,menuMain,NULL}
};
//state���Ӳ˵�
struct MenuProperty_t setMenu2Property={4,0};
struct Menu_t setMenu2[4]=
{
	{&setMenu2Property,"setMenu2","last menu      ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"setMenu2","anglex         ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"setMenu2","angley         ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"setMenu2","anglez         ",NULL, NULL, menuMain,NULL}
};

void game1_create(void);
void game2_create(void);

//game���Ӳ˵�
struct MenuProperty_t setMenu3Property={4,0};
struct Menu_t setMenu3[4]=
{
	{&setMenu3Property,"setMenu2","last menu      ",NULL, NULL, menuMain,NULL},
	{&setMenu3Property,"setMenu2","ball game      ",NULL, game1_create, menuMain,NULL},
	{&setMenu3Property,"setMenu2","snake game     ",NULL, game2_create, menuMain,NULL},
	{&setMenu3Property,"setMenu2","game3          ",NULL, NULL, menuMain,NULL}
};


static TaskHandle_t         GAME1Task_Handler;
void game1_create()
{
	xTaskCreate((TaskFunction_t)game_task,          //������
                (const char *)"game_task",          //��������
                (uint16_t)256,            //�����ջ��С
                (void *)((uint16_t)1),                        //���ݸ��������Ĳ���
                (UBaseType_t)6,        //�������ȼ�
                (TaskHandle_t *)&GAME1Task_Handler); //������
	vTaskSuspend(GUITask_Handler);
}

static TaskHandle_t         GAME2Task_Handler;
void game2_create()
{
	xTaskCreate((TaskFunction_t)game_task,          //������
                (const char *)"game_task",          //��������
                (uint16_t)256,            //�����ջ��С
                (void *)((uint16_t)2),                        //���ݸ��������Ĳ���
                (UBaseType_t)6,        //�������ȼ�
                (TaskHandle_t *)&GAME2Task_Handler); //������
	vTaskSuspend(GUITask_Handler);
}

void userGuiInit()
{
	MainUI.childrenMenu = menuMain;
	menuMain[1].childrenMenu = setMenu1;
	menuMain[2].childrenMenu = setMenu2;
	menuMain[3].childrenMenu = setMenu3;
	menuPoint = setMenu3;
}

void userMainSet( )
{
	OLED_ShowPicture(29,0,65,60,(uint8_t *)BMP7,1);
}

void userGuiDataDisplayRefresh(struct Menu_t *menuPoint)
{
	rc_data_t rc_data_temp;
	// �ȴ���������ȷ����д�빲������ʱ���ᱻ���������ж�
	if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
		// ���¹�������
		rc_data_temp = rc_data;
		// �ͷŻ�����
		xSemaphoreGive(rcDataMutexSemaphore);
	}
	if(menuPoint == setMenu1)
	{
		sprintf((char*)setMenu1[1].displayString,"lx    %4u     ",rc_data_temp.lx_value);
		sprintf((char*)setMenu1[2].displayString,"ly    %4u     ",rc_data_temp.ly_value);
		sprintf((char*)setMenu1[3].displayString,"rx    %4u     ",rc_data_temp.rx_value);
		sprintf((char*)setMenu1[4].displayString,"ry    %4u     ",rc_data_temp.ry_value);
		sprintf((char*)setMenu1[5].displayString,"power %4u     ",rc_data_temp.power_value);
	}
	else if(menuPoint==setMenu2)
	{
		sprintf((char*)setMenu2[1].displayString,"anglex %2.2f   ",rc_data_temp.angle[0]);
		sprintf((char*)setMenu2[2].displayString,"angley %2.2f   ",rc_data_temp.angle[1]);
		sprintf((char*)setMenu2[3].displayString,"anglez %2.2f   ",rc_data_temp.angle[2]);
	}
	
}
