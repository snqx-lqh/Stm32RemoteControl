#include "ball_game.h"

#include "ssd1306_oled_driver.h"
#include "keyscan.h"
#include "rc_data_task.h"
#include "semphr.h"

extern TaskHandle_t   GUITask_Handler;
extern SemaphoreHandle_t rcDataMutexSemaphore;

#define BALL_START_X 56
#define BALL_START_Y 3

#define OLED_WIDTH  128
#define OLED_HIGH   64

#define BALL_RADIUS  2
#define GM_BOX_LEFT  39

static uint8_t ball_pos_x = 56     ,ball_pos_y = 3;
static uint8_t ball_pos_x_last = 56,ball_pos_y_last = 3;
static int8_t  ball_pos_x_dir = 2  ,ball_pos_y_dir = 2;

static uint8_t board_pos_x = 40     ,board_pos_y = 61;
static uint8_t board_pos_x_last = 0 ,board_pos_y_last = 61;
static uint8_t board_length = 40;

static uint8_t life  = 2;
static uint8_t score = 0;
static uint8_t level = 1;
static uint8_t ball_speed_count = 4; 

static void draw_game_box(uint8_t score,uint8_t life ,uint8_t level)
{
	//画一个游戏边框
	OLED_DrawLine(0,0, 127,0,1);
	OLED_DrawLine(0,63,127,63,1);
	OLED_DrawLine(0,0,0,63,1);
	OLED_DrawLine(127,0,127,63,1);
	OLED_DrawLine(GM_BOX_LEFT,0,GM_BOX_LEFT,63,1);
	//写上参数
	OLED_ShowString(2,2, "score",12,1);
	OLED_ShowNum(2,14,score,3,8,1);
	OLED_ShowString(2,22,"life ",12,1);
	OLED_ShowNum(2,34,life,3,8,1);
	OLED_ShowString(2,42,"level",12,1);
	OLED_ShowNum(2,54,level,3,8,1);
}

void ball_init()
{
	ball_pos_x = BALL_START_X;
	ball_pos_y = BALL_START_Y;
	ball_pos_x_dir = 2;
	ball_pos_y_dir = 2;
}

void draw_ball()
{
	OLED_DrawCircle(ball_pos_x,ball_pos_y,BALL_RADIUS,1);
}

void draw_ball_clear()
{
	OLED_DrawCircle(ball_pos_x_last,ball_pos_y_last,BALL_RADIUS,0);
}

void ball_move()
{
	ball_pos_x = ball_pos_x + ball_pos_x_dir;
	ball_pos_y = ball_pos_y + ball_pos_y_dir;
	
	if(ball_pos_x > (OLED_WIDTH - BALL_RADIUS - 3))
	{
		ball_pos_x = OLED_WIDTH - BALL_RADIUS - 3;
		ball_pos_x_dir = -2;
	}else if(ball_pos_x < GM_BOX_LEFT + BALL_RADIUS + 3)
	{
		ball_pos_x_dir = GM_BOX_LEFT + BALL_RADIUS + 3;
		ball_pos_x_dir = 2;
	}
		
	if(ball_pos_y <= BALL_RADIUS + 1) 
	{
		ball_pos_y = BALL_RADIUS + 1;
		ball_pos_y_dir = 2;
	}
}

void draw_board()
{
	OLED_DrawLine(board_pos_x,board_pos_y,board_pos_x+board_length,board_pos_y,1);
	OLED_DrawLine(board_pos_x,board_pos_y+1,board_pos_x+board_length,board_pos_y+1,1);
}

void draw_board_clear()
{
	OLED_DrawLine(board_pos_x_last,board_pos_y_last,board_pos_x_last+board_length,board_pos_y_last,0);
	OLED_DrawLine(board_pos_x_last,board_pos_y_last+1,board_pos_x_last+board_length,board_pos_y_last+1,0);
}

void board_move(uint8_t  key_value)
{
	if(key_value == 3)
		board_pos_x += 5;
	else if(key_value == 1)
		board_pos_x -= 5;
	else 
		board_pos_x = board_pos_x;
	
	if(board_pos_x < (GM_BOX_LEFT + 1))
		board_pos_x = (GM_BOX_LEFT + 1);
	else if(board_pos_x > (OLED_WIDTH - 1 - board_length))
		board_pos_x = (OLED_WIDTH - 1 - board_length);
}

