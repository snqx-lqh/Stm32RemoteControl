#include "ssd1306_oled_driver.h"

#include "oledfont.h"

uint8_t OLED_GRAM[8][128];

void OLED_Write_Byte(ssd1306_oled_oper_t *p_ssd1306_oled_oper,unsigned char data,unsigned char cmd)
{
	if(cmd) {
		if(p_ssd1306_oled_oper->iic_or_spi == IIC_MODE)
			p_ssd1306_oled_oper->iic_write_one_byte(0x40,data);
		else if(p_ssd1306_oled_oper->iic_or_spi == SPI_MODE)
		{
			p_ssd1306_oled_oper->set_dc_level(1);
			p_ssd1306_oled_oper->set_cs_level(0);
			p_ssd1306_oled_oper->spi_write_one_byte(data);
			p_ssd1306_oled_oper->set_dc_level(1);
			p_ssd1306_oled_oper->set_cs_level(1);
		}
    }
    else {
        if(p_ssd1306_oled_oper->iic_or_spi == IIC_MODE)
			p_ssd1306_oled_oper->iic_write_one_byte(0x00,data);
		else if(p_ssd1306_oled_oper->iic_or_spi == SPI_MODE)
		{
			p_ssd1306_oled_oper->set_dc_level(0);
			p_ssd1306_oled_oper->set_cs_level(0);
			p_ssd1306_oled_oper->spi_write_one_byte(data);
			p_ssd1306_oled_oper->set_dc_level(1);
			p_ssd1306_oled_oper->set_cs_level(1);
		}
    }
}

void OLED_Write_Data_Bytes(ssd1306_oled_oper_t *p_ssd1306_oled_oper,unsigned char *data,unsigned int len)
{
	if(p_ssd1306_oled_oper->iic_or_spi == IIC_MODE)
		p_ssd1306_oled_oper->iic_write_len_byte(0x40,data,len);
	else if(p_ssd1306_oled_oper->iic_or_spi == SPI_MODE)
	{
		p_ssd1306_oled_oper->set_dc_level(1);
		p_ssd1306_oled_oper->set_cs_level(0);
		p_ssd1306_oled_oper->spi_write_len_byte(data,len);
		p_ssd1306_oled_oper->set_dc_level(1);
		p_ssd1306_oled_oper->set_cs_level(1);
	}
}

//���Ժ���
void OLED_ColorTurn(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t i)
{
    if(i==0)
    {
        OLED_Write_Byte(p_ssd1306_oled_oper,0xA6,OLED_CMD);//������ʾ
    }
    if(i==1)
    {
        OLED_Write_Byte(p_ssd1306_oled_oper,0xA7,OLED_CMD);//��ɫ��ʾ
    }
}

//��Ļ��ת180��
void OLED_DisplayTurn(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t i)
{
    if(i==0)
    {
        OLED_Write_Byte(p_ssd1306_oled_oper,0xC8,OLED_CMD);//������ʾ
        OLED_Write_Byte(p_ssd1306_oled_oper,0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_Write_Byte(p_ssd1306_oled_oper,0xC0,OLED_CMD);//��ת��ʾ
        OLED_Write_Byte(p_ssd1306_oled_oper,0xA0,OLED_CMD);
    }
}



//����OLED��ʾ
void OLED_DisPlay_On(ssd1306_oled_oper_t *p_ssd1306_oled_oper)
{
    OLED_Write_Byte(p_ssd1306_oled_oper,0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_Write_Byte(p_ssd1306_oled_oper,0x14,OLED_CMD);//������ɱ�
    OLED_Write_Byte(p_ssd1306_oled_oper,0xAF,OLED_CMD);//������Ļ
}

//�ر�OLED��ʾ
void OLED_DisPlay_Off(ssd1306_oled_oper_t *p_ssd1306_oled_oper)
{
    OLED_Write_Byte(p_ssd1306_oled_oper,0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_Write_Byte(p_ssd1306_oled_oper,0x10,OLED_CMD);//�رյ�ɱ�
    OLED_Write_Byte(p_ssd1306_oled_oper,0xAE,OLED_CMD);//�ر���Ļ
}

//�����Դ浽OLED
void OLED_Refresh(ssd1306_oled_oper_t *p_ssd1306_oled_oper)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        OLED_Write_Byte(p_ssd1306_oled_oper,0xb0+i,OLED_CMD);   //��������ʼ��ַ
        OLED_Write_Byte(p_ssd1306_oled_oper,0x00,OLED_CMD);     //���õ�����ʼ��ַ
        OLED_Write_Byte(p_ssd1306_oled_oper,0x10,OLED_CMD);     //���ø�����ʼ��ַ
		OLED_Write_Data_Bytes(p_ssd1306_oled_oper,OLED_GRAM[i],128);
    }
}
//����BUFF
void OLED_Clear_Buff(ssd1306_oled_oper_t *p_ssd1306_oled_oper)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        for(n=0; n<128; n++)
        {
            OLED_GRAM[i][n]=0;//�����������
        }
    }
}

