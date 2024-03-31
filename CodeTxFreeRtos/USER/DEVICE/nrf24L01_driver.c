#include "nrf24L01_driver.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //接收地址

void NRF24L01_Init(nrf24L01_oper_t *p_nrf24L01_operations)
{
    p_nrf24L01_operations->io_init();

    p_nrf24L01_operations->set_ce_level(0);  
    p_nrf24L01_operations->set_csn_level(1); 
}

uint8_t NRF24L01_Write_Reg(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t reg,uint8_t value)
{
    uint8_t status;
    p_nrf24L01_operations->set_csn_level(0);              
    status =p_nrf24L01_operations->read_write_byte(reg);
    p_nrf24L01_operations->read_write_byte(value);      
    p_nrf24L01_operations->set_csn_level(1);                 
    return status;                
}

uint8_t NRF24L01_Read_Reg(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t reg)
{
    uint8_t reg_val;
    p_nrf24L01_operations->set_csn_level(0);         
    p_nrf24L01_operations->read_write_byte(reg);  
    reg_val=p_nrf24L01_operations->read_write_byte(0XFF);
    p_nrf24L01_operations->set_csn_level(1);          
    return reg_val;          
}

uint8_t NRF24L01_Read_Buf(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t reg,uint8_t *pBuf,uint8_t len)
{
    uint8_t status,uint8_t_ctr;
    p_nrf24L01_operations->set_csn_level(0);          
    status=p_nrf24L01_operations->read_write_byte(reg);
    for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
        pBuf[uint8_t_ctr]=p_nrf24L01_operations->read_write_byte(0XFF);
    p_nrf24L01_operations->set_csn_level(1);       
    return status;        
}

uint8_t NRF24L01_Write_Buf(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status,uint8_t_ctr;
    p_nrf24L01_operations->set_csn_level(0);          
    status = p_nrf24L01_operations->read_write_byte(reg);
    for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
        p_nrf24L01_operations->read_write_byte(*pBuf++); 
    p_nrf24L01_operations->set_csn_level(1);       
    return status;        
}

/**
  * @brief   该函数初始化NRF24L01到RX模式
			 设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
			 当CE变高后,即进入RX模式,并可以接收数据了
  * @param   None
  * @retval
 **/
void NRF24L01_RX_Mode(nrf24L01_oper_t *p_nrf24L01_operations)
{
    p_nrf24L01_operations->set_ce_level(0);
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);
    //写RX节点地址
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_CH,40);           //设置RF通信频率
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
    //即32个字节
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+CONFIG, 0x0f);       //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
    //向CONFIG(00)写入0000 1111;PRIM_RX=1 接收模式; PWR_UP=1 上电; CRCO=1 16位CRC校验; EN_CRC =1 CRC使能
    p_nrf24L01_operations->set_ce_level(1); //CE为高,进入接收模式
}

/**
  * @brief   该函数初始化NRF24L01到TX模式
			 设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
			 PWR_UP,CRC使能
			 当CE变高后,即进入RX模式,并可以接收数据了
			 CE为高大于10us,则启动发送.
  * @param   None
  * @retval
 **/
void NRF24L01_TX_Mode(nrf24L01_oper_t *p_nrf24L01_operations)
{
    p_nrf24L01_operations->set_ce_level(0);
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);
    //写TX节点地址
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);
    //设置TX节点地址,主要为了使能ACK
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	
    //向CONFIG(00)写入0000 1111;PRIM_RX=0 发射模式; PWR_UP=1 上电; CRCO=1 16位CRC校验; EN_CRC   =1 CRC使能
    p_nrf24L01_operations->set_ce_level(1);//CE为高,10us后启动发送
}

/**
  * @brief   启动NRF24L01发送一次数据
  * @param   txbuf:待发送数据首地址
  * @retval  发送完成状况
 **/
uint8_t NRF24L01_TxPacket(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t *txbuf)
{
    uint8_t sta;
	uint8_t read_irq_data;
    p_nrf24L01_operations->set_ce_level(0);
    NRF24L01_Write_Buf(p_nrf24L01_operations,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
    p_nrf24L01_operations->set_ce_level(1);//启动发送
	do{
		read_irq_data = p_nrf24L01_operations->read_irq_data();
    }while(read_irq_data !=0 );//等待发送完成
    sta=NRF24L01_Read_Reg(p_nrf24L01_operations,STATUS);  //读取状态寄存器的值
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
    if(sta&MAX_TX)//达到最大重发次数
    {
        NRF24L01_Write_Reg(p_nrf24L01_operations,FLUSH_TX,0xff);//清除TX FIFO寄存器
        return MAX_TX;
    }
    if(sta&TX_OK)//发送完成
    {
        return TX_OK;
    }
    return 0xff;//其他原因发送失败
}
/**
  * @brief   启动NRF24L01接收一次数据
  * @param   rxbuf:待接收数据首地址
  * @retval  0，接收完成；其他，错误代码
 **/
uint8_t NRF24L01_RxPacket(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t *rxbuf)
{
    uint8_t sta;
    sta=NRF24L01_Read_Reg(p_nrf24L01_operations,STATUS);  //读取状态寄存器的值
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
    if(sta&RX_OK)//接收到数据
    {
        NRF24L01_Read_Buf(p_nrf24L01_operations,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
        NRF24L01_Write_Reg(p_nrf24L01_operations,FLUSH_RX,0xff);//清除RX FIFO寄存器
        return 0;
    }
    return 1;//没收到任何数据
}

/**
  * @brief   检测24L01是否存在
  * @param   
  * @retval  0，成功;1，失败
 **/
uint8_t NRF24L01_Check(nrf24L01_oper_t *p_nrf24L01_operations)
{
    uint8_t i;
	uint8_t nrf24L01_check_buf[5]= {0XAA,0XAA,0XAA,0XAA,0XAA}; //写入5个0XAA字节
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+TX_ADDR,nrf24L01_check_buf,5);//写入5个字节的地址.
    NRF24L01_Read_Buf(p_nrf24L01_operations,TX_ADDR,nrf24L01_check_buf,5); //读出写入的地址
    for(i=0; i<5; i++)
        if(nrf24L01_check_buf[i]!=0XAA)
            break;
    if(i!=5)
		return 1;//检测24L01错误
    return 0;        //检测到24L01
}
