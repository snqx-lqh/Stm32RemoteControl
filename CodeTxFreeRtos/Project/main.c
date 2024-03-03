#include "main.h"

#include "bsp_delay.h"
#include "bsp_usart.h"

#include "bsp_led.h"
#include "bsp_adc.h"
#include "bsp_key.h"

#include "start_task.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	bsp_usart1_init(115200);
	delay_init();

	bsp_led_init();
	bsp_adc_init();
	bsp_key_init();

	startTask();
    vTaskStartScheduler();
	
	while(1)
	{

	}
}


