#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "main.h"

void bsp_adc_init(void);
u16 get_lx_value(void); 
u16 get_ly_value(void); 
u16 get_rx_value(void); 
u16 get_ry_value(void); 
u16 get_power_value(void); 

#endif

