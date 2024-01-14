/**
  ******************************************************************************
  * @file    shell_task.c 
  * @author  少年潜行(snqx-lgh)
  * @version V
  * @date    2024-01-10
  * @brief   移植letter-shell，做shell相关任务
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; Copyright 2024 LQH,China</center></h2>
  ******************************************************************************
  */#include "shell_task.h"

#include "shell_port.h"

void shell_task(void *pvParameters)
{
	userShellInit();
	while(1)
	{	
		
		vTaskDelay(5);
	}
}

int shell_fun(uint32_t num)
{
	printf("shell:%d\r\n",num);
	return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), shell_cmd, shell_fun, shell_cmd-shell_fun);

int shell_add(uint32_t num1,uint32_t num2)
{
	printf("shell:%d\r\n",num1 + num2);
	return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), shell_add_cmd, shell_add, shell_add_cmd-shell_add);

//int func(int argc, char *agrv[])
//{
//    printf("%dparameter(s)\r\n", argc);
//    for (char i = 1; i < argc; i++)
//    {
//        printf("%s\r\n", agrv[i]);
//    }
//}
//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func, func, test);
 

