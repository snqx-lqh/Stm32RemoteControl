#include "nrf24l01.h"



void NRF24L01_Init(void)
{
    //CE低电平 待机模式
    RF24L01_SET_CE_LOW();  //使能24L01
    //CSN为低电平芯片工作
    RF24L01_SET_CS_HIGH(); //SPI片选取消
	HAL_Delay(10);
	NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
	//HAL_Delay(10);
}

/**
 * @brief       SPI1读写一个字节数据
 * @param       txdata  : 要发送的数据(1字节)
 * @retval      接收到的数据(1字节)
 */
uint8_t SPI1_ReadWriteByte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 0xFFFF);
    return rxdata; /* 返回收到的数据 */
}


//SPI写寄存器
//入口参数: reg:指定寄存器地址; value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;
    RF24L01_SET_CS_LOW();                 //使能SPI传输
    status =SPI1_ReadWriteByte(reg);//发送寄存器号
    SPI1_ReadWriteByte(value);      //写入寄存器的值
    RF24L01_SET_CS_HIGH();                 //禁止SPI传输
    return(status);                 //返回状态值
}
//读取SPI寄存器值
//入口参数:reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    RF24L01_SET_CS_LOW();          //使能SPI传输
    SPI1_ReadWriteByte(reg);   //发送寄存器号
    reg_val=SPI1_ReadWriteByte(0XFF);//读取寄存器内容
    RF24L01_SET_CS_HIGH();          //禁止SPI传输
    return(reg_val);           //返回状态值
}

//在指定位置读出指定长度的数据
//入口参数:reg:寄存器(位置); *pBuf:数据指针; len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,u8_ctr;
    RF24L01_SET_CS_LOW();           //使能SPI传输
    status=SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//读出数据
    RF24L01_SET_CS_HIGH();       //关闭SPI传输
    return status;        //返回读到的状态值
}
//在指定位置写指定长度的数据
//入口参数:reg:寄存器(位置); *pBuf:数据指针; len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,u8_ctr;
    RF24L01_SET_CS_LOW();          //使能SPI传输
    status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        SPI1_ReadWriteByte(*pBuf++); //写入数据
    RF24L01_SET_CS_HIGH();       //关闭SPI传输
    return status;          //返回读到的状态值
}

const u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //接收地址
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void NRF24L01_RX_Mode(void)
{
    RF24L01_SET_CE_LOW();
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
    RF24L01_SET_CE_HIGH(); //CE为高,进入接收模式
}
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void NRF24L01_TX_Mode(void)
{
    RF24L01_SET_CE_LOW();
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
    RF24L01_SET_CE_HIGH();//CE为高,10us后启动发送
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    RF24L01_SET_CE_LOW();
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
    RF24L01_SET_CE_HIGH();//启动发送
    while(RF24L01_GET_IRQ_STATUS()!=0);//等待发送完成
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

/**
  * @brief :NRF24L01读状态寄存器
  * @param :无
  * @note  :无
  * @retval:RF24L01状态
  */
uint8_t NRF24L01_Read_Status_Register( void )
{
    uint8_t Status;
	
    RF24L01_SET_CS_LOW( );		// 片选
	
    Status = SPI1_ReadWriteByte( NRF_READ_REG + STATUS );	// 读状态寄存器
	
    RF24L01_SET_CS_HIGH( );		// 取消片选
	
    return Status;
}

/**
  * @brief :NRF24L01清中断
  * @param :
           @IRQ_Source:中断源
  * @note  :无
  * @retval:清除后状态寄存器的值
  */
uint8_t NRF24L01_Clear_IRQ_Flag( uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	// 中断标志处理
    btmp = NRF24L01_Read_Status_Register( );			//读状态寄存器
			
    RF24L01_SET_CS_LOW( );			// 片选
    SPI1_ReadWriteByte( NRF_WRITE_REG + STATUS );	// 写状态寄存器命令
    SPI1_ReadWriteByte( IRQ_Source | btmp );		// 清相应中断标志
    RF24L01_SET_CS_HIGH( );			// 取消片选
	
    return ( NRF24L01_Read_Status_Register( ));			// 返回状态寄存器状态
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
