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

void GuiInit(void);
void GuiControl(void);
void DisplayRefreash(struct Menu_t *nowMenu,u8 selectItem,u8 scrollBar);
void DisplayRefreashData(struct Menu_t *nowMenu,u8 selectItem,u8 scrollBar);
void GuiDataDisplayRefresh(struct Menu_t *menuPoint);

#endif
