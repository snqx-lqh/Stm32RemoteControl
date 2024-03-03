#include "nrf24L01_middle.h"
#include "nrf24L01_driver.h"

#include "bsp_spi.h"

int     nrf24L01_io_init        (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( NRF24L01_CSN_RCC|  \
							NRF24L01_CE_RCC|   \
							NRF24L01_IRQ_RCC, ENABLE );	
	
	//SPI片选信号
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRF24L01_CSN_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(NRF24L01_CSN_GPIO,NRF24L01_CSN_GPIO_PIN);
	//24L01模式控制信号
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_GPIO_PIN;
    GPIO_Init(NRF24L01_CE_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_CE_GPIO,NRF24L01_CE_GPIO_PIN);
    //IRQ IRQ主机数据输入
    GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;   //上拉输入
    GPIO_Init(NRF24L01_IRQ_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_IRQ_GPIO,NRF24L01_IRQ_GPIO_PIN);
	
	return 0;
}

void    nrf24L01_set_csn_level  (unsigned char data)
{
	if(data==1){
		NRF24L01_CSN = 1;
	}else if(data == 0){
		NRF24L01_CSN = 0;
	}
}
void    nrf24L01_set_ce_level   (unsigned char data)
{
	if(data==1){
		NRF24L01_CE = 1;
	}else if(data == 0){
		NRF24L01_CE = 0;
	}
}
uint8_t nrf24L01_read_write_byte(unsigned char data)
{
	return SPI2_ReadWriteByte(data);
}
uint8_t nrf24L01_read_irq_data  (void)
{
	return GPIO_ReadInputDataBit(NRF24L01_IRQ_GPIO,NRF24L01_IRQ_GPIO_PIN);
}

struct nrf24L01_operations nrf24L01_oper = {
	.io_init= nrf24L01_io_init,
	.set_csn_level= nrf24L01_set_csn_level,
	.set_ce_level= nrf24L01_set_ce_level,
	.read_write_byte= nrf24L01_read_write_byte,
	.read_irq_data= nrf24L01_read_irq_data,
};

void nrf24L01_middle_init()
{
	SPI2_Init();
	nrf24L01_operation_register(&nrf24L01_oper);
	NRF24L01_Init();
}