/**
 * @file GUI.c
 * @author LQH
 * @brief  使用本库时，注意初始化字符串一定要占满15个字符，不然，不会将上次的字符覆盖 
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

//用户函数自定义
extern void userGuiInit(void);
extern void userMainSet(void);
extern void userGuiDataDisplayRefresh(struct Menu_t *menuPoint);

#define  uMainUi                  "MainUI"
#define  uGuiInit                 userGuiInit 
#define  uMainSet                 userMainSet                   
#define  uGuiDataDisplayRefresh   userGuiDataDisplayRefresh

int8_t selectItem = 0;//当前菜单在当前菜单页的索引，最大为3，一页最多显示四个菜单
int8_t scrollBar  = 0;//滚动条
struct Menu_t *menuPoint;//当前菜单

/**
 * @brief 主界面，可以自己设置图画
 * 
 */
void MainUiSet()
{		
	uMainSet();
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
void DisplayRefreash(struct Menu_t *nowMenu,u8 selectItem,u8 scrollBar)
{
	int i = 0;
	static u8 lastSelectItem=0;//记录上次索引
	if(strcmp((char*)nowMenu->name,uMainUi) == 0)//当回到主菜单时，由于没有全占屏，所以全部清屏，再画
	{
		OLED_Clear();
		MainUiSet();
	}else 
	{	
		OLED_ShowChar(0,lastSelectItem*16, ' ',16,1);//清除上次索引
		OLED_ShowChar(0,selectItem*16,     '>',16,1);//画出这次索引
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
  * @brief   刷新当前菜单页的所有菜单项,主要拿来更新数据
  * @param   nowMenu:    当前菜单指针
**			 selectItem: 当前菜单在当前菜单页的索引，指小光标在第几行
**           scrollBar： 滚动条所在位置 
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
  * @brief   一些菜单初始化主要是主菜单页的子菜单确定，由于父菜单结构体初始化时不能初始化子菜单
  * 		 所以在这里初始化
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
  * @brief   菜单页的数据更新，记得一定要字符串占满15个字符
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
  * @brief   按键控制，按键控制的确定变量在keyscan.c中,按键扫描实现又是放在定时器里面的
  * @param   : [输入/出] 
  * @retval  void
  * @author  LQH
  * @Data    2022-01-27
  */
void GuiControl()
{
	if(isKeyUp==1)//上键按下
	{
		isKeyUp=0;//标志位清零
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
		DisplayRefreash(menuPoint,selectItem,scrollBar);//刷新显示
	}else if(isKeyDown==1)//和上键差不多
	{
		isKeyDown=0;
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
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}else if(isKeyLeft==1)
	{
		//假如当前菜单的func1不为空，执行相关函数
		if(menuPoint[selectItem+scrollBar].func1!=NULL)
		{
			menuPoint[selectItem+scrollBar].func1();
		}
		isKeyLeft=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}else if(isKeyRight==1)
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
		isKeyRight=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}
	GuiDataDisplayRefresh(menuPoint);
}
