#include "main.h"

#include "bsp_delay.h"
#include "bsp_usart.h"
#include "bsp_sys.h"

#include "nrf24l01.h"

int16_t check_nrf = 0;
uint8_t check_rx = 0;
uint8_t nrf_test[32] = {0};

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	bsp_usart1_init(115200);
	delay_init();
	
	NRF24L01_Init();
	check_nrf = NRF24L01_Check();
	NRF24L01_RX_Mode();
	
	while(1)
	{
		if(NRF24L01_RxPacket(nrf_test)==0)//一旦接收到信息,则显示出来.
		{
			USART1_Send_Len_Data(nrf_test,32);
		}
		delay_ms(5);
	}
}

