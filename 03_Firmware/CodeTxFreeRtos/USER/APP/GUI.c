/**
 * @file GUI.c
 * @author LQH
 * @brief  在上一版本做了些许修改 
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

static int8_t selectItem = 0;//当前选中菜单项在当前显示菜单页的索引
static int8_t scrollBar  = 0;//滚动条
struct Menu_t *menuPoint    ;//当前菜单

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
 * @brief 主界面，可以自己设置图画
 * 
 */
void MainUiSet()
{		
	p_gui_operations->userMainSet();
}

/**
  * @Name    DisplayRefreash
  * @brief   刷新当前菜单页的所有菜单项
  * @param   nowMenu: 当前菜单指针
**			 selectItem: 当前菜单在当前菜单页的索引，指小光标在第几行
**           scrollBar：滚动条所在位置 
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
	if(strcmp((char*)nowMenu->name,p_gui_operations->main_ui_name) == 0)//回到主页面
	{
		MainUiSet();
	}else 
	{	
		OLED_ShowChar(&ssd1306_oled_oper,0,selectItem*16,     '>',16,1);//画出当前选中菜单项在当前显示菜单页的索引
		//显示菜单项的内容
		for(i=0;i<(nowMenu->MenuProperty->MenuLen-nowMenu->MenuProperty->scrollBarLen);i++)
		{
			OLED_ShowString(&ssd1306_oled_oper,8,i*16,nowMenu[i+scrollBar].displayString,16,1);
		}
	}
	OLED_Refresh(&ssd1306_oled_oper);
}

/**
  * @name    GuiInit
  * @brief   一些菜单初始化主要是主菜单页的子菜单确定，由于父菜单结构体初始化时不能初始化子菜单
  * 		 所以在这里初始化
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
  * @brief   按键控制，按键控制的确定变量在keyscan.c中,按键扫描实现又是放在定时器里面的
  * @param   : [输入/出] 
  * @retval  void
  * @author  LQH
  * @Data    2022-01-27
  */
void GuiKeyControl()
{
	if(gui_key_value == KeyUp)//上键按下
	{
		selectItem--;//当前菜单在当前菜单页的索引--
		if(selectItem<0&&scrollBar!=0)//小于0,但是滚动条不在0，就减滚动条
		{
			selectItem = 0;
			scrollBar--;
		}else if(selectItem<0&&scrollBar==0)//小于0,滚动条也在0，就将索引移到最后，滚动条到最大
		{
			selectItem = menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen;
			scrollBar  = menuPoint->MenuProperty->scrollBarLen;
		}
	}else if(gui_key_value == KeyDown)//和上键差不多
	{
		selectItem++;
		//假如索引大于最大值，但是滚动条不在最大值，保持索引最大值，滚动条++
		if(selectItem>(menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen)&&scrollBar!=menuPoint->MenuProperty->scrollBarLen)
		{
			selectItem = menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen;
			scrollBar++;
		}
		//假如索引大于最大值，滚动条在最大值，移动到第一个位置
		else if(selectItem>(menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen)&&scrollBar==menuPoint->MenuProperty->scrollBarLen)
		{
			selectItem=0;
			scrollBar =0;
		}
	}else if(gui_key_value == KeyLeft)
	{
		//假如当前菜单的func1不为空，执行相关函数
		if(menuPoint[selectItem+scrollBar].func1!=NULL)
		{
			menuPoint[selectItem+scrollBar].func1();
		}
	}else if(gui_key_value == KeyRight)
	{
		if(selectItem==0 && scrollBar==0 && menuPoint[selectItem].fatherMenu!=NULL)//假如索引为零而且父菜单不为空，指向父指针
		{
			menuPoint = menuPoint[selectItem].fatherMenu;
		}
		else if(menuPoint[selectItem+scrollBar].childrenMenu!=NULL)//假如该索引子菜单页不为空，指向子菜单
		{
			if(menuPoint[selectItem+scrollBar].func2!=NULL)//假如当前菜单的func2不为空，执行相关函数
			{
				menuPoint[selectItem+scrollBar].func2();
			}
			menuPoint = menuPoint[selectItem+scrollBar].childrenMenu;
			selectItem = 0;
		}
		else if(menuPoint[selectItem+scrollBar].func2!=NULL)//假如当前菜单的func2不为空，执行相关函数
		{
			menuPoint[selectItem+scrollBar].func2();
		}
	}
}
