/**
 * @file GUI.c
 * @author LQH
 * @brief  ʹ�ñ���ʱ��ע���ʼ���ַ���һ��Ҫռ��15���ַ�����Ȼ�����Ὣ�ϴε��ַ����� 
 * @version 1.0
 * @date 2022-02-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "GUI.h"

#include "ssd1306_oled_driver.h"
#include "ssd1306_oled_middle.h"

#include "keyscan.h"

//�û������Զ���
extern void userGuiInit(void);
extern void userMainSet(void);
extern void userGuiDataDisplayRefresh(struct Menu_t *menuPoint);

#define  uMainUi                  "MainUI"
#define  uGuiInit                 userGuiInit 
#define  uMainSet                 userMainSet                   
#define  uGuiDataDisplayRefresh   userGuiDataDisplayRefresh

int8_t selectItem = 0;//��ǰ�˵��ڵ�ǰ�˵�ҳ�����������Ϊ3��һҳ�����ʾ�ĸ��˵�
int8_t scrollBar  = 0;//������
struct Menu_t *menuPoint;//��ǰ�˵�

/**
 * @brief �����棬�����Լ�����ͼ��
 * 
 */
void MainUiSet()
{		
	uMainSet();
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
	if(strcmp((char*)nowMenu->name,uMainUi) == 0)//���ص����˵�ʱ������û��ȫռ��������ȫ���������ٻ�
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
	uGuiInit();
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
void GuiDataDisplayRefresh(struct Menu_t *menuPoint)
{
	userGuiDataDisplayRefresh(menuPoint);
	if(strcmp((char*)menuPoint->name,uMainUi) == 0)
	{
		MainUiSet();
		OLED_Refresh();
	}else
	{
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
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
	GuiDataDisplayRefresh(menuPoint);
}
