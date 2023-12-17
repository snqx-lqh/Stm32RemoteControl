#include "user_task.h"

#include "user_task.h"

#include "nrf24l01.h"
#include "bsp_adc.h"
#include "mpu6050.h"
#include "bsp_key.h"

int16_t key1_value = 0,key2_value = 0,key3_value = 0,key4_value = 0;
uint16_t lx_value = 0,ly_value = 0,rx_value = 0,ry_value = 0,power_value = 0;
int16_t check_nrf = 0;
uint8_t check_tx = 0;
uint8_t nrf_tx_buff[32] = {0};
uint8_t nrf_rx_buff[32] = {0};
uint8_t key_value = 0;

void nrf24l01_tx(void);

void nrf_task(void *pvParameters)
{
	check_nrf = NRF24L01_Check();
	//NRF24L01_RX_Mode();
	NRF24L01_TX_Mode();
	while(1)
	{
		nrf24l01_tx();
//		if(NRF24L01_RxPacket(nrf_rx_buff)==0)//一旦接收到信息,则发送出来.
//	 	{
//	 		//HAL_UART_Transmit(&huart1,nrf_test,sizeof(nrf_test),100);
//	 	}
		vTaskDelay(20);
	}
}

typedef union {
	uint8_t uchar_data[4];
	float   float_data;
}uchar_to_float_t;

void nrf24l01_tx()
{
	uchar_to_float_t uchar_to_float;
	
	lx_value = get_lx_value();
	ly_value = get_ly_value();
	rx_value = get_rx_value();
	ry_value = get_ry_value();

	key_value = (KEY_UP << 7)|(KEY_DOWN << 6)|(KEY_LEFT << 5)|(KEY_RIGHT << 4)| \
				(KEY_YG_L << 3)|(KEY_YG_R << 2)|(KEY_BM_L << 1)|(KEY_BM_R << 0);
	
	nrf_tx_buff[0] = 0xA3;
	
	//遥控器通道值
	nrf_tx_buff[1] = (lx_value & 0xFF00) >> 8;
 	nrf_tx_buff[2] = (lx_value & 0x00FF);
	nrf_tx_buff[3] = (ly_value & 0xFF00) >> 8;
 	nrf_tx_buff[4] = (ly_value & 0x00FF);
	nrf_tx_buff[5] = (rx_value & 0xFF00) >> 8;
 	nrf_tx_buff[6] = (rx_value & 0x00FF);
	nrf_tx_buff[7] = (ry_value & 0xFF00) >> 8;
 	nrf_tx_buff[8] = (ry_value & 0x00FF);
	//陀螺仪原始值
	nrf_tx_buff[9]  = (gyrox & 0xFF00) >> 8;
 	nrf_tx_buff[10] = (gyrox & 0x00FF);
	nrf_tx_buff[11] = (gyroy & 0xFF00) >> 8;
 	nrf_tx_buff[12] = (gyroy & 0x00FF);
	nrf_tx_buff[13] = (gyroz & 0xFF00) >> 8;
 	nrf_tx_buff[14] = (gyroz & 0x00FF);
	//按键值
	nrf_tx_buff[15] = key_value;
	
	uchar_to_float.float_data = angle[0];
 	nrf_tx_buff[16] = uchar_to_float.uchar_data[0];
	nrf_tx_buff[17] = uchar_to_float.uchar_data[1];
 	nrf_tx_buff[18] = uchar_to_float.uchar_data[2];
	nrf_tx_buff[19] = uchar_to_float.uchar_data[3];
	
	uchar_to_float.float_data = angle[1];
 	nrf_tx_buff[20] = uchar_to_float.uchar_data[0];
	nrf_tx_buff[21] = uchar_to_float.uchar_data[1];
 	nrf_tx_buff[22] = uchar_to_float.uchar_data[2];
	nrf_tx_buff[23] = uchar_to_float.uchar_data[3];
	
	uchar_to_float.float_data = angle[2];
 	nrf_tx_buff[24] = uchar_to_float.uchar_data[0];
	nrf_tx_buff[25] = uchar_to_float.uchar_data[1];
 	nrf_tx_buff[26] = uchar_to_float.uchar_data[2];
	nrf_tx_buff[27] = uchar_to_float.uchar_data[3];
	
	nrf_tx_buff[28] = 0x3A;
	
	check_tx = NRF24L01_TxPacket(nrf_tx_buff);
}
