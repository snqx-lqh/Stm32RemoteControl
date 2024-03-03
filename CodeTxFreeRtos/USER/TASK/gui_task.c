/**
  ******************************************************************************
  * @file    gui_task.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-10
  * @brief   oled菜单显示相关
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

//主UI
struct MenuProperty_t MainUIProperty={1,0};
struct Menu_t MainUI=
{&MainUIProperty,"MainUI","MainUI" ,NULL,NULL,NULL};

//主菜单
struct MenuProperty_t menuMainProperty={4,0};
struct Menu_t menuMain[4]=
{
	{&menuMainProperty,"menuMain","last menu     ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"menuMain","adc value     ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"menuMain","state         ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"menuMain","game          ", NULL,NULL, &MainUI,NULL}
};
//adc value的子菜单
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
//state的子菜单
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

//game的子菜单
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
	xTaskCreate((TaskFunction_t)game_task,          //任务函数
                (const char *)"game_task",          //任务名称
                (uint16_t)256,            //任务堆栈大小
                (void *)((uint16_t)1),                        //传递给任务函数的参数
                (UBaseType_t)6,        //任务优先级
                (TaskHandle_t *)&GAME1Task_Handler); //任务句柄
	vTaskSuspend(GUITask_Handler);
}

static TaskHandle_t         GAME2Task_Handler;
void game2_create()
{
	xTaskCreate((TaskFunction_t)game_task,          //任务函数
                (const char *)"game_task",          //任务名称
                (uint16_t)256,            //任务堆栈大小
                (void *)((uint16_t)2),                        //传递给任务函数的参数
                (UBaseType_t)6,        //任务优先级
                (TaskHandle_t *)&GAME2Task_Handler); //任务句柄
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
	// 等待互斥量，确保在写入共享数据时不会被其他任务中断
	if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
		// 更新共享数据
		rc_data_temp = rc_data;
		// 释放互斥量
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
