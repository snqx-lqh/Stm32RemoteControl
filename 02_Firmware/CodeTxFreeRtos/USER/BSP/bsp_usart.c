#include "bsp_usart.h"
#include "stdio.h"

#if CONFIG_LETTER_SHELL_ENABLE
#include "shell.h"
extern Shell shell;
#endif

#if CONFIG_ANO_ENABLE
extern void AnoPTv8HwRecvByte(uint8_t dat);
#endif

void DebugUsartMain()
{
    uint8_t res;
    res = USART_ReceiveData(USART1);
    //USART_SendData(USART1,res);
	
	#if CONFIG_LETTER_SHELL_ENABLE
	//shellHandler(&shell,res);
	#endif
	
	#if CONFIG_ANO_ENABLE
	AnoPTv8HwRecvByte(res);
	#endif
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        DebugUsartMain();
        EXTI_ClearITPendingBit(USART_IT_RXNE);
    }
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口 */
    USART_SendData(USART1, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//    /* 等待串口输入数据 */
//    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

//    return (int)USART_ReceiveData(USART1);
//}

void USART1_Send_Len_Data(uint8_t *sendArray,uint8_t sendLen)
{
	int i = 0;
	for(i=0;i<sendLen;i++)
	{
		USART_SendData(USART1, sendArray[i]);//向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束				
	}	
}

void USART1_Receive_Len_Data(uint8_t *buffer, uint16_t length) 
{
    for (uint16_t i = 0; i < length; ++i) {
        // 等待接收数据就绪
        while (!(USART1->SR & USART_SR_RXNE));

        // 从数据寄存器读取接收到的数据
        buffer[i] = USART1->DR;
    }
}


void bsp_usart1_init(u32 bound)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|
                           RCC_APB2Periph_GPIOA,  ENABLE);	//使能USART1，GPIOA时钟
    //GPIO端口设置
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        //USART1_TX   GPIOA.9
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        //USART1_RX	  GPIOA.10初始化
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    //Usart1 NVIC 配置
    {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
    }
    //USART 初始化设置
    {
        USART_InitTypeDef USART_InitStructure;
        USART_InitStructure.USART_BaudRate = bound;//串口波特率
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
        USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

        USART_Init(USART1, &USART_InitStructure); //初始化串口1
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
        USART_Cmd(USART1, ENABLE);                    //使能串口1
    }
}




