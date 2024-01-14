#ifndef _USER_TASK_H
#define _USER_TASK_H

#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

void user_task(void *pvParameters);

extern float angle[];

#endif

