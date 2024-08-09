/**
  ******************************************************************************
  * @author  	Haozi
  * @version 	HAL��汾
  * @date    	2022.11.19
  * @brief   	2.4G ģ��ͨ��
  ******************************************************************************
  * @attention
  *		����CubeMX���ɵĹ��̣�����˵������
  * 		1. ������Ŷ��壨IRQ��CE��CS���� main.h �У�
  *			2. ���ų�ʼ���� gpio.c �У�
  * 		3. SPI ͨ�ų�ʼ���� spi.c �У�
  * 	���� SPI ͨ�ŵײ㣬CubeMX�Ѿ����ɹ��ˣ���stm32f1xx_hal_spi.c�У�����ֻ��Ҫ����Ӳ��ģ���������а�װ���ɡ�
  ******************************************************************************
  */

  
  
#include "nrf24l01.h"
//#include "drv_delay.h"

const char *g_ErrorString = "RF24L01 is not find !...";


/**
  * @brief :��װspi��д����
  * @param :
           @Addr:�Ĵ�����ַ
  * @note  :��ַ���豸����Ч
  * @retval:��ȡ������
  */
unsigned char spi_read_write_byte(unsigned char txdata)
{
	unsigned char rxdata;
	HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 0xFFFF);
	return(rxdata);
}

/**
  * @brief :NRF24L01���Ĵ���
  * @param :
           @Addr:�Ĵ�����ַ
  * @note  :��ַ���豸����Ч
  * @retval:��ȡ������
  */
uint8_t NRF24L01_Read_Reg( uint8_t RegAddr )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );			// Ƭѡ
	
	spi_read_write_byte( NRF_READ_REG | RegAddr );	// ������ ��ַ
	btmp = spi_read_write_byte( 0xFF );				// ������
	
    RF24L01_SET_CS_HIGH( );			// ȡ��Ƭѡ
	
    return btmp;
}

/**
  * @brief :NRF24L01��ָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:���ݴ�ŵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:��ȡ״̬
  */
void NRF24L01_Read_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );			// Ƭѡ
	
	spi_read_write_byte( NRF_READ_REG | RegAddr );		// ������ ��ַ
	for( btmp = 0; btmp < len; btmp ++ )
	{
		*( pBuf + btmp ) = spi_read_write_byte( 0xFF );	// ������
	}
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01д�Ĵ���
  * @param :��
  * @note  :��ַ���豸����Ч
  * @retval:��д״̬
  */
void NRF24L01_Write_Reg( uint8_t RegAddr, uint8_t Value )
{
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( NRF_WRITE_REG | RegAddr );	// д���� ��ַ
    spi_read_write_byte( Value );					// д����
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01дָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:д������ݵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:д״̬
  */
void NRF24L01_Write_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t i;
	
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( NRF_WRITE_REG | RegAddr );	// д���� ��ַ
    for( i = 0; i < len; i ++ )
    {
        spi_read_write_byte( *( pBuf + i ) );		// д����
    }
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

/**
  * @brief :���TX������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Flush_Tx_Fifo ( void )
{
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( FLUSH_TX );	// ��TX FIFO����
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

/**
  * @brief :���RX������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Flush_Rx_Fifo( void )
{
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( FLUSH_RX );	// ��RX FIFO����
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

/**
  * @brief :����ʹ����һ������
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Reuse_Tx_Payload( void )
{
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( REUSE_TX_PL );		// ����ʹ����һ������
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

/**
  * @brief :NRF24L01�ղ���
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Nop( void )
{
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( NOP );		// �ղ�������
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
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
	
    Status = spi_read_write_byte( NRF_READ_REG + STATUS );	// ��״̬�Ĵ���
	
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
    spi_read_write_byte( NRF_WRITE_REG + STATUS );	// д״̬�Ĵ�������
    spi_read_write_byte( IRQ_Source | btmp );		// ����Ӧ�жϱ�־
    RF24L01_SET_CS_HIGH( );			// ȡ��Ƭѡ
	
    return ( NRF24L01_Read_Status_Register( ));			// ����״̬�Ĵ���״̬
}

/**
  * @brief :��RF24L01�ж�״̬
  * @param :��
  * @note  :��
  * @retval:�ж�״̬
  */
uint8_t NRF24L01_Read_IRQ_Status( void )
{
    return ( NRF24L01_Read_Status_Register( ) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//�����ж�״̬
}
 
 /**
  * @brief :��FIFO�����ݿ��
  * @param :��
  * @note  :��
  * @retval:���ݿ��
  */
uint8_t NRF24L01_Read_Top_Fifo_Width( void )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );		// Ƭѡ
	
    spi_read_write_byte( R_RX_PL_WID );	// ��FIFO�����ݿ������
    btmp = spi_read_write_byte( 0xFF );	// ������
	
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
	
    return btmp;
}

 /**
  * @brief :�����յ�������
  * @param :��
  * @note  :��
  * @retval:
           @pRxBuf:���ݴ�ŵ�ַ�׵�ַ
  */
