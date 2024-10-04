#include "bsp_spi.h"

SPI_InitTypeDef  SPI2_InitStructure; //全局变量SPI结构体

void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );	
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
    
	//时钟信号
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置SPI工作模式:设置为主SPI
    SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //选择了串行时钟的稳态:时钟悬空低电平
    //即(CPOL=0)
    SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //数据捕获于第一个时钟沿
    //即(CPHA=0)
    SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;      //定义波特率预分频的值:波特率预分频值为8
    //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
    SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
    SPI_Init(SPI2, &SPI2_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    SPI_Cmd(SPI2,ENABLE);//使能SPI1

}

//通过SPI2发送或者接收一个字节的数据
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
    uint8_t retry=0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查发送缓存空标志位设置与否
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //通过外设SPI1发送一个数据
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//检查接受缓存非空标志位设置与否
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //返回通过SPI1最近接收的数据
}