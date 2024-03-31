#include "nrf24L01_driver.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //���յ�ַ

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
  * @brief   �ú�����ʼ��NRF24L01��RXģʽ
			 ����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
			 ��CE��ߺ�,������RXģʽ,�����Խ���������
  * @param   None
  * @retval
 **/
void NRF24L01_RX_Mode(nrf24L01_oper_t *p_nrf24L01_operations)
{
    p_nrf24L01_operations->set_ce_level(0);
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);
    //дRX�ڵ��ַ
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_CH,40);           //����RFͨ��Ƶ��
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
    //��32���ֽ�
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+CONFIG, 0x0f);       //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
    //��CONFIG(00)д��0000 1111;PRIM_RX=1 ����ģʽ; PWR_UP=1 �ϵ�; CRCO=1 16λCRCУ��; EN_CRC =1 CRCʹ��
    p_nrf24L01_operations->set_ce_level(1); //CEΪ��,�������ģʽ
}

/**
  * @brief   �ú�����ʼ��NRF24L01��TXģʽ
			 ����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
			 PWR_UP,CRCʹ��
			 ��CE��ߺ�,������RXģʽ,�����Խ���������
			 CEΪ�ߴ���10us,����������.
  * @param   None
  * @retval
 **/
void NRF24L01_TX_Mode(nrf24L01_oper_t *p_nrf24L01_operations)
{
    p_nrf24L01_operations->set_ce_level(0);
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);
    //дTX�ڵ��ַ
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);
    //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
    //��CONFIG(00)д��0000 1111;PRIM_RX=0 ����ģʽ; PWR_UP=1 �ϵ�; CRCO=1 16λCRCУ��; EN_CRC   =1 CRCʹ��
    p_nrf24L01_operations->set_ce_level(1);//CEΪ��,10us����������
}

/**
  * @brief   ����NRF24L01����һ������
  * @param   txbuf:�����������׵�ַ
  * @retval  �������״��
 **/
uint8_t NRF24L01_TxPacket(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t *txbuf)
{
    uint8_t sta;
	uint8_t read_irq_data;
    p_nrf24L01_operations->set_ce_level(0);
    NRF24L01_Write_Buf(p_nrf24L01_operations,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
    p_nrf24L01_operations->set_ce_level(1);//��������
	do{
		read_irq_data = p_nrf24L01_operations->read_irq_data();
    }while(read_irq_data !=0 );//�ȴ��������
    sta=NRF24L01_Read_Reg(p_nrf24L01_operations,STATUS);  //��ȡ״̬�Ĵ�����ֵ
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
    if(sta&MAX_TX)//�ﵽ����ط�����
    {
        NRF24L01_Write_Reg(p_nrf24L01_operations,FLUSH_TX,0xff);//���TX FIFO�Ĵ���
        return MAX_TX;
    }
    if(sta&TX_OK)//�������
    {
        return TX_OK;
    }
    return 0xff;//����ԭ����ʧ��
}
/**
  * @brief   ����NRF24L01����һ������
  * @param   rxbuf:�����������׵�ַ
  * @retval  0��������ɣ��������������
 **/
uint8_t NRF24L01_RxPacket(nrf24L01_oper_t *p_nrf24L01_operations,uint8_t *rxbuf)
{
    uint8_t sta;
    sta=NRF24L01_Read_Reg(p_nrf24L01_operations,STATUS);  //��ȡ״̬�Ĵ�����ֵ
    NRF24L01_Write_Reg(p_nrf24L01_operations,NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
    if(sta&RX_OK)//���յ�����
    {
        NRF24L01_Read_Buf(p_nrf24L01_operations,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
        NRF24L01_Write_Reg(p_nrf24L01_operations,FLUSH_RX,0xff);//���RX FIFO�Ĵ���
        return 0;
    }
    return 1;//û�յ��κ�����
}

/**
  * @brief   ���24L01�Ƿ����
  * @param   
  * @retval  0���ɹ�;1��ʧ��
 **/
uint8_t NRF24L01_Check(nrf24L01_oper_t *p_nrf24L01_operations)
{
    uint8_t i;
	uint8_t nrf24L01_check_buf[5]= {0XAA,0XAA,0XAA,0XAA,0XAA}; //д��5��0XAA�ֽ�
    NRF24L01_Write_Buf(p_nrf24L01_operations,NRF_WRITE_REG+TX_ADDR,nrf24L01_check_buf,5);//д��5���ֽڵĵ�ַ.
    NRF24L01_Read_Buf(p_nrf24L01_operations,TX_ADDR,nrf24L01_check_buf,5); //����д��ĵ�ַ
    for(i=0; i<5; i++)
        if(nrf24L01_check_buf[i]!=0XAA)
            break;
    if(i!=5)
		return 1;//���24L01����
    return 0;        //��⵽24L01
}
