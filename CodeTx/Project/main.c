#include "main.h"

#include "bsp_delay.h"
#include "bsp_usart.h"
#include "bsp_sys.h"

#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_timer.h"
#include "bsp_adc.h"
#include "bsp_key.h"

#include "mpu6050.h"
#include "oled.h"
#include "nrf24l01.h"
#include "GUI.h"
#include "keyscan.h"
#include "mytime.h"

int16_t key1_value = 0,key2_value = 0,key3_value = 0,key4_value = 0;
uint16_t lx_value = 0,ly_value = 0,rx_value = 0,ry_value = 0,power_value = 0;
int16_t check_nrf = 0;
uint8_t check_tx = 0;
uint8_t nrf_tx_buff[32] = "HELLO WORLD";
uint8_t nrf_tx_flag       = 0;
uint8_t oled_refresh_flag = 0;
uint8_t led_blink_flag = 0;
uint8_t remote_ctr_flag = 0;

void nrf24l01_tx(void);
void remote_ctrl(void);

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	bsp_usart1_init(115200);
	delay_init();
	bsp_led_init();
	bsp_exti_init();
	bsp_tim3_init(5000,72);
	bsp_adc_init();
	bsp_key_init();
	
	MPU_Init();
	OLED_Init();
	GuiInit();
	NRF24L01_Init();
	check_nrf = NRF24L01_Check();
	NRF24L01_TX_Mode();
	
	while(1)
	{
		if(led_blink_flag == 1)
		{
			led_blink_flag = 0;
			LED = !LED;
		}
		
//		if(nrf_tx_flag == 1)
//		{
//			nrf24l01_tx();
//			nrf_tx_flag = 0;
//		}
		
//		if(remote_ctr_flag == 1)
//		{
//			remote_ctrl();
//			remote_ctr_flag = 0;
//		}
		
		if(oled_refresh_flag == 1)
		{
			count1++;
			count2++;
			count3++;
			count4++;
			GuiControl();
			GuiDataDisplayRefresh();
			oled_refresh_flag = 0;
		}
	}
}

void remote_ctrl()
{
	power_value = get_power_value();
	MPU_Get_Gyro(&gyrox,&gyroy,&gyroz);
	MPU_Get_Acc(&accx,&accy,&accz);
	KeyScan(0);
	MyTimeSecondRun();
	MyTimeFun();
}

void nrf24l01_tx()
{
	lx_value = get_lx_value();
	ly_value = get_ly_value();
	rx_value = get_rx_value();
	ry_value = get_ry_value();

	check_tx = NRF24L01_TxPacket(nrf_tx_buff);
}

void Timer3Function()
{
	static uint8_t count_40ms = 0;
	static uint8_t count_1s = 0;
	static uint8_t count_20ms = 0;
	static uint8_t count_10ms = 0;
	
	count_40ms++;
	if(count_40ms > 7)
	{
		oled_refresh_flag = 1;
		count_40ms = 0;
	}
	
	count_1s++;
	if(count_1s > 9)
	{
		led_blink_flag = 1;
		count_1s = 0;
	}
	
	count_20ms++;
	if(count_20ms > 3)
	{
		nrf_tx_flag = 1;
		count_20ms = 0;
	}
	
	count_10ms++;
	if(count_10ms > 1)
	{
		remote_ctrl();
		//remote_ctr_flag = 1;
		count_10ms = 0;
	}
	
}

void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		Timer3Function();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );   //清除TIMx更新中断标志 
	}
}
