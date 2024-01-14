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
#include "oled.h"
#include "bmp.h"

#include "semphr.h"
#include "rc_data_task.h"

extern struct Menu_t *menuPoint;
extern SemaphoreHandle_t rcDataMutexSemaphore;

void userMainSet();
void userGuiDataDisplayRefresh(struct Menu_t *menuPoint);

void gui_task(void *pvParameters)
{
	OLED_Init();
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
	{&menuMainProperty,"menuMain","              ", NULL,NULL, &MainUI,NULL}
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
	{&setMenu2Property,"setMenu2","               ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"setMenu2","               ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"setMenu2","               ",NULL, NULL, menuMain,NULL}
};

void userGuiInit()
{
	MainUI.childrenMenu = menuMain;
	menuMain[1].childrenMenu = setMenu1;
	menuMain[2].childrenMenu = setMenu2;
	menuPoint = &MainUI;
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
		
	}
	
}