void ball_game_run()
{
	uint8_t showStr[20] = {0};
	rc_data_t rc_data_temp;
	uint8_t ball_move_count = 0;
	uint8_t  key_value = 0;
	rc_data_init(&rc_data_temp);
	
	OLED_Clear_Buff();
//	OLED_Refresh();
	
	sprintf((char*)showStr,"level %u",level);
	OLED_ShowString(55,25,showStr,16,1);
	draw_game_box(score,life ,level);
	OLED_Refresh();
	vTaskDelay(1000);
	OLED_Clear_Buff();
	draw_game_box(score,life ,level);
	OLED_Refresh();
	
	while(1)
	{	
		// 等待互斥量，确保在写入共享数据时不会被其他任务中断
		if (xSemaphoreTake(rcDataMutexSemaphore, portMAX_DELAY) == pdTRUE) {
			// 更新共享数据
			rc_data_temp = rc_data;
			// 释放互斥量
			xSemaphoreGive(rcDataMutexSemaphore);
		}
		if(rc_data_temp.ry_value-2047 < -1000)
			key_value = 3;
		else if((rc_data_temp.ry_value-2047 > 1000))
			key_value = 1;
		else 
			key_value = 2;
		
		draw_game_box(score, life ,level);
		
		draw_ball_clear();
		draw_board_clear();
		
		board_move(key_value);
		
		ball_move_count++;
		if(ball_move_count > ball_speed_count)
		{	
			ball_move();	
			ball_move_count = 0;
			if(ball_pos_y >= (OLED_HIGH - BALL_RADIUS - 4) && (ball_pos_x > board_pos_x && ball_pos_x < (board_pos_x+board_length))) 
			{
				ball_pos_y = OLED_HIGH - BALL_RADIUS - 4;
				ball_pos_y_dir = -2;
				score ++;
				if(score > 2)
				{
					score = 0;
					level ++;
					if(level > 4)
					{
						OLED_ShowString(55,25,"you win",16,1);
						OLED_Refresh();
						while(isKeyDown == 0)
						{
							vTaskDelay(50);
						}
						isKeyDown = 0;
						life = 2;
						level = 1;
						OLED_Clear_Buff();
						OLED_Refresh();
						ball_init();
						board_length = 40;
						ball_speed_count = 5;
					}else
					{
						sprintf((char*)showStr,"level %u",level);
						OLED_ShowString(55,25,showStr,16,1);
						OLED_Refresh();
						vTaskDelay(1000);
						life = 2;
						OLED_Clear_Buff();
						OLED_Refresh();
						ball_init();
						board_length = board_length - 8;
						ball_speed_count --;
					}
				}		
			}
			else if(ball_pos_y >= OLED_HIGH - BALL_RADIUS - 4)
			{
				ball_pos_x = BALL_START_X;
				ball_pos_y = BALL_START_Y;
				life -= 1;
			}
			
		}
		if(life == 0)
		{
			OLED_ShowString(50,25,"game_over",16,1);
			OLED_Refresh();
			while(isKeyDown == 0)
			{
				vTaskDelay(50);
			}
			isKeyDown = 0;
			life = 2;
			OLED_Clear_Buff();
			OLED_Refresh();
			ball_init();
			board_length = 40;
			level = 1;
			ball_speed_count = 5;
			sprintf((char*)showStr,"level %u",level);
			OLED_ShowString(55,25,showStr,16,1);
			draw_game_box(score,life ,level);
			OLED_Refresh();
			vTaskDelay(1000);
			OLED_Clear_Buff();
			draw_game_box(score,life ,level);
			OLED_Refresh();
		}
		
		ball_pos_x_last = ball_pos_x;
		ball_pos_y_last = ball_pos_y;
		
		board_pos_x_last = board_pos_x;
		board_pos_y_last = board_pos_y;
		
		draw_ball();
		draw_board();
		OLED_Refresh();
		
		if(isKeyUp == 1)
		{
			isKeyUp = 0;
			vTaskResume(GUITask_Handler);
			OLED_Clear_Buff();
			OLED_Refresh();
			vTaskDelete(NULL);
		}
		vTaskDelay(10);
	}
}