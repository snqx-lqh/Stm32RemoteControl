#include "bsp_timer.h"
#include "bsp_led.h"

void Tim3Main()
{
	//LED = !LED;
}

void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		Tim3Main();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );   //���TIMx�����жϱ�־ 
	}
}

void bsp_tim3_init(uint16_t arr,uint16_t psc)
{
	//ʱ�ӳ�ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	//��ʱ����ʼ��
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		TIM_TimeBaseStructure.TIM_Period    =   arr-1;                      //�����Զ���װ��ֵ	
		TIM_TimeBaseStructure.TIM_Prescaler =   psc-1;                    //����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	}
	//NVIC��ʼ��
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    //��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ����ʹ��
		
		NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	}
	//��ʱ��ʹ���Լ������ж�
	{
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
		TIM_Cmd(TIM3, ENABLE);                    //ʹ��TIMx	
	}
}
