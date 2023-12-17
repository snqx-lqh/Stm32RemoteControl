#include "gui_task.h"

#include "GUI.h"

void gui_task(void *pvParameters)
{
	
	while(1)
	{
		count1++;
		count2++;
		count3++;
		count4++;
		GuiControl();
		GuiDataDisplayRefresh();
		vTaskDelay(20);
	}
}
