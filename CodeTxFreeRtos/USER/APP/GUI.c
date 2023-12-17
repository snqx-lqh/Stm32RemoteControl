/**
 * @file GUI.c
 * @author LQH
 * @brief  ʹ�ñ���ʱ��ע���ʼ���ַ���һ��Ҫռ��15���ַ�����Ȼ�����Ὣ�ϴε��ַ����� 
 * @version 0.1
 * @date 2022-02-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "GUI.h"
#include "oled.h"
#include "bmp.h"

#include "keyscan.h"
#include "mytime.h"

int8_t selectItem = 0;//��ǰ�˵��ڵ�ǰ�˵�ҳ�����������Ϊ3��һҳ�����ʾ�ĸ��˵�
int8_t scrollBar  = 0;//������
struct Menu_t *menuPoint;//��ǰ�˵�

//���������Ե�����
int8_t kp = 0;
int8_t ki = 0;
int8_t kd = 0;
int16_t count_test=0;
uint16_t count1=0,count2=2,count3=4,count4=6,count5=8;

//һЩ�˵��Ĺ��ܺ���
void KpSub(void);
void KiAdd(void);
void KdAdd(void);
void KpAdd(void);
void HourAdd(void);
void MinuteAdd(void);
void SecondAdd(void);
void YearAdd(void);
void MonthAdd(void);
void DayAdd(void);
void TimeOK(void);
void TimeSetInit(void);

//��UI
struct MenuProperty_t MainUIProperty={1,0};
struct Menu_t MainUI=
{&MainUIProperty,"MainUI" ,NULL,NULL,NULL};

//���˵�
struct MenuProperty_t menuMainProperty={4,0};
struct Menu_t menuMain[4]=
{
	{&menuMainProperty,"last menu     ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"Animal        ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"Pid           ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"Time set      ", NULL,TimeSetInit, &MainUI,NULL}
};
//animal���Ӳ˵�
struct MenuProperty_t setMenu1Property={6,2};
struct Menu_t setMenu1[6]=
{
	{&setMenu1Property,"last menu     ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"bull          ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"bird          ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"dog           ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"bow           ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"fish          ",NULL,NULL,menuMain,NULL}
};
//Pid���Ӳ˵�
struct MenuProperty_t setMenu2Property={5,1};
struct Menu_t setMenu2[5]=
{
	{&setMenu2Property,"last menu      ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"KP   0         ",KpSub,KpAdd,menuMain,NULL},
	{&setMenu2Property,"KI   0         ",NULL, KiAdd,menuMain,NULL},
	{&setMenu2Property,"KD   0         ",NULL, KdAdd,menuMain,NULL},
	{&setMenu2Property,"PWM  0         ",NULL, NULL, menuMain,NULL},
};
//Time set���Ӳ˵�
struct MenuProperty_t setMenu3Property={8,4};
struct Menu_t setMenu3[8]=
{
	{&setMenu3Property,"last menu      ",NULL, NULL,menuMain,NULL},
	{&setMenu3Property,"Hour   0       ",NULL, HourAdd,menuMain,NULL},
	{&setMenu3Property,"Minute 0       ",NULL, MinuteAdd,menuMain,NULL},
	{&setMenu3Property,"Second 0       ",NULL, SecondAdd,menuMain,NULL},
	{&setMenu3Property,"Year   0       ",NULL, YearAdd,menuMain,NULL},
	{&setMenu3Property,"Month  0       ",NULL, MonthAdd,menuMain,NULL},
	{&setMenu3Property,"Day    0       ",NULL, DayAdd,menuMain,NULL},
	{&setMenu3Property,"OK             ",NULL, TimeOK,menuMain,NULL},
};

void KpSub()
{
	kp--;
}

void KpAdd()
{
	kp++;
}

void KiAdd()
{
	ki++;
}

void KdAdd()
{
	kd++;
}

void HourAdd(void)
{
	myTimeTemp.Hour++;
	if(myTimeTemp.Hour>23)
		myTimeTemp.Hour=0;	
}
void MinuteAdd(void)
{
	myTimeTemp.Minute++;
	if(myTimeTemp.Minute>59)
		myTimeTemp.Minute=0;	
}
void SecondAdd(void)
{
	myTimeTemp.Second++;
	if(myTimeTemp.Second>59)
		myTimeTemp.Second=0;
}
void YearAdd(void)
{
	myTimeTemp.Year++;
}
void MonthAdd(void)
{
	myTimeTemp.Month++;
	if(myTimeTemp.Month>12)
		myTimeTemp.Month=1;
}
void DayAdd(void)
{
	myTimeTemp.Day++;
	if(myTimeTemp.Day>30)
		myTimeTemp.Day=1;
}
void TimeOK(void)
{
	myTime = myTimeTemp;
}

void TimeSetInit(void)
{
	myTimeTemp = myTime;
}
/**
 * @brief �����棬�����Լ�����ͼ��,�����ﻭ�������ߺ�һ��ʱ��
 * 
 */
