#include "snake_game.h"

#include "ssd1306_oled_driver.h"
#include "ssd1306_oled_middle.h"
#include "keyscan.h"
#include "rc_data_task.h"
#include "semphr.h"

extern TaskHandle_t   GUITask_Handler;
extern SemaphoreHandle_t rcDataMutexSemaphore;

#define OLED_WIDTH  128
#define OLED_HIGH   64

#define GM_BOX_LEFT  39

static void draw_game_box(uint8_t score,uint8_t life ,uint8_t level)
{
	//画一个游戏边框
	OLED_DrawLine(&ssd1306_oled_oper,0,0, 127,0,1);
	OLED_DrawLine(&ssd1306_oled_oper,0,63,127,63,1);
	OLED_DrawLine(&ssd1306_oled_oper,0,0,0,63,1);
	OLED_DrawLine(&ssd1306_oled_oper,127,0,127,63,1);
	OLED_DrawLine(&ssd1306_oled_oper,GM_BOX_LEFT,0,GM_BOX_LEFT,63,1);
	//写上参数
	OLED_ShowString(&ssd1306_oled_oper,2,2, "score",12,1);
	OLED_ShowNum(&ssd1306_oled_oper,2,14,score,3,8,1);
	OLED_ShowString(&ssd1306_oled_oper,2,22,"life ",12,1);
	OLED_ShowNum(&ssd1306_oled_oper,2,34,life,3,8,1);
	OLED_ShowString(&ssd1306_oled_oper,2,42,"level",12,1);
	OLED_ShowNum(&ssd1306_oled_oper,2,54,level,3,8,1);
}

void snake_game_run()
{
	OLED_Clear_Buff(&ssd1306_oled_oper);
	draw_game_box(1,2 ,3);
	OLED_Refresh(&ssd1306_oled_oper);
	while(1)
	{
		if(isKeyUp == 1)
		{
			isKeyUp = 0;
			vTaskResume(GUITask_Handler);
			OLED_Clear_Buff(&ssd1306_oled_oper);
			OLED_Refresh(&ssd1306_oled_oper);
			vTaskDelete(NULL);
		}
		vTaskDelay(10);
	}
}

