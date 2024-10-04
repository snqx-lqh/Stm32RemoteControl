#ifndef _NRF24L01_MIDDLE_H
#define _NRF24L01_MIDDLE_H

#include "main.h"

//SPI的片选信号  
#define NRF24L01_CSN_RCC        RCC_APB2Periph_GPIOA
#define NRF24L01_CSN_GPIO       GPIOA
#define NRF24L01_CSN_GPIO_PIN   GPIO_Pin_11
#define NRF24L01_CSN            PAout(11)
//24L01模式控制信号
#define NRF24L01_CE_RCC         RCC_APB2Periph_GPIOB
#define NRF24L01_CE_GPIO        GPIOB
#define NRF24L01_CE_GPIO_PIN    GPIO_Pin_12
#define NRF24L01_CE             PBout(12)
//IRQ主机数据输入
#define NRF24L01_IRQ_RCC        RCC_APB2Periph_GPIOA
#define NRF24L01_IRQ_GPIO       GPIOA
#define NRF24L01_IRQ_GPIO_PIN   GPIO_Pin_8
#define NRF24L01_IRQ            PAin(8)

void nrf24L01_middle_init(void);

#endif
