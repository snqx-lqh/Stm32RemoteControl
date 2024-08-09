#ifndef _BSP_SYS_H
#define _BSP_SYS_H

#include "main.h"

//定义GPIO结构体位域
typedef struct
{
   volatile unsigned short bit0 : 1;
   volatile unsigned short bit1 : 1;
   volatile unsigned short bit2 : 1;
   volatile unsigned short bit3 : 1;
   volatile unsigned short bit4 : 1;
   volatile unsigned short bit5 : 1;
   volatile unsigned short bit6 : 1;
   volatile unsigned short bit7 : 1;
   volatile unsigned short bit8 : 1;
   volatile unsigned short bit9 : 1;
   volatile unsigned short bit10 : 1;
   volatile unsigned short bit11 : 1;
   volatile unsigned short bit12 : 1;
   volatile unsigned short bit13 : 1;
   volatile unsigned short bit14 : 1;
   volatile unsigned short bit15 : 1;
 
} GPIO_Bit_TypeDef;

#define PORTA_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOA->ODR)))
#define PORTA_IN     ((GPIO_Bit_TypeDef *)(&(GPIOA->IDR)))
#define PORTB_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOB->ODR)))
#define PORTB_IN     ((GPIO_Bit_TypeDef *)(&(GPIOB->IDR)))
#define PORTC_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOC->ODR)))
#define PORTC_IN     ((GPIO_Bit_TypeDef *)(&(GPIOC->IDR)))
#define PORTD_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOD->ODR)))
#define PORTD_IN     ((GPIO_Bit_TypeDef *)(&(GPIOD->IDR)))
#define PORTE_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOE->ODR)))
#define PORTE_IN     ((GPIO_Bit_TypeDef *)(&(GPIOE->IDR)))
#define PORTF_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOF->ODR)))
#define PORTF_IN     ((GPIO_Bit_TypeDef *)(&(GPIOF->IDR)))
#define PORTG_OUT    ((GPIO_Bit_TypeDef *)(&(GPIOG->ODR)))
#define PORTG_IN     ((GPIO_Bit_TypeDef *)(&(GPIOG->IDR)))

#define PAout(n) (PORTA_OUT->bit##n)
#define PAin(n)  (PORTA_IN->bit##n)
#define PBout(n) (PORTB_OUT->bit##n)
#define PBin(n)  (PORTB_IN->bit##n)
#define PCout(n) (PORTC_OUT->bit##n)
#define PCin(n)  (PORTC_IN->bit##n)
#define PDout(n) (PORTD_OUT->bit##n)
#define PDin(n)  (PORTD_IN->bit##n)
#define PEout(n) (PORTE_OUT->bit##n)
#define PEin(n)  (PORTE_IN->bit##n)
#define PFout(n) (PORTF_OUT->bit##n)
#define PFin(n)  (PORTF_IN->bit##n)
#define PGout(n) (PORTG_OUT->bit##n)
#define PGin(n)  (PORTG_IN->bit##n)

#endif
