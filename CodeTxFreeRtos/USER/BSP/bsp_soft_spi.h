#ifndef _BSP_SOFT_SPI_H
#define _BSP_SOFT_SPI_H

#include <stdint.h>

struct soft_spi_operations{
	void (*spi_init)(void);
	void (*spi_sck_high)(void);
	void (*spi_sck_low)(void);
	void (*spi_mosi_high)(void);
	void (*spi_mosi_low)(void);
	uint8_t (*spi_miso_read)(void);	
};

uint8_t SOFT_SPI_Init(struct soft_spi_operations *soft_spi);
uint8_t SPI_ReadWriteByte_Mode0(struct soft_spi_operations *soft_spi,uint8_t write_dat );
uint8_t SPI_ReadWriteByte_Mode1(struct soft_spi_operations *soft_spi,uint8_t write_dat );
uint8_t SPI_ReadWriteByte_Mode2(struct soft_spi_operations *soft_spi,uint8_t write_dat );
uint8_t SPI_ReadWriteByte_Mode3(struct soft_spi_operations *soft_spi,uint8_t write_dat );

#endif

