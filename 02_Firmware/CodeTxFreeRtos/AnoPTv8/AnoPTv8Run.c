#include "AnoPTv8Run.h"
#include "AnoPTv8DataDef.h"
#include "AnoPTv8Par.h"
#include "AnoPTv8Cmd.h"

//接收缓冲区
_recvST recvBuf;
//发送缓冲区
_sendST AnoPTv8TxBuf[SENDBUFLEN];

_st_frame_v8 AnoPTv8CurrentAnlFrame;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
1毫秒周期执行函数，用来检查是否有数据需要发送以及其他需要定期检查的功能
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
获取下一个空闲的发送缓冲区
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
对接收到的帧进行校验
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
对校验通过的数据帧进行解析，执行相应的功能
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void anoPTv8FrameAnl(const _un_frame_v8 *p)
{
    if(p->frame.ddevid != ANOPTV8_MYDEVID && p->frame.ddevid != ANOPTV8DEVID_ALL)
        return;

    AnoPTv8CurrentAnlFrame = p->frame;
    //根据帧ID执行不同的功能
    switch (p->frame.frameid)
    {
    case 0xC0:
    case 0xC1:
        //命令相关帧
        AnoPTv8CmdFrameAnl(p);
        break;
    case 0xE0:
    case 0xE1:
        //参数读写相关帧
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
串口等通信方式下，每收到1字节数据，调用本函数一次，将接收到的数据传入进行数据解析
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8RecvOneByte(uint8_t dat)
{
    if (recvBuf.recvSta == 0)
    {
        //第一步，搜索帧头
        if (dat == ANOPTV8_FRAME_HEAD)
        {
            recvBuf.recvSta = 1;
            recvBuf.recvDataLen = 1;
            recvBuf.dataBuf.rawBytes[0] = dat;
        }
    }
    else if (recvBuf.recvSta == 1)
    {
        //第二步，接收地址字节、帧ID、帧长度等数据
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        if (recvBuf.recvDataLen >= ANOPTV8_FRAME_HEADLEN)
        {
            //如果数据长度超过定义的最大数据长度，重新开始接收
            if(recvBuf.dataBuf.frame.datalen >= ANOPTV8_FRAME_MAXDATALEN)
                recvBuf.recvSta = 0;
            else
                recvBuf.recvSta = 2;
        }
    }
    else if (recvBuf.recvSta == 2)
    {
        //第三步，接收数据内容
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        if (recvBuf.recvDataLen >= ANOPTV8_FRAME_HEADLEN + recvBuf.dataBuf.frame.datalen)
            recvBuf.recvSta = 3;
    }
    else if (recvBuf.recvSta == 3)
    {
        //第四步，接收和校验字节
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        recvBuf.dataBuf.frame.sumcheck = dat;
        recvBuf.recvSta = 4;
    }
    else if (recvBuf.recvSta == 4)
    {
        //第5步，接收附加校验字节
        recvBuf.dataBuf.rawBytes[recvBuf.recvDataLen++] = dat;
        recvBuf.dataBuf.frame.addcheck = dat;
        recvBuf.recvSta = 0;
        //一帧数据接收完毕，进行校验，如果校验通过则进行帧解析
        if (anoPTv8FrameCheck(&recvBuf.dataBuf))
            anoPTv8FrameAnl(&recvBuf.dataBuf);
    }
    else
    {
        recvBuf.recvSta = 0;
    }
}


