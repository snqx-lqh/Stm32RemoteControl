#include "AnoPTv8Run.h"
#include "AnoPTv8DataDef.h"
#include "AnoPTv8Par.h"
#include "AnoPTv8Cmd.h"

//���ջ�����
_recvST recvBuf;
//���ͻ�����
_sendST AnoPTv8TxBuf[SENDBUFLEN];

_st_frame_v8 AnoPTv8CurrentAnlFrame;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
1��������ִ�к�������������Ƿ���������Ҫ�����Լ�������Ҫ���ڼ��Ĺ���
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8TxRunThread1ms(void)
{
    static uint16_t lastIndex = 0;

    for(int i=0; i<SENDBUFLEN; i++)
    {
        if(AnoPTv8TxBuf[lastIndex].used == 1 && AnoPTv8TxBuf[lastIndex].readyToSend == 1)
        {
            //
            AnoPTv8HwSendBytes(AnoPTv8TxBuf[lastIndex].dataBuf.rawBytes, AnoPTv8TxBuf[lastIndex].dataBuf.frame.datalen+ANOPTV8_FRAME_HEADLEN+2);
            AnoPTv8TxBuf[lastIndex].used = 0;
            AnoPTv8TxBuf[lastIndex].readyToSend = 0;

            break;
        }
        else
        {
            lastIndex++;
            while(lastIndex >= SENDBUFLEN)
                lastIndex -= SENDBUFLEN;
        }
    }

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
��ȡ��һ�����еķ��ͻ�����
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int AnoPTv8GetFreeTxBufIndex(void)
{
    static uint16_t lastIndex = 0;
    int ret = -1;

    for(int i=0; i<SENDBUFLEN; i++)
    {
        if(AnoPTv8TxBuf[lastIndex].used == 0)
        {
            AnoPTv8TxBuf[lastIndex].used = 1;
            ret = lastIndex;
            return ret;
        }
        else
        {
            lastIndex++;
            while(lastIndex >= SENDBUFLEN)
                lastIndex -= SENDBUFLEN;
        }

    }
    return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
�Խ��յ���֡����У��
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int8_t anoPTv8FrameCheck(const _un_frame_v8 *p)
{
    uint8_t sumcheck = 0, addcheck = 0;
    if (p->frame.head != ANOPTV8_FRAME_HEAD)
        return 0;
    for (uint16_t i = 0; i < (ANOPTV8_FRAME_HEADLEN + p->frame.datalen); i++)
    {
        sumcheck += p->rawBytes[i];
        addcheck += sumcheck;
    }
    if (sumcheck == p->frame.sumcheck && addcheck == p->frame.addcheck)
        return 1;
    else
        return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
��У��ͨ��������֡���н�����ִ����Ӧ�Ĺ���
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void anoPTv8FrameAnl(const _un_frame_v8 *p)
{
    if(p->frame.ddevid != ANOPTV8_MYDEVID && p->frame.ddevid != ANOPTV8DEVID_ALL)
        return;

    AnoPTv8CurrentAnlFrame = p->frame;
    //����֡IDִ�в�ͬ�Ĺ���
    switch (p->frame.frameid)
    {
    case 0xC0:
    case 0xC1:
        //�������֡
        AnoPTv8CmdFrameAnl(p);
        break;
    case 0xE0:
    case 0xE1:
        //������д���֡
        AnoPTv8ParFrameAnl(p);
        break;
    case 0xF0:
    {
        // AnoIapGetFrame(&p->frame);
        // AnoDTSendStr("anoDTFrameAnl", 1);
    }
    break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
���ڵ�ͨ�ŷ�ʽ�£�ÿ�յ�1�ֽ����ݣ����ñ�����һ�Σ������յ������ݴ���������ݽ���
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8RecvOneByte(uint8_t dat)
{
    if (recvBuf.recvSta == 0)
    {
        //��һ��������֡ͷ
        if (dat == ANOPTV8_FRAME_HEAD)
        {
            recvBuf.recvSta = 1;
            recvBuf.recvDataLen = 1;
            recvBuf.dataBuf.rawBytes[0] = dat;
        }
    }
    else if (recvBuf.recvSta == 1)
    {
        //�ڶ��������յ�ַ�ֽڡ�֡ID��֡���ȵ�����
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        if (recvBuf.recvDataLen >= ANOPTV8_FRAME_HEADLEN)
        {
            //������ݳ��ȳ��������������ݳ��ȣ����¿�ʼ����
            if(recvBuf.dataBuf.frame.datalen >= ANOPTV8_FRAME_MAXDATALEN)
                recvBuf.recvSta = 0;
            else
                recvBuf.recvSta = 2;
        }
    }
    else if (recvBuf.recvSta == 2)
    {
        //��������������������
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        if (recvBuf.recvDataLen >= ANOPTV8_FRAME_HEADLEN + recvBuf.dataBuf.frame.datalen)
            recvBuf.recvSta = 3;
    }
    else if (recvBuf.recvSta == 3)
    {
        //���Ĳ������պ�У���ֽ�
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        recvBuf.dataBuf.frame.sumcheck = dat;
        recvBuf.recvSta = 4;
    }
    else if (recvBuf.recvSta == 4)
    {
        //��5�������ո���У���ֽ�
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        recvBuf.dataBuf.frame.addcheck = dat;
        recvBuf.recvSta = 0;
        //һ֡���ݽ�����ϣ�����У�飬���У��ͨ�������֡����
        if (anoPTv8FrameCheck(&recvBuf.dataBuf))
            anoPTv8FrameAnl(&recvBuf.dataBuf);
    }
    else
    {
        recvBuf.recvSta = 0;
    }
}


