/**
  ******************************************************************************
  * @file    bsp_soft_spi.c 
  * @author  ����Ǳ��(snqx-lqh)
  * @version V
  * @date    2024-03-01
  * @brief   ���SPIģ�飬ʹ��ģ�黯��̣��û�ʹ�õ�ʱ����Ҫ��дsoft_spi_t�ṹ����
			 ����غ���
			 void (*spi_sck_high)(void);            
			 void (*spi_sck_low)(void);
			 void (*spi_mosi_high)(void);
			 void (*spi_mosi_low)(void);
			 uint8_t (*spi_miso_read)(void);
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */
#include "bsp_soft_spi.h"

uint8_t SOFT_SPI_Init(struct soft_spi_operations *soft_spi)
{
	soft_spi->spi_init();
	return 0;
}

/* CPOL = 0, CPHA = 0, MSB first */
uint8_t SPI_ReadWriteByte_Mode0(struct soft_spi_operations *soft_spi,uint8_t write_dat )
{
    uint8_t i, read_dat, read_temp;;
    for( i = 0; i < 8; i++ )
    {
        if( write_dat & 0x80 )
            soft_spi->spi_mosi_high();  
        else                    
            soft_spi->spi_mosi_low();  
        write_dat <<= 1;
        soft_spi->spi_sck_high(); 
        read_dat <<= 1;  
        read_temp = soft_spi->spi_miso_read();
        if( read_temp ) 
            read_dat |= 0x01; 
        soft_spi->spi_sck_low(); 
    }	
    return read_dat;
}
 
/* CPOL=0��CPHA=1, MSB first */
uint8_t SPI_ReadWriteByte_Mode1(struct soft_spi_operations *soft_spi,uint8_t write_dat) 
{
    uint8_t i,read_dat=0, read_temp;;
 
	for(i=0;i<8;i++)     
	{
		soft_spi->spi_sck_high();      
		if(write_dat&0x80)
			soft_spi->spi_mosi_high();   
		else      
			soft_spi->spi_mosi_low();    
		write_dat <<= 1;     
		soft_spi->spi_sck_low();      
		read_dat <<= 1;    
		read_temp = soft_spi->spi_miso_read();
        if( read_temp ) 
			read_dat |= 0x01;    
	}
	return (read_dat);     
}
 
/* CPOL=1��CPHA=0, MSB first */
uint8_t SPI_ReadWriteByte_Mode2(struct soft_spi_operations *soft_spi,uint8_t write_dat) 
{
    uint8_t i,read_dat=0, read_temp;;
 
	for(i=0;i<8;i++)    
	{
		if(write_dat&0x80)
			soft_spi->spi_mosi_high();  
		else      
			soft_spi->spi_mosi_low();   
		write_dat <<= 1;     
		soft_spi->spi_sck_low();     
		read_dat <<= 1;     
		read_temp = soft_spi->spi_miso_read();
        if( read_temp ) 
			read_dat |= 0x01;      
		soft_spi->spi_sck_high();  
	}
	return (read_dat);     
}
 
/* CPOL = 1, CPHA = 1, MSB first */
uint8_t SPI_ReadWriteByte_Mode3(struct soft_spi_operations *soft_spi,uint8_t write_dat )
{
    uint8_t i, read_dat, read_temp;;
    for( i = 0; i < 8; i++ )
    {
		soft_spi->spi_sck_low();
        if( write_dat & 0x80 )
            soft_spi->spi_mosi_high(); 
        else                    
            soft_spi->spi_mosi_low();
        write_dat <<= 1;
        soft_spi->spi_sck_high();
        read_dat <<= 1;  
        read_temp = soft_spi->spi_miso_read();
        if( read_temp ) 
            read_dat |= 0x01; 
    }
    return read_dat;
}
 
