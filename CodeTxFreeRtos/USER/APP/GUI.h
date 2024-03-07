#ifndef _GUI_H
#define _GUI_H

#include "main.h"

struct MenuProperty_t
{
	u8 MenuLen;//��ǰ�˵�ҳ�˵�����
	u8 scrollBarLen;//����������,���ڶ����õ�16SIZE���ַ�������һ���˵�ҳ����ĸ��˵�������˵���������Ϊ1
};

//�˵��ṹ��
struct Menu_t{
	struct MenuProperty_t *MenuProperty;
	u8 name[15];//��ǰ�˵�������
	u8 displayString[15];//��ǰ�˵����ַ�
	void (*func1) (void);//��ǰ�˵��Ĺ��ܺ���
	void (*func2) (void);//��ǰ�˵��Ĺ��ܺ���
	struct Menu_t *fatherMenu;//��ǰ�˵��ĸ����˵�
	struct Menu_t *childrenMenu;//��ǰ�˵����Ӽ��˵�	
};

struct gui_operations {
	void (*userGuiInit)(void);
	void (*userMainSet)(void);
	void (*userGuiDataDisplayRefresh)(struct Menu_t *menuPoint);
	char *main_ui_name;
	int  (*guiGetValue)(void);
};

void gui_operations_register(struct gui_operations *gui_oper);
void GuiInit(void);
void GuiKeyControl(void);
void DisplayRefreash(struct Menu_t *nowMenu);

#endif
