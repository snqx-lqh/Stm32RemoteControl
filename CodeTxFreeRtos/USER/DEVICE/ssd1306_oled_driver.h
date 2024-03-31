#ifndef _SSD1306_OLED_DRIVER_H
#define _SSD1306_OLED_DRIVER_H

#include <stdint.h>

enum ssd1306_iic_or_spi{
	SPI_MODE = 0,
	IIC_MODE = 1
};

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý

typedef struct ssd1306_oled_operations{
	int (*spi_write_one_byte)    (unsigned char data );
	int (*spi_write_len_byte)    (unsigned char *data,unsigned int len);
	void (*set_dc_level)         (unsigned char level); 
	void (*set_cs_level)         (unsigned char level);
	void (*set_res_level)        (unsigned char level);
	void (*delay_ms)             (unsigned short ms);
	
	int (*iic_write_one_byte)    (unsigned char reg,unsigned char data );
	int (*iic_write_len_byte)    (unsigned char reg,unsigned char *data,unsigned int len);

	enum ssd1306_iic_or_spi iic_or_spi;
}ssd1306_oled_oper_t;

void ssd1306_oled_operation_register(struct ssd1306_oled_operations *ssd1306_oled_oper);

void OLED_ClearPoint(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y);
void OLED_ColorTurn(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t i);
void OLED_DisplayTurn(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t i);
//void OLED_WR_Byte(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(ssd1306_oled_oper_t *p_ssd1306_oled_oper);
void OLED_DisPlay_Off(ssd1306_oled_oper_t *p_ssd1306_oled_oper);
void OLED_Refresh(ssd1306_oled_oper_t *p_ssd1306_oled_oper);
void OLED_Clear(ssd1306_oled_oper_t *p_ssd1306_oled_oper);
void OLED_Clear_Buff(ssd1306_oled_oper_t *p_ssd1306_oled_oper);
void OLED_Fill(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawPoint(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t r,uint8_t mode);
void OLED_ShowChar(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowChinese(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_Init(ssd1306_oled_oper_t *p_ssd1306_oled_oper);

#endif

