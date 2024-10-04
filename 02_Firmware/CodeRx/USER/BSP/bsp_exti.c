#include "bsp_exti.h"

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!= RESET) 
	{
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void bsp_exti_init()
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//���ų�ʼ��
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	}
	//�ж��߳�ʼ��
	{
		EXTI_InitTypeDef EXTI_InitStructure;
		
		EXTI_InitStructure.EXTI_Line    = EXTI_Line4;	
		EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		
		EXTI_Init(&EXTI_InitStructure);
		
		//��ʼ���ж���
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	}
	//NVIC��ʼ��
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ���ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =        0;	//�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
		
		NVIC_Init(&NVIC_InitStructure); 
	}
}