void MainUiSet()
{
	int i;
	u8 time_x = 18,time_y = 12,date_x=26,date_y=40;
	for(i=1;i<15;i++)
	{
		OLED_ShowChar(8*i,0, '-',16,1);
		OLED_ShowChar(8*i,48,'-',16,1);
	}	

	OLED_ShowNum(time_x,    time_y,myTime.Hour  ,2,24,1);
	OLED_ShowChar(time_x+24,time_y,':'   ,24,1);
	OLED_ShowNum(time_x+36, time_y,myTime.Minute,2,24,1);
	OLED_ShowChar(time_x+60,time_y,':'   ,24,1);
	OLED_ShowNum(time_x+72, time_y,myTime.Second,2,24,1);
	
	OLED_ShowNum(date_x,    date_y,myTime.Year  ,4,16,1);
	OLED_ShowChar(date_x+32,date_y,'/'   ,16,1);
	OLED_ShowNum(date_x+40, date_y,myTime.Month,2,16,1);
	OLED_ShowChar(date_x+56,date_y,'/'   ,16,1);
	OLED_ShowNum(date_x+64, date_y,myTime.Day,  2,16,1);
}

/**
  * @Name    DisplayRefreash
  * @brief   ˢ�µ�ǰ�˵�ҳ�����в˵���
  * @param   nowMenu: ��ǰ�˵�ָ��
**			 selectItem: ��ǰ�˵��ڵ�ǰ�˵�ҳ��������ָС����ڵڼ���
**           scrollBar������������λ�� 
  * @retval  void
  * @author  LQH
  * @Data    2022-01-27
  */
void DisplayRefreash(struct Menu_t *nowMenu,u8 selectItem,u8 scrollBar)
{
	int i = 0;
	static u8 lastSelectItem=0;//��¼�ϴ�����
	if(nowMenu==&MainUI)//���ص����˵�ʱ������û��ȫռ��������ȫ���������ٻ�
	{
		OLED_Clear();
		MainUiSet();
	}else 
	{	
		OLED_ShowChar(0,lastSelectItem*16, ' ',16,1);//����ϴ�����
		OLED_ShowChar(0,selectItem*16,     '>',16,1);//�����������
		for(i=0;i<(nowMenu->MenuProperty->MenuLen-nowMenu->MenuProperty->scrollBarLen);i++)
		{
			OLED_ShowString(8,i*16,nowMenu[i+scrollBar].displayString,16,1);
		}
	}
	OLED_Refresh();
	lastSelectItem = selectItem;
}
/**
  * @Name    DisplayRefreashDate
  * @brief   ˢ�µ�ǰ�˵�ҳ�����в˵���,��Ҫ������������
  * @param   nowMenu:    ��ǰ�˵�ָ��
**			 selectItem: ��ǰ�˵��ڵ�ǰ�˵�ҳ��������ָС����ڵڼ���
**           scrollBar�� ����������λ�� 
  * @retval  void
  * @author  LQH
  * @Data    2022-01-27
  */
void DisplayRefreashData(struct Menu_t *nowMenu,u8 selectItem,u8 scrollBar)
{
	int i = 0;
	for(i=0;i<(nowMenu->MenuProperty->MenuLen-nowMenu->MenuProperty->scrollBarLen);i++)
	{
		OLED_ShowString(8,i*16,nowMenu[i+scrollBar].displayString,16,1);
	}
	OLED_Refresh();
}

/**
  * @name    GuiInit
  * @brief   һЩ�˵���ʼ����Ҫ�����˵�ҳ���Ӳ˵�ȷ�������ڸ��˵��ṹ���ʼ��ʱ���ܳ�ʼ���Ӳ˵�
  * 		 �����������ʼ��
  * @param   : 
  * @retval  void
  * @author  LQH
  * @date    2022-01-27
  */
void GuiInit()
{
	MainUI.childrenMenu = menuMain;
	menuMain[1].childrenMenu = setMenu1;
	menuMain[2].childrenMenu = setMenu2;
	menuMain[3].childrenMenu = setMenu3;
	menuPoint = &MainUI;
	DisplayRefreash(menuPoint,selectItem,scrollBar);
}
/**
  * @name    GuiDataDisplayRefresh
  * @brief   �˵�ҳ�����ݸ��£��ǵ�һ��Ҫ�ַ���ռ��15���ַ�
  * @param   : 
  * @retval  void
  * @author  LQH
  * @date    2022-01-27
  */
