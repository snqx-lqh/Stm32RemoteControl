#include "bsp_delay.h"



//���ʹ��OS,����������ͷ�ļ�����
#if SYSTEM_SUPPORT_OS == 0

static u8  fac_us=0;							//us��ʱ������
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
    fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8
    fac_ms=(u16)fac_us*1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����
}

void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD=nus*fac_us; 					//ʱ�����
    SysTick->VAL=0x00;        					//��ռ�����
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����
    do
    {
        temp=SysTick->CTRL;
    } while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL =0X00;      					 //��ռ�����
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL =0x00;							//��ռ�����
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����
    do
    {
        temp=SysTick->CTRL;
    } while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
    SysTick->VAL =0X00;       					//��ռ�����
}

#else

#include "FreeRTOS.h"					//FreeRTOSʹ��		  
#include "task.h"

static uint8_t  fac_us=0;							//us��ʱ������			   
static uint16_t fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��

extern void xPortSysTickHandler(void);
 
//systick�жϷ�����,ʹ��OSʱ�õ�
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
}
			   
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӣ�������������SYSTICKʱ��Ƶ��ΪAHB/8
//����Ϊ�˼���FreeRTOS�����Խ�SYSTICK��ʱ��Ƶ�ʸ�ΪAHB��Ƶ�ʣ�
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init()
{
	uint32_t reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=72;							//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=72;							//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/configTICK_RATE_HZ;		//����delay_ostickspersec�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.0998s����	
	fac_ms=1000/configTICK_RATE_HZ;			//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/configTICK_RATE_HZ��һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK     
}								    

//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=168)	    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};										    
}  
//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(uint16_t nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	{		
		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS��ʱ
		}
		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((uint32_t)(nms*1000));				//��ͨ��ʽ��ʱ
}

//��ʱnms,���������������
//nms:Ҫ��ʱ��ms��
void delay_xms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

#endif

