#include "nrf24l01.h"

SPI_InitTypeDef  SPI_InitStructure; //全局变量SPI结构体

void NRF24L01_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( NRF24L01_SCK_RCC|  \
							NRF24L01_MISO_RCC| \
							NRF24L01_MOSI_RCC| \
	                        NRF24L01_CSN_RCC|  \
							NRF24L01_CE_RCC|   \
							NRF24L01_IRQ_RCC, ENABLE );	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
    
	//时钟信号
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SCK_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRF24L01_SCK_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_SCK_GPIO,NRF24L01_SCK_GPIO_PIN);
	//MISO信号
	GPIO_InitStructure.GPIO_Pin = NRF24L01_MISO_GPIO_PIN;
	GPIO_Init(NRF24L01_MISO_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_MISO_GPIO,NRF24L01_MISO_GPIO_PIN);
	//MOSI信号
	GPIO_InitStructure.GPIO_Pin = NRF24L01_MOSI_GPIO_PIN;
	GPIO_Init(NRF24L01_MOSI_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_MOSI_GPIO,NRF24L01_MOSI_GPIO_PIN);
	//SPI片选信号
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRF24L01_CSN_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(NRF24L01_CSN_GPIO,NRF24L01_CSN_GPIO_PIN);
	//24L01模式控制信号
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_GPIO_PIN;
    GPIO_Init(NRF24L01_CE_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_CE_GPIO,NRF24L01_CE_GPIO_PIN);
    //IRQ IRQ主机数据输入
    GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;   //上拉输入
    GPIO_Init(NRF24L01_IRQ_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_IRQ_GPIO,NRF24L01_IRQ_GPIO_PIN);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //选择了串行时钟的稳态:时钟悬空低电平
    //即(CPOL=0)
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //数据捕获于第一个时钟沿
    //即(CPHA=0)
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;      //定义波特率预分频的值:波特率预分频值为8
    //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    SPI_Cmd(SPI1,ENABLE);//使能SPI1

    //CE低电平 待机模式
    NRF24L01_CE=0;  //使能24L01
    //CSN为低电平芯片工作
    NRF24L01_CSN=1; //SPI片选取消
}

//void Delay(vu32 nCount)
//{
//  for(; nCount != 0; nCount--);
//}

////模拟SPI读写数据函数
//u8 SPI1_ReadWriteByte(u8 TxData)                                        
//{		
//	u16 bit_ctr;
//   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) 
//   	{
//		if(TxData & 0x80)
//			NRF24L01_MOSI = 1;         
//		else
//			NRF24L01_MOSI = 0;
//		TxData = (TxData << 1);           
//		NRF24L01_SCK=1; 
//		Delay(0xff);
//		if(NRF24L01_MISO)                     
//		TxData |= 0x01;       		  
//		NRF24L01_SCK=0; 
//		Delay(0xff);           		 
//   	}
//    return(TxData);           		  		    
//}

//通过SPI1发送或者接收一个字节的数据
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 retry=0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查发送缓存空标志位设置与否
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI1, TxData); //通过外设SPI1发送一个数据
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查接受缓存非空标志位设置与否
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI1); //返回通过SPI1最近接收的数据
}

//SPI写寄存器
//入口参数: reg:指定寄存器地址; value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;
    NRF24L01_CSN=0;                 //使能SPI传输
    status =SPI1_ReadWriteByte(reg);//发送寄存器号
    SPI1_ReadWriteByte(value);      //写入寄存器的值
    NRF24L01_CSN=1;                 //禁止SPI传输
    return(status);                 //返回状态值
}
//读取SPI寄存器值
//入口参数:reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    NRF24L01_CSN = 0;          //使能SPI传输
    SPI1_ReadWriteByte(reg);   //发送寄存器号
    reg_val=SPI1_ReadWriteByte(0XFF);//读取寄存器内容
    NRF24L01_CSN = 1;          //禁止SPI传输
    return(reg_val);           //返回状态值
}

//在指定位置读出指定长度的数据
//入口参数:reg:寄存器(位置); *pBuf:数据指针; len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,u8_ctr;
    NRF24L01_CSN = 0;           //使能SPI传输
    status=SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//读出数据
    NRF24L01_CSN=1;       //关闭SPI传输
    return status;        //返回读到的状态值
}
//在指定位置写指定长度的数据
//入口参数:reg:寄存器(位置); *pBuf:数据指针; len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,u8_ctr;
    NRF24L01_CSN = 0;          //使能SPI传输
    status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        SPI1_ReadWriteByte(*pBuf++); //写入数据
    NRF24L01_CSN = 1;       //关闭SPI传输
    return status;          //返回读到的状态值
}

const u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //接收地址
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void NRF24L01_RX_Mode(void)
{
    NRF24L01_CE=0;
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
    //写RX节点地址
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);           //设置RF通信频率
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
    //即32个字节
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);       //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
    //向CONFIG(00)写入0000 1111;PRIM_RX=1 接收模式; PWR_UP=1 上电; CRCO=1 16位CRC校验; EN_CRC =1 CRC使能
    NRF24L01_CE = 1; //CE为高,进入接收模式
}
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void NRF24L01_TX_Mode(void)
{
    NRF24L01_CE=0;
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);
    //写TX节点地址
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
    //设置TX节点地址,主要为了使能ACK
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
    NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	
    //向CONFIG(00)写入0000 1111;PRIM_RX=0 发射模式; PWR_UP=1 上电; CRCO=1 16位CRC校验; EN_CRC   =1 CRC使能
    NRF24L01_CE=1;//CE为高,10us后启动发送
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    NRF24L01_CE=0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
    NRF24L01_CE=1;//启动发送
    while(NRF24L01_IRQ!=0);//等待发送完成
    sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
    if(sta&MAX_TX)//达到最大重发次数
    {
        NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器
        return MAX_TX;
    }
    if(sta&TX_OK)//发送完成
    {
        return TX_OK;
    }
    return 0xff;//其他原因发送失败
}
//启动NRF24L01接收一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
    u8 sta;
    sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
    if(sta&RX_OK)//接收到数据
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
        NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器
        return 0;
    }
    return 1;//没收到任何数据
}

//检测24L01是否存在
//返回值:0，成功;1，失败
u8 buf[5]= {0XAA,0XAA,0XAA,0XAA,0XAA}; //写入5个0XAA字节
u8 NRF24L01_Check(void)
{
    
    u8 i;
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址
    for(i=0; i<5; i++)
        if(buf[i]!=0XAA)
            break;
    if(i!=5)return 1;//检测24L01错误
    return 0;        //检测到24L01
}
