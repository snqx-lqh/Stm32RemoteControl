#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "main.h"

void bsp_led_init(void);
void led_on(void);
void led_off(void);

#define LED PCout(13)

#endif

