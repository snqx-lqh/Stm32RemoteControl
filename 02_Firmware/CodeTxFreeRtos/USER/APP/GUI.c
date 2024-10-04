/**
 * @file GUI.c
 * @author LQH
 * @brief  ����һ�汾����Щ���޸� 
 * @version 2.0
 * @date 2022-02-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "GUI.h"

#include "ssd1306_oled_driver.h"
#include "ssd1306_oled_middle.h"

struct gui_operations *p_gui_operations;

static int8_t selectItem = 0;//��ǰѡ�в˵����ڵ�ǰ��ʾ�˵�ҳ������
static int8_t scrollBar  = 0;//������
struct Menu_t *menuPoint    ;//��ǰ�˵�

static uint8_t gui_key_value = 0;

#define KeyUp    1
#define KeyDown  2
#define KeyLeft  3
#define KeyRight 4

void gui_operations_register(struct gui_operations *gui_oper)
{
	p_gui_operations = gui_oper;
}

/**
 * @brief �����棬�����Լ�����ͼ��
 * 
 */
void MainUiSet()
{		
	p_gui_operations->userMainSet();
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
void DisplayRefreash(struct Menu_t *nowMenu)
{
	int i = 0;
	OLED_Clear_Buff(&ssd1306_oled_oper);
	gui_key_value = p_gui_operations->guiGetValue();
	GuiKeyControl();
	p_gui_operations->userGuiDataDisplayRefresh(nowMenu);
	if(strcmp((char*)nowMenu->name,p_gui_operations->main_ui_name) == 0)//�ص���ҳ��
	{
		MainUiSet();
	}else 
	{	
		OLED_ShowChar(&ssd1306_oled_oper,0,selectItem*16,     '>',16,1);//������ǰѡ�в˵����ڵ�ǰ��ʾ�˵�ҳ������
		//��ʾ�˵��������
		for(i=0;i<(nowMenu->MenuProperty->MenuLen-nowMenu->MenuProperty->scrollBarLen);i++)
		{
			OLED_ShowString(&ssd1306_oled_oper,8,i*16,nowMenu[i+scrollBar].displayString,16,1);
		}
	}
	OLED_Refresh(&ssd1306_oled_oper);
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
	p_gui_operations->userGuiInit();
}


/**
  * @Name    GuiKeyControl
  * @brief   �������ƣ��������Ƶ�ȷ��������keyscan.c��,����ɨ��ʵ�����Ƿ��ڶ�ʱ�������
  * @param   : [����/��] 
  * @retval  void
  * @author  LQH
  * @Data    2022-01-27
  */
void GuiKeyControl()
{
	if(gui_key_value == KeyUp)//�ϼ�����
	{
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
	}else if(gui_key_value == KeyDown)//���ϼ����
	{
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
	}else if(gui_key_value == KeyLeft)
	{
		//���統ǰ�˵���func1��Ϊ�գ�ִ����غ���
		if(menuPoint[selectItem+scrollBar].func1!=NULL)
		{
			menuPoint[selectItem+scrollBar].func1();
		}
	}else if(gui_key_value == KeyRight)
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
	}
}
