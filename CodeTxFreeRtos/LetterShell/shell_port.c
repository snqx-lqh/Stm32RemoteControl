/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "shell.h"
#include "bsp_usart.h"
#include "stm32f10x.h"

Shell shell;
char shellBuffer[512];

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 */
void userShellWrite(char data)
{
    USART1_Send_Len_Data((uint8_t *)&data, 1);
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @return char 状态
 */
signed char userShellRead(char *data)
{
    *data = 0;
    while (!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
    {
    }
    *data = USART_ReceiveData(USART1);
	return 0;  //返回0标识数据已经准备OK
}

TaskHandle_t         MYSHELLTask_Handler;
/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
    shell.read = userShellRead;
    shellInit(&shell, shellBuffer, 512);
	xTaskCreate((TaskFunction_t)shellTask,          //任务函数
                (const char *)"shellTask",          //任务名称
                (uint16_t)256,            //任务堆栈大小
                (void *)&shell,                        //传递给任务函数的参数
                (UBaseType_t)1,        //任务优先级
                (TaskHandle_t *)MYSHELLTask_Handler); //任务句柄

}

