#include "bsp_led.h"

void bsp_led_init()
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//��ʼ��IO��
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_13;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOC,&GPIO_InitStruct);
	}
}

void led_on()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void led_off()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}