uint8_t NRF24L01_Read_Rx_Payload( uint8_t *pRxBuf )
{
    uint8_t Width, PipeNum;
	
    PipeNum = ( NRF24L01_Read_Reg( STATUS ) >> 1 ) & 0x07;	// ������״̬
    Width = NRF24L01_Read_Top_Fifo_Width( );				// ���������ݸ���

    RF24L01_SET_CS_LOW( );		// Ƭѡ
    spi_read_write_byte( RD_RX_PLOAD );						// ����Ч��������
	
    for( PipeNum = 0; PipeNum < Width; PipeNum ++ )
    {
        *( pRxBuf + PipeNum ) = spi_read_write_byte( 0xFF );// ������
    }
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
    NRF24L01_Flush_Rx_Fifo( );	// ���RX FIFO
	
    return Width;
}

 /**
  * @brief :�������ݣ���Ӧ��
  * @param :
  *			@pTxBuf:�������ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01_Write_Tx_Payload_Ack( uint8_t *pTxBuf, uint8_t len )
{
    uint8_t btmp;
    uint8_t length = ( len > 32 ) ? 32 : len;	// ���ݳ����Լ32 ��ֻ����32��

    NRF24L01_Flush_Tx_Fifo( );			// ��TX FIFO
	
    RF24L01_SET_CS_LOW( );				// Ƭѡ
    spi_read_write_byte( WR_TX_PLOAD );	// ��������
	
    for( btmp = 0; btmp < length; btmp ++ )
    {
        spi_read_write_byte( *( pTxBuf + btmp ) );	// ��������
    }
    RF24L01_SET_CS_HIGH( );			// ȡ��Ƭѡ
}

 /**
  * @brief :�������ݣ�����Ӧ��
  * @param :
  *			@pTxBuf:�������ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01_Write_Tx_Payload_NoAck( uint8_t *pTxBuf, uint8_t len )
{
    if( len > 32 || len == 0 ) return ;	// ���ݳ��ȴ���32 ���ߵ���0 ��ִ��

    RF24L01_SET_CS_LOW( );		// Ƭѡ
    spi_read_write_byte( WR_TX_PLOAD_NACK );	// ��������
    while( len-- )
    {
        spi_read_write_byte( *pTxBuf );			// ��������
		pTxBuf++;
    }
    RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

 /**
  * @brief :�ڽ���ģʽ����TX FIFOд����(��ACK)
  * @param :
  *			@pData:���ݵ�ַ
  *			@len:����
  * @note  :һ�β�����32���ֽ�
  * @retval:��
  */
void NRF24L01_Write_Tx_Payload_InAck( uint8_t *pData, uint8_t len )
{
    uint8_t btmp;
	
	len = ( len > 32 ) ? 32 : len;		// ���ݳ��ȴ���32����ֻд32���ֽ�

    RF24L01_SET_CS_LOW( );			// Ƭѡ
    spi_read_write_byte( W_ACK_PLOAD );		// ����
    for( btmp = 0; btmp < len; btmp ++ )
    {
        spi_read_write_byte( *( pData + btmp ) );	// д����
    }
    RF24L01_SET_CS_HIGH( );			// ȡ��Ƭѡ
}

 /**
  * @brief :���÷��͵�ַ
  * @param :
  *			@pAddr:��ַ��ŵ�ַ
  *			@len:����
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_TxAddr( uint8_t *pAddr, uint8_t len )
{
	len = ( len > 5 ) ? 5 : len;				// ��ַ���ܴ���5���ֽ�
    NRF24L01_Write_Buf( TX_ADDR, pAddr, len );	// д��ַ
}

 /**
  * @brief :���ý���ͨ����ַ
  * @param :
  *			@PipeNum:ͨ��
  *			@pAddr:��ַ����ŵ�ַ
  *			@Len:����
  * @note  :ͨ��������5 ��ַ���Ȳ�����5���ֽ�
  * @retval:��
  */