void GuiDataDisplayRefresh()
{
	if(menuPoint == setMenu1)
	{
		sprintf((char*)setMenu1[1].displayString,"bull  %3d     ",count1);
		sprintf((char*)setMenu1[2].displayString,"bird  %3d     ",count2);
		sprintf((char*)setMenu1[3].displayString,"dog   %3d     ",count3);
		sprintf((char*)setMenu1[4].displayString,"bow   %3d     ",count4);
		sprintf((char*)setMenu1[5].displayString,"fish  %3d     ",count5);
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
	}
	else if(menuPoint==setMenu2)
	{
		sprintf((char*)setMenu2[1].displayString,"KP   %3d      ",kp);
		sprintf((char*)setMenu2[2].displayString,"KI   %3d      ",ki);
		sprintf((char*)setMenu2[3].displayString,"KD   %3d      ",kd);
		sprintf((char*)setMenu2[4].displayString,"PWM  %3d      ",0);
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
	}
	else if(menuPoint==setMenu3)
	{
		sprintf((char*)setMenu3[1].displayString,"Hour   %2d    ",myTimeTemp.Hour);	
		sprintf((char*)setMenu3[2].displayString,"Minute %2d    ",myTimeTemp.Minute);	
		sprintf((char*)setMenu3[3].displayString,"Second %2d    ",myTimeTemp.Second);	
		sprintf((char*)setMenu3[4].displayString,"Year   %4d    ",myTimeTemp.Year);
		sprintf((char*)setMenu3[5].displayString,"Month  %2d    ",myTimeTemp.Month);
		sprintf((char*)setMenu3[6].displayString,"Day    %2d    ",myTimeTemp.Day);
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
	}
	else if(menuPoint==&MainUI)
	{
		MainUiSet();
		OLED_Refresh();
	}
}
/**
  * @Name    GuiControl
  * @brief   �������ƣ��������Ƶ�ȷ��������keyscan.c��,����ɨ��ʵ�����Ƿ��ڶ�ʱ�������
  * @param   : [����/��] 
  * @retval  void
  * @author  LQH
  * @Data    2022-01-27
  */
void GuiControl()
{
	if(isKeyUp==1)//�ϼ�����
	{
		isKeyUp=0;//��־λ����
		selectItem--;//��ǰ�˵��ڵ�ǰ�˵�ҳ������--
		if(selectItem<0&&scrollBar!=0)//С��0,���ǹ���������0���ͼ�������
		{
			selectItem = 0;
			scrollBar--;
		}else if(selectItem<0&&scrollBar==0)//С��0,������Ҳ��0���ͽ������Ƶ���󣬹����������
		{
			selectItem = menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen;
			scrollBar  = menuPoint->MenuProperty->scrollBarLen;
		}
		DisplayRefreash(menuPoint,selectItem,scrollBar);//ˢ����ʾ
	}else if(isKeyDown==1)//���ϼ����
	{
		isKeyDown=0;
		selectItem++;
		//���������������ֵ�����ǹ������������ֵ�������������ֵ��������++
		if(selectItem>(menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen)&&scrollBar!=menuPoint->MenuProperty->scrollBarLen)
		{
			selectItem = menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen;
			scrollBar++;
			
		}
		//���������������ֵ�������������ֵ���ƶ�����һ��λ��
		else if(selectItem>(menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen)&&scrollBar==menuPoint->MenuProperty->scrollBarLen)
		{
			selectItem=0;
			scrollBar =0;
		}
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}else if(isKeyLeft==1)
	{
		//���統ǰ�˵���func1��Ϊ�գ�ִ����غ���
		if(menuPoint[selectItem+scrollBar].func1!=NULL)
		{
			menuPoint[selectItem+scrollBar].func1();
		}
		isKeyLeft=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}else if(isKeyRight==1)
	{
		if(selectItem==0 && scrollBar==0 && menuPoint[selectItem].fatherMenu!=NULL)//��������Ϊ����Ҹ��˵���Ϊ�գ�ָ��ָ��
		{
			menuPoint = menuPoint[selectItem].fatherMenu;
		}
		else if(menuPoint[selectItem+scrollBar].childrenMenu!=NULL)//����������Ӳ˵�ҳ��Ϊ�գ�ָ���Ӳ˵�
		{
			if(menuPoint[selectItem+scrollBar].func2!=NULL)//���統ǰ�˵���func2��Ϊ�գ�ִ����غ���
			{
				menuPoint[selectItem+scrollBar].func2();
			}
			menuPoint = menuPoint[selectItem+scrollBar].childrenMenu;
			selectItem = 0;
		}
		else if(menuPoint[selectItem+scrollBar].func2!=NULL)//���統ǰ�˵���func2��Ϊ�գ�ִ����غ���
		{
			menuPoint[selectItem+scrollBar].func2();
		}
		isKeyRight=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}
	GuiDataDisplayRefresh();
}
