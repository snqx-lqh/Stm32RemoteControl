#include "nrf24l01.h"

SPI_InitTypeDef  SPI_InitStructure; //ȫ�ֱ���SPI�ṹ��

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
    
	//ʱ���ź�
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SCK_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRF24L01_SCK_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_SCK_GPIO,NRF24L01_SCK_GPIO_PIN);
	//MISO�ź�
	GPIO_InitStructure.GPIO_Pin = NRF24L01_MISO_GPIO_PIN;
	GPIO_Init(NRF24L01_MISO_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_MISO_GPIO,NRF24L01_MISO_GPIO_PIN);
	//MOSI�ź�
	GPIO_InitStructure.GPIO_Pin = NRF24L01_MOSI_GPIO_PIN;
	GPIO_Init(NRF24L01_MOSI_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_MOSI_GPIO,NRF24L01_MOSI_GPIO_PIN);
	//SPIƬѡ�ź�
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRF24L01_CSN_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(NRF24L01_CSN_GPIO,NRF24L01_CSN_GPIO_PIN);
	//24L01ģʽ�����ź�
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_GPIO_PIN;
    GPIO_Init(NRF24L01_CE_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_CE_GPIO,NRF24L01_CE_GPIO_PIN);
    //IRQ IRQ������������
    GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;   //��������
    GPIO_Init(NRF24L01_IRQ_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(NRF24L01_IRQ_GPIO,NRF24L01_IRQ_GPIO_PIN);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
    //��(CPOL=0)
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //���ݲ����ڵ�һ��ʱ����
    //��(CPHA=0)
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;      //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8
    //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    SPI_Cmd(SPI1,ENABLE);//ʹ��SPI1

    //CE�͵�ƽ ����ģʽ
    NRF24L01_CE=0;  //ʹ��24L01
    //CSNΪ�͵�ƽоƬ����
    NRF24L01_CSN=1; //SPIƬѡȡ��
}

//void Delay(vu32 nCount)
//{
//  for(; nCount != 0; nCount--);
//}

////ģ��SPI��д���ݺ���
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

//ͨ��SPI1���ͻ��߽���һ���ֽڵ�����
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 retry=0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //��鷢�ͻ���ձ�־λ�������
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI1, TxData); //ͨ������SPI1����һ������
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//�����ܻ���ǿձ�־λ�������
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPI1������յ�����
}

//SPIд�Ĵ���
//��ڲ���: reg:ָ���Ĵ�����ַ; value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
    u8 status;
    NRF24L01_CSN=0;                 //ʹ��SPI����
    status =SPI1_ReadWriteByte(reg);//���ͼĴ�����
    SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
    NRF24L01_CSN=1;                 //��ֹSPI����
    return(status);                 //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//��ڲ���:reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    NRF24L01_CSN = 0;          //ʹ��SPI����
    SPI1_ReadWriteByte(reg);   //���ͼĴ�����
    reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
    NRF24L01_CSN = 1;          //��ֹSPI����
    return(reg_val);           //����״ֵ̬
}

//��ָ��λ�ö���ָ�����ȵ�����
//��ڲ���:reg:�Ĵ���(λ��); *pBuf:����ָ��; len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
    u8 status,u8_ctr;
    NRF24L01_CSN = 0;           //ʹ��SPI����
    status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
    NRF24L01_CSN=1;       //�ر�SPI����
    return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//��ڲ���:reg:�Ĵ���(λ��); *pBuf:����ָ��; len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status,u8_ctr;
    NRF24L01_CSN = 0;          //ʹ��SPI����
    status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        SPI1_ReadWriteByte(*pBuf++); //д������
    NRF24L01_CSN = 1;       //�ر�SPI����
    return status;          //���ض�����״ֵ̬
}

const u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x11,0x22,0x33,0x44,0x55}; //���յ�ַ
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
void NRF24L01_RX_Mode(void)
{
    NRF24L01_CE=0;
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
    NRF24L01_CE = 1; //CEΪ��,�������ģʽ
}
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,����������.
void NRF24L01_TX_Mode(void)
{
    NRF24L01_CE=0;
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
    NRF24L01_CE=1;//CEΪ��,10us����������
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    NRF24L01_CE=0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
    NRF24L01_CE=1;//��������
    while(NRF24L01_IRQ!=0);//�ȴ��������
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
