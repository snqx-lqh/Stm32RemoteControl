#ifndef _BSP_DELAY_H
#define _BSP_DELAY_H

#include "stm32f10x.h"

#define SYSTEM_SUPPORT_OS 1

void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);

#if SYSTEM_SUPPORT_OS
void delay_xms(uint32_t nms);
#endif

#endif