void NRF24L01_Set_RxAddr( uint8_t PipeNum, uint8_t *pAddr, uint8_t Len )
{
    Len = ( Len > 5 ) ? 5 : Len;					// ��ַ���Ȳ�����5���ֽ�
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		// ͨ��������5 

    NRF24L01_Write_Buf( RX_ADDR_P0 + PipeNum, pAddr, Len );	// д���ַ
}

 /**
  * @brief :����ͨ���ٶ�
  * @param :
  *			@Speed:�ٶ�
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_Speed( nRf24l01SpeedType Speed )
{
	uint8_t btmp = 0;
	
	btmp = NRF24L01_Read_Reg( RF_SETUP );
	btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	
	if( Speed == SPEED_250K )		// 250K
	{
		btmp |= ( 1<<5 );
	}
	else if( Speed == SPEED_1M )   // 1M
	{
   		btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	}
	else if( Speed == SPEED_2M )   // 2M
	{
		btmp |= ( 1<<3 );
	}

	NRF24L01_Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :���ù���
  * @param :
  *			@Speed:�ٶ�
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_Power( nRf24l01PowerType Power )
{
    uint8_t btmp;
	
	btmp = NRF24L01_Read_Reg( RF_SETUP ) & ~0x07;
    switch( Power )
    {
        case POWER_F18DBM:
            btmp |= PWR_18DB;
            break;
        case POWER_F12DBM:
            btmp |= PWR_12DB;
            break;
        case POWER_F6DBM:
            btmp |= PWR_6DB;
            break;
        case POWER_0DBM:
            btmp |= PWR_0DB;
            break;
        default:
            break;
    }
    NRF24L01_Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :����Ƶ��
  * @param :
  *			@FreqPoint:Ƶ�����ò���
  * @note  :ֵ������127
  * @retval:��
  */
void NRF24LL01_Write_Hopping_Point( uint8_t FreqPoint )
{
    NRF24L01_Write_Reg( RF_CH, FreqPoint & 0x7F );
}

/**
  * @brief :NRF24L01���
  * @param :��
  * @note  :��
  * @retval:��
  */ 
uint8_t read_buf[ 5 ] = { 0 };
uint8_t NRF24L01_Check( void )
{
	uint8_t i;
	uint8_t buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	
	 
	while( 1 )
	{
		NRF24L01_Write_Buf( TX_ADDR, buf, 5 );			//д��5���ֽڵĵ�ַ
		NRF24L01_Read_Buf( TX_ADDR, read_buf, 5 );		//����д��ĵ�ַ  
		for( i = 0; i < 5; i++ )
		{
			if( buf[ i ] != read_buf[ i ] )
			{
				break;
			}	
		} 
		
		if( 5 == i )
		{
			return 0;
			//printf("NRF24L01 is find !... \r\n");
			//break;
		}
		else
		{
			return 1;
			// ������ʾ
			//printf("NRF24L01 is not find !... \r\n");
		}
//		HAL_Delay(2000);
	}
}

 /**
  * @brief :����ģʽ
  * @param :
  *			@Mode:����ģʽ�����ģʽ
  * @note  :��
  * @retval:��
  */
void NRF24L01_Set_Mode( nRf24l01ModeType Mode )
{
    uint8_t controlreg = 0;
	controlreg = NRF24L01_Read_Reg( CONFIG );
	
    if( Mode == MODE_TX )
	{
		controlreg &= ~( 1<< PRIM_RX );
	}
    else 
	{
		if( Mode == MODE_RX )  
		{ 
			controlreg |= ( 1<< PRIM_RX ); 
		}
	}

    NRF24L01_Write_Reg( CONFIG, controlreg );
}

/**
  * @brief :NRF24L01����һ������
  * @param :
  *			@txbuf:�����������׵�ַ
  *			@Length:�������ݳ���
  * @note  :��
  * @retval:
  *			MAX_TX���ﵽ����ط�����
  *			TX_OK���������
  *			0xFF:����ԭ��
  */ 