//��������
void OLED_Clear(ssd1306_oled_oper_t *p_ssd1306_oled_oper)
{
    uint8_t i,n;
    for(i=0; i<8; i++)
    {
        for(n=0; n<128; n++)
        {
            OLED_GRAM[i][n]=0;//�����������
        }
    }
    OLED_Refresh(p_ssd1306_oled_oper);//������ʾ
}

//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t t)
{
    uint8_t i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    if(t) {
        OLED_GRAM[i][x]|=n;
    }
    else
    {
        OLED_GRAM[i][x]=~OLED_GRAM[i][x];
        OLED_GRAM[i][x]|=n;
        OLED_GRAM[i][x]=~OLED_GRAM[i][x];
    }
}

void OLED_Fill(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
	uint8_t i,j;
	for(i = x1; i< x2 ; i++)
	{
		for(j = y1; j< y2 ; j++)
		{
			OLED_DrawPoint(p_ssd1306_oled_oper,i,j,mode);
		}
	}
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED_DrawLine(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //������������
    delta_y=y2-y1;
    uRow=x1;//�����������
    uCol=y1;
    if(delta_x>0)incx=1; //���õ�������
    else if (delta_x==0)incx=0;//��ֱ��
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//ˮƽ��
    else {
        incy=-1;
        delta_y=-delta_x;
    }
    if(delta_x>delta_y)distance=delta_x; //ѡȡ��������������
    else distance=delta_y;
    for(t=0; t<distance+1; t++)
    {
        OLED_DrawPoint(p_ssd1306_oled_oper,uRow,uCol,mode);//����
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t r,uint8_t mode)
{
    int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(p_ssd1306_oled_oper,x + a, y - b,mode);
        OLED_DrawPoint(p_ssd1306_oled_oper,x - a, y - b,mode);
        OLED_DrawPoint(p_ssd1306_oled_oper,x - a, y + b,mode);
        OLED_DrawPoint(p_ssd1306_oled_oper,x + a, y + b,mode);

        OLED_DrawPoint(p_ssd1306_oled_oper,x + b, y + a,mode);
        OLED_DrawPoint(p_ssd1306_oled_oper,x + b, y - a,mode);
        OLED_DrawPoint(p_ssd1306_oled_oper,x - b, y - a,mode);
        OLED_DrawPoint(p_ssd1306_oled_oper,x - b, y + a,mode);

        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChar(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
    uint8_t i,m,temp,size2,chr1;
    uint8_t x0=x,y0=y;
    if(size1==8)size2=6;
    else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr1=chr-' ';  //����ƫ�ƺ��ֵ
    for(i=0; i<size2; i++)
    {
        if(size1==8)
        {
            temp=asc2_0806[chr1][i];   //����0806����
        }
        else if(size1==12)
        {
            temp=asc2_1206[chr1][i];   //����1206����
        }
        else if(size1==16)
        {
            temp=asc2_1608[chr1][i];   //����1608����
        }
        else if(size1==24)
        {
            temp=asc2_2412[chr1][i];   //����2412����
        }
        else return;
        for(m=0; m<8; m++)
        {
            if(temp&0x01)OLED_DrawPoint(p_ssd1306_oled_oper,x,y,mode);
            else OLED_DrawPoint(p_ssd1306_oled_oper,x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((size1!=8)&&((x-x0)==size1/2))
        {
            x=x0;
            y0=y0+8;
        }
        y=y0;
    }
}


//��ʾ�ַ���
//x,y:�������
//size1:�����С
//*chr:�ַ�����ʼ��ַ
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowString(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode)
{
    while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
    {
        OLED_ShowChar(p_ssd1306_oled_oper,x,y,*chr,size1,mode);
        if(size1==8)x+=6;
        else x+=size1/2;
        chr++;
    }
}

//m^n
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
    {
        result*=m;
    }
    return result;
}

//��ʾ����
//x,y :�������
//num :Ҫ��ʾ������
//len :���ֵ�λ��
//size:�����С
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowNum(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
    uint8_t t,temp,m=0;
    if(size1==8)m=2;
    for(t=0; t<len; t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(temp==0)
        {
            OLED_ShowChar(p_ssd1306_oled_oper,x+(size1/2+m)*t,y,'0',size1,mode);
        }
        else
        {
            OLED_ShowChar(p_ssd1306_oled_oper,x+(size1/2+m)*t,y,temp+'0',size1,mode);
        }
    }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChinese(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode)
{
    uint8_t m,temp;
    uint8_t x0=x,y0=y;
    uint16_t i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    for(i=0; i<size3; i++)
    {
        if(size1==16)
        {
            temp=Hzk1[num][i];   //����16*16����
        }
        else if(size1==24)
        {
            temp=Hzk2[num][i];   //����24*24����
        }
        else if(size1==32)
        {
            temp=Hzk3[num][i];   //����32*32����
        }
        else if(size1==64)
        {
            temp=Hzk4[num][i];   //����64*64����
        }
        else return;
        for(m=0; m<8; m++)
        {
            if(temp&0x01)OLED_DrawPoint(p_ssd1306_oled_oper,x,y,mode);
            else OLED_DrawPoint(p_ssd1306_oled_oper,x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((x-x0)==size1)
        {
            x=x0;
            y0=y0+8;
        }
        y=y0;
    }
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ScrollDisplay(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t num,uint8_t space,uint8_t mode)
{
    uint8_t i,n,t=0,m=0,r;
    while(1)
    {
        if(m==0)
        {
            OLED_ShowChinese(p_ssd1306_oled_oper,128,24,t,16,mode); //д��һ�����ֱ�����OLED_GRAM[][]������
            t++;
        }
        if(t==num)
        {
            for(r=0; r<16*space; r++)    //��ʾ���
            {
                for(i=1; i<144; i++)
                {
                    for(n=0; n<8; n++)
                    {
                        OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                    }
                }
                OLED_Refresh(p_ssd1306_oled_oper);
            }
            t=0;
        }
        m++;
        if(m==16) {
            m=0;
        }
        for(i=1; i<144; i++) //ʵ������
        {
            for(n=0; n<8; n++)
            {
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        OLED_Refresh(p_ssd1306_oled_oper);
    }
}

//x,y���������
//sizex,sizey,ͼƬ����
//BMP[]��Ҫд���ͼƬ����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowPicture(ssd1306_oled_oper_t *p_ssd1306_oled_oper,uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
    uint16_t j=0;
    uint8_t i,n,temp,m;
    uint8_t x0=x,y0=y;
    sizey=sizey/8+((sizey%8)?1:0);
    for(n=0; n<sizey; n++)
    {
        for(i=0; i<sizex; i++)
        {
            temp=BMP[j];
            j++;
            for(m=0; m<8; m++)
            {
                if(temp&0x01)OLED_DrawPoint(p_ssd1306_oled_oper,x,y,mode);
                else OLED_DrawPoint(p_ssd1306_oled_oper,x,y,!mode);
                temp>>=1;
                y++;
            }
            x++;
            if((x-x0)==sizex)
            {
                x=x0;
                y0=y0+8;
            }
            y=y0;
        }
    }
}
//OLED�ĳ�ʼ��
void OLED_Init(ssd1306_oled_oper_t *p_ssd1306_oled_oper)
{
	if(p_ssd1306_oled_oper->iic_or_spi == SPI_MODE)
	{
		p_ssd1306_oled_oper->set_res_level(0);
		p_ssd1306_oled_oper->delay_ms(100);
		p_ssd1306_oled_oper->set_res_level(1);
	}

    OLED_Write_Byte(p_ssd1306_oled_oper,0xAE,OLED_CMD);//--turn off oled panel
    OLED_Write_Byte(p_ssd1306_oled_oper,0x00,OLED_CMD);//---set low column address
    OLED_Write_Byte(p_ssd1306_oled_oper,0x10,OLED_CMD);//---set high column address
    OLED_Write_Byte(p_ssd1306_oled_oper,0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_Write_Byte(p_ssd1306_oled_oper,0x81,OLED_CMD);//--set contrast control register
    OLED_Write_Byte(p_ssd1306_oled_oper,0xCF,OLED_CMD);// Set SEG Output Current Brightness
    OLED_Write_Byte(p_ssd1306_oled_oper,0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_Write_Byte(p_ssd1306_oled_oper,0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_Write_Byte(p_ssd1306_oled_oper,0xA6,OLED_CMD);//--set normal display
    OLED_Write_Byte(p_ssd1306_oled_oper,0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_Write_Byte(p_ssd1306_oled_oper,0x3f,OLED_CMD);//--1/64 duty
    OLED_Write_Byte(p_ssd1306_oled_oper,0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_Write_Byte(p_ssd1306_oled_oper,0x00,OLED_CMD);//-not offset
    OLED_Write_Byte(p_ssd1306_oled_oper,0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_Write_Byte(p_ssd1306_oled_oper,0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_Write_Byte(p_ssd1306_oled_oper,0xD9,OLED_CMD);//--set pre-charge period
    OLED_Write_Byte(p_ssd1306_oled_oper,0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_Write_Byte(p_ssd1306_oled_oper,0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_Write_Byte(p_ssd1306_oled_oper,0x12,OLED_CMD);
    OLED_Write_Byte(p_ssd1306_oled_oper,0xDB,OLED_CMD);//--set vcomh
    OLED_Write_Byte(p_ssd1306_oled_oper,0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_Write_Byte(p_ssd1306_oled_oper,0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_Write_Byte(p_ssd1306_oled_oper,0x02,OLED_CMD);//
    OLED_Write_Byte(p_ssd1306_oled_oper,0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_Write_Byte(p_ssd1306_oled_oper,0x14,OLED_CMD);//--set(0x10) disable
    OLED_Write_Byte(p_ssd1306_oled_oper,0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_Write_Byte(p_ssd1306_oled_oper,0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    
	OLED_Clear(p_ssd1306_oled_oper);
    
	OLED_Write_Byte(p_ssd1306_oled_oper,0xAF,OLED_CMD);
}


