#include "HWInterface.h"
#include "AnoPTv8Run.h"
#include "AnoPTv8Par.h"
//AptHwSendBytes�˺�����Ҫ�����û��Լ����豸������ʵ�֣�����ʹ�ô���������λ��������Ͷ�Ӧ�ô��ڵķ��ͺ���
//ע�⣺�����������ʹ���ж�+����������DMA+�������ķ�ʽ������ʽ���ͽ����Ӱ��ϵͳ����
//ע�⣺���ڻ�������Ӧ��С���Ƽ�256�ֽڻ�����
//���������ö�Ӧ�Ĵ���h�ļ�
#include "bsp_usart.h"

void AnoPTv8HwSendBytes(uint8_t *buf, uint16_t len)
{
    USART1_Send_Len_Data(buf, len);
}

//AptHwRecvByte�˺�������hardwareInterface.h���������û���Ҫ�ڶ�Ӧ���ڵĽ����¼��е��ô˺���
//ע�⣺�˺�����������ֽ����ݣ���������¼����յ������ݴ���1�ֽڣ���ε��ô˺�������
void AnoPTv8HwRecvByte(uint8_t dat)
{
    AnoPTv8RecvOneByte(dat);
}

//AptHwTrigger1ms�˺�������hardwareInterface.h���������û���Ҫ��1ms��ʱ�жϻ���ϵͳ�δ�����Լ���Ƶĵ�������
//��1ms��ʱ�������ô˺���
void AnoPTv8HwTrigger1ms(void)
{
    AnoPTv8TxRunThread1ms();
}

void AnoPTv8HwParValRecvCallback(uint16_t parid, int32_t parval)
{
    
}

void AnoPTv8HwParCmdRecvCallback(uint8_t id, uint16_t val)
{
    if(id == 0x10)
    {
        if(val == 0x0020)
        {
            //������ٶ�У׼����
            
        }
        else if(val == 0x0021)
        {
            //�յ����ٶ�У׼���ݣ��������棬���ò���
            
        }
    }
}

void AnoPTv8HwParCmdResetParameter(void)
{
    
}

