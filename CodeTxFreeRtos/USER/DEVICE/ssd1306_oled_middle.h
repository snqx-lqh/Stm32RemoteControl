#ifndef _SSD1306_OLED_MIDDLE_H
#define _SSD1306_OLED_MIDDLE_H

#include "main.h"

#define IIC_USE 

#ifdef IIC_USE

//-----------------OLED端口定义----------------
#define OLED_RCC_SCL         RCC_APB2Periph_GPIOA
#define OLED_SCL_GPIO        GPIOA
#define OLED_SCL_GPIO_PIN    GPIO_Pin_5

#define OLED_RCC_SDA         RCC_APB2Periph_GPIOA
#define OLED_SDA_GPIO        GPIOA
#define OLED_SDA_GPIO_PIN    GPIO_Pin_7

//#define OLED_RCC_RES         RCC_APB2Periph_GPIOA
//#define OLED_RES_GPIO        GPIOA
//#define OLED_RES_GPIO_PIN    GPIO_Pin_3

#define OLED_SCL_Clr() GPIO_ResetBits(OLED_SCL_GPIO,OLED_SCL_GPIO_PIN)//SCL
#define OLED_SCL_Set() GPIO_SetBits(OLED_SCL_GPIO,OLED_SCL_GPIO_PIN)

#define OLED_SDA_Clr() GPIO_ResetBits(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN)//DIN
#define OLED_SDA_Set() GPIO_SetBits(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN)

#else

//-----------------OLED端口定义----------------
#define OLED_RCC_SCL         RCC_APB2Periph_GPIOC
#define OLED_SCL_GPIO        GPIOC
#define OLED_SCL_GPIO_PIN    GPIO_Pin_15

#define OLED_RCC_SDA         RCC_APB2Periph_GPIOC
#define OLED_SDA_GPIO        GPIOC
#define OLED_SDA_GPIO_PIN    GPIO_Pin_14

#define OLED_RCC_RES         RCC_APB2Periph_GPIOC
#define OLED_RES_GPIO        GPIOC
#define OLED_RES_GPIO_PIN    GPIO_Pin_13

#define OLED_RCC_DC         RCC_APB2Periph_GPIOB
#define OLED_DC_GPIO        GPIOB
#define OLED_DC_GPIO_PIN    GPIO_Pin_4

#define OLED_RCC_CS         RCC_APB2Periph_GPIOA
#define OLED_CS_GPIO        GPIOA
#define OLED_CS_GPIO_PIN    GPIO_Pin_8

#define OLED_SCL_Clr() GPIO_ResetBits(OLED_SCL_GPIO,OLED_SCL_GPIO_PIN)//SCL
#define OLED_SCL_Set() GPIO_SetBits(OLED_SCL_GPIO,OLED_SCL_GPIO_PIN)

#define OLED_SDA_Clr() GPIO_ResetBits(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN)//DIN
#define OLED_SDA_Set() GPIO_SetBits(OLED_SDA_GPIO,OLED_SDA_GPIO_PIN)

#define OLED_RES_Clr() GPIO_ResetBits(OLED_RES_GPIO,OLED_RES_GPIO_PIN)//RES
#define OLED_RES_Set() GPIO_SetBits(OLED_RES_GPIO,OLED_RES_GPIO_PIN)

#define OLED_DC_Clr()  GPIO_ResetBits(OLED_DC_GPIO,OLED_DC_GPIO_PIN)//DC
#define OLED_DC_Set()  GPIO_SetBits(OLED_DC_GPIO,OLED_DC_GPIO_PIN)

// #define OLED_CS_Clr()  GPIO_ResetBits(OLED_CS_GPIO,OLED_CS_GPIO_PIN)//CS
// #define OLED_CS_Set()  GPIO_SetBits(OLED_CS_GPIO,OLED_CS_GPIO_PIN)
#define OLED_CS_Clr()
#define OLED_CS_Set()

#endif

void oled1306_middle_init(void);
void oled1306_example(void);

extern struct ssd1306_oled_operations ssd1306_oled_oper;

#endif