uint8_t NRF24L01_TxPacket( uint8_t *txbuf, uint8_t Length )
{
	uint8_t l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		// Ƭѡ
	spi_read_write_byte( FLUSH_TX );
	RF24L01_SET_CS_HIGH( );
	
	RF24L01_SET_CE_LOW( );		
	NRF24L01_Write_Buf( WR_TX_PLOAD, txbuf, Length );	// д���ݵ�TX BUF 32�ֽ�  TX_PLOAD_WIDTH
	RF24L01_SET_CE_HIGH( );			// ��������
	while( 0 != RF24L01_GET_IRQ_STATUS())
	{
		HAL_Delay(1);
		if( 500 == l_MsTimes++ )						// 500ms��û�з��ͳɹ������³�ʼ���豸
		{
			NRF24L01_Gpio_Init( );
			NRF24L01_Init( );
			NRF24L01_Set_Mode( MODE_TX );
			break;
		}
	}
	l_Status = NRF24L01_Read_Reg(STATUS);						// ��״̬�Ĵ���
	NRF24L01_Write_Reg( STATUS, l_Status );						// ���TX_DS��MAX_RT�жϱ�־
	
	if( l_Status & MAX_TX )	// �ﵽ����ط�����
	{
		NRF24L01_Write_Reg( FLUSH_TX, 0xff );	// ���TX FIFO�Ĵ���
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	// �������
	{
		return TX_OK;
	}
	
	return 0xFF;	// ����ԭ����ʧ��
}

/**
  * @brief :NRF24L01��������
  * @param :
  *			@rxbuf:�������ݴ�ŵ�ַ
  * @note  :��
  * @retval:���յ����ݸ���
  */ 
uint8_t NRF24L01_RxPacket( uint8_t *rxbuf )
{
	uint8_t l_Status = 0, l_RxLength = 0, l_100MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		// Ƭѡ
	spi_read_write_byte( FLUSH_RX );
	RF24L01_SET_CS_HIGH( );
	
	while( 0 != RF24L01_GET_IRQ_STATUS())
	{
		HAL_Delay(100);
		
		if( 30 == l_100MsTimes++ )		// 3sû���չ����ݣ����³�ʼ��ģ��
		{
			NRF24L01_Gpio_Init( );
			NRF24L01_Init( );
			NRF24L01_Set_Mode( MODE_RX );
			break;
		}
	}
	
	l_Status = NRF24L01_Read_Reg( STATUS );		// ��״̬�Ĵ���
	NRF24L01_Write_Reg( STATUS, l_Status );		// ���жϱ�־
	if( l_Status & RX_OK)	// ���յ�����
	{
		l_RxLength = NRF24L01_Read_Reg( R_RX_PL_WID );		// ��ȡ���յ������ݸ���
		NRF24L01_Read_Buf( RD_RX_PLOAD, rxbuf, l_RxLength );// ���յ����� 
		NRF24L01_Write_Reg( FLUSH_RX, 0xff );				// ���RX FIFO
		return l_RxLength; 
	}
	
	return 0;				//û���յ�����	
}

 /**
  * @brief :RF24L01���ų�ʼ��
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Gpio_Init( void )
{
	RF24L01_SET_CE_LOW( );		// ����
	RF24L01_SET_CS_HIGH( );		// ȡ��Ƭѡ
}

 /**
  * @brief :RF24L01ģ���ʼ��
  * @param :��
  * @note  :��
  * @retval:��
  */
void NRF24L01_Init( void )
{
    uint8_t addr[5] = {INIT_ADDR};

    RF24L01_SET_CE_HIGH( );
    NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
#if DYNAMIC_PACKET == 1

    NRF24L01_Write_Reg( DYNPD, ( 1 << 0 ) ); 	//ʹ��ͨ��1��̬���ݳ���
    NRF24L01_Write_Reg( FEATRUE, 0x07 );
    NRF24L01_Read_Reg( DYNPD );
    NRF24L01_Read_Reg( FEATRUE );
	
#elif DYNAMIC_PACKET == 0
    
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN );// �̶����ݳ���
	
#endif	// DYNAMIC_PACKET

    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/		// �����ж�
                                      ( 1 << EN_CRC ) |     // ʹ��CRC 1���ֽ�
                                      ( 1 << PWR_UP ) );    // �����豸
    NRF24L01_Write_Reg( EN_AA, ( 1 << ENAA_P0 ) );   		// ͨ��0�Զ�Ӧ��
    NRF24L01_Write_Reg( EN_RXADDR, ( 1 << ERX_P0 ) );		// ͨ��0����
    NRF24L01_Write_Reg( SETUP_AW, AW_5BYTES );     			// ��ַ��� 5���ֽ�
    NRF24L01_Write_Reg( SETUP_RETR, ARD_4000US |
                        ( REPEAT_CNT & 0x0F ) );         	// �ظ��ȴ�ʱ�� 250us
    NRF24L01_Write_Reg( RF_CH, 60 );             			// ��ʼ��ͨ��
    NRF24L01_Write_Reg( RF_SETUP, 0x26 );

    NRF24L01_Set_TxAddr( &addr[0], 5 );                      // ����TX��ַ
    NRF24L01_Set_RxAddr( 0, &addr[0], 5 );                   // ����RX��ַ
}
