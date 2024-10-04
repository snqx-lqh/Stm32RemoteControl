#include "nrf24l01.h"



void NRF24L01_Init(void)
{
    //CE�͵�ƽ ����ģʽ
    RF24L01_SET_CE_LOW();  //ʹ��24L01
    //CSNΪ�͵�ƽоƬ����
    RF24L01_SET_CS_HIGH(); //SPIƬѡȡ��
	HAL_Delay(10);
	NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
	//HAL_Delay(10);
}

/**
 * @brief       SPI1��дһ���ֽ�����
 * @param       txdata  : Ҫ���͵�����(1�ֽ�)
 * @retval      ���յ�������(1�ֽ�)
 */
uint8_t SPI1_ReadWriteByte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 0xFFFF);
    return rxdata; /* �����յ������� */
}


//SPIд�Ĵ���
//��ڲ���: reg:ָ���Ĵ�����ַ; value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;
    RF24L01_SET_CS_LOW();                 //ʹ��SPI����
    status =SPI1_ReadWriteByte(reg);//���ͼĴ�����
    SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
    RF24L01_SET_CS_HIGH();                 //��ֹSPI����
    return(status);                 //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//��ڲ���:reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    RF24L01_SET_CS_LOW();          //ʹ��SPI����
    SPI1_ReadWriteByte(reg);   //���ͼĴ�����
    reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
    RF24L01_SET_CS_HIGH();          //��ֹSPI����
    return(reg_val);           //����״ֵ̬
}

//��ָ��λ�ö���ָ�����ȵ�����
//��ڲ���:reg:�Ĵ���(λ��); *pBuf:����ָ��; len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,u8_ctr;
    RF24L01_SET_CS_LOW();           //ʹ��SPI����
    status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
    RF24L01_SET_CS_HIGH();       //�ر�SPI����
    return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//��ڲ���:reg:�Ĵ���(λ��); *pBuf:����ָ��; len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,u8_ctr;
    RF24L01_SET_CS_LOW();          //ʹ��SPI����
    status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        SPI1_ReadWriteByte(*pBuf++); //д������
    RF24L01_SET_CS_HIGH();       //�ر�SPI����
    return status;          //���ض�����״ֵ̬
}

const u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //���յ�ַ
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
void NRF24L01_RX_Mode(void)
{
    RF24L01_SET_CE_LOW();
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
    //дRX�ڵ��ַ
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);           //����RFͨ��Ƶ��
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
    //��32���ֽ�
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);       //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
    //��CONFIG(00)д��0000 1111;PRIM_RX=1 ����ģʽ; PWR_UP=1 �ϵ�; CRCO=1 16λCRCУ��; EN_CRC =1 CRCʹ��
    RF24L01_SET_CE_HIGH(); //CEΪ��,�������ģʽ
}
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,����������.
void NRF24L01_TX_Mode(void)
{
    RF24L01_SET_CE_LOW();
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);
    //дTX�ڵ��ַ
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
    //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ
    NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
    //��CONFIG(00)д��0000 1111;PRIM_RX=0 ����ģʽ; PWR_UP=1 �ϵ�; CRCO=1 16λCRCУ��; EN_CRC   =1 CRCʹ��
    RF24L01_SET_CE_HIGH();//CEΪ��,10us����������
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    RF24L01_SET_CE_LOW();
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
    RF24L01_SET_CE_HIGH();//��������
    while(RF24L01_GET_IRQ_STATUS()!=0);//�ȴ��������
    sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
    if(sta&MAX_TX)//�ﵽ����ط�����
    {
        NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ���
        return MAX_TX;
    }
    if(sta&TX_OK)//�������
    {
        return TX_OK;
    }
    return 0xff;//����ԭ����ʧ��
}

/**
  * @brief :NRF24L01��״̬�Ĵ���
  * @param :��
  * @note  :��
  * @retval:RF24L01״̬
  */
uint8_t NRF24L01_Read_Status_Register( void )
{
    uint8_t Status;
	
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    Status = SPI1_ReadWriteByte( NRF_READ_REG + STATUS );	// ��״̬�Ĵ���
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
	
    return Status;
}

/**
  * @brief :NRF24L01���ж�
  * @param :
           @IRQ_Source:�ж�Դ
  * @note  :��
  * @retval:�����״̬�Ĵ�����ֵ
  */
uint8_t NRF24L01_Clear_IRQ_Flag( uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	// �жϱ�־����
    btmp = NRF24L01_Read_Status_Register( );			//��״̬�Ĵ���
			
    RF24L01_SET_CS_LOW( );			// Ƭѡ
    SPI1_ReadWriteByte( NRF_WRITE_REG + STATUS );	// д״̬�Ĵ�������
    SPI1_ReadWriteByte( IRQ_Source | btmp );		// ����Ӧ�жϱ�־
    RF24L01_SET_CS_HIGH( );			// ȡ��Ƭѡ
	
    return ( NRF24L01_Read_Status_Register( ));			// ����״̬�Ĵ���״̬
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
    u8 sta;
    sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
    if(sta&RX_OK)//���յ�����
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
        NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ���
        return 0;
    }
    return 1;//û�յ��κ�����
}

//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��
u8 buf[5]= {0XAA,0XAA,0XAA,0XAA,0XAA}; //д��5��0XAA�ֽ�
u8 NRF24L01_Check(void)
{
    u8 i;
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.
    NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ
    for(i=0; i<5; i++)
        if(buf[i]!=0XAA)
            break;
    if(i!=5)return 1;//���24L01����
    return 0;        //��⵽24L01
}
