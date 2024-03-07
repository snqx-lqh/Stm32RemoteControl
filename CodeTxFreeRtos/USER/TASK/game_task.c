#include "game_task.h"

#include "ball_game.h"
#include "snake_game.h"

void game_task(void *pvParameters)
{
	uint16_t game = (uint16_t)pvParameters;
	if(game == 1)
		ball_game_run();
	else if(game == 2)
		snake_game_run();
		
}
