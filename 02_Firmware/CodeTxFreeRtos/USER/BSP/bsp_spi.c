#include "bsp_spi.h"

SPI_InitTypeDef  SPI2_InitStructure; //ȫ�ֱ���SPI�ṹ��

void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );	
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
    
	//ʱ���ź�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;       //����SPI����ģʽ:����Ϊ��SPI
    SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
    //��(CPOL=0)
    SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //���ݲ����ڵ�һ��ʱ����
    //��(CPHA=0)
    SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;      //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8
    //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��
    SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI2_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI2_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    SPI_Cmd(SPI2,ENABLE);//ʹ��SPI1

}

//ͨ��SPI2���ͻ��߽���һ���ֽڵ�����
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
    uint8_t retry=0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //��鷢�ͻ���ձ�־λ�������
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //ͨ������SPI1����һ������
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//�����ܻ���ǿձ�־λ�������
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPI1������յ�����
}