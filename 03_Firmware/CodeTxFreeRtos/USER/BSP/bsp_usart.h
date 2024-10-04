#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "main.h"

/*
串口号    没有重映像    部分重映像   全部重映像
1			A9/A10	                    B6/B7
2           A2/A3                       D5/D6
3           B10/B11       C10/C11       D8/D9
4           C10/C11
5           C12/D2
GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
*/

void bsp_usart1_init(u32 bound);

void USART1_Send_Len_Data(uint8_t *sendArray,uint8_t sendLen);
void USART1_Receive_Len_Data(uint8_t *buffer, uint16_t length);
#endif

