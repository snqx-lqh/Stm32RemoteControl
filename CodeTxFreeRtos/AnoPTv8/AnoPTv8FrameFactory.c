#include "AnoPTv8FrameFactory.h"
#include "AnoPTv8.h"
#include "AnoPTv8Run.h"
#include "AnoPTv8DataDef.h"
#include "AnoPTv8Par.h"
#include "AnoPTv8Cmd.h"
#include "HWInterface.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
发送数据时，根据数据帧计算sumcheck和addcheck
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void AnoPTv8CalFrameCheck(_un_frame_v8 *p)
{
    uint8_t sumcheck = 0, addcheck = 0;
    for (uint16_t i = 0; i < (ANOPTV8_FRAME_HEADLEN + p->frame.datalen); i++)
    {
        sumcheck += p->rawBytes[i];
        addcheck += sumcheck;
    }
    p->rawBytes[ANOPTV8_FRAME_HEADLEN + p->frame.datalen] = sumcheck;
    p->rawBytes[ANOPTV8_FRAME_HEADLEN + p->frame.datalen + 1] = addcheck;
    p->frame.sumcheck = sumcheck;
    p->frame.addcheck = addcheck;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8SendBuf(uint8_t daddr, uint8_t fid, uint8_t *buf, uint16_t len)
{
    uint8_t _cnt = 0;

    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = fid;
        /***********************************************************************/
        for(uint16_t i=0; i<len; i++)
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = *(buf+i);
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
校验帧发送
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8SendCheck(uint8_t daddr, uint8_t id, uint8_t sc, uint8_t ac)
{
    uint8_t _cnt = 0;
    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0x00;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = id;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = sc;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = ac;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
设备信息帧发送
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8SendDevInfo(uint8_t daddr)
{
    uint8_t _cnt = 0;
    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xE3;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = ANOPTV8_MYDEVID;
        uint16_t _tmp = ANOPTV8_HWVER;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        _tmp = ANOPTV8_SWVER;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        _tmp = ANOPTV8_BLVER;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        _tmp = ANOPTV8_PTVER;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        uint8_t i=0;
        char *str = "TestDevice123";
        while(*(str+i) != '\0')//单引号字符，双引号字符串
        {
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = *(str+i++);
            if(i > 20)
            {
                break;
            }
        }
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
字符串信息发送
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8SendStr(uint8_t daddr, uint8_t string_color, char *str)
{
    uint8_t _cnt = 0;
    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xA0;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = string_color;
        uint8_t i=0;
        while(*(str+i) != '\0')//单引号字符，双引号字符串
        {
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = *(str+i++);
            if(i > 50)
            {
                break;
            }
        }
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
void AnoPTv8SendValStr(uint8_t daddr, float val, char *str)
{
    uint8_t _cnt = 0;
    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xA1;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(val);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(val);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE2(val);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE3(val);
        uint8_t i=0;
        while(*(str+i) != '\0')//单引号字符，双引号字符串
        {
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = *(str+i++);
            if(i > 50)
            {
                break;
            }
        }
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
参数类帧发送
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8SendParNum(uint8_t daddr)
{
    uint8_t _cnt = 0;
    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xE0;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = 1;
        uint16_t _tmp = AnoPTv8ParGetCount();
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
void AnoPTv8SendParVal(uint8_t daddr, uint16_t parid)
{
    uint8_t _cnt = 0;

    if(parid >= AnoPTv8ParGetCount())
        return;

    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xE1;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(parid);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(parid);
        int32_t _tmp = AnoPTv8ParGetVal(parid);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE2(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE3(_tmp);
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
void AnoPTv8SendParInfo(uint8_t daddr, uint16_t parid)
{
    uint8_t _cnt = 0;

    if(parid >= AnoPTv8ParGetCount())
        return;

    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xE2;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(parid);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(parid);
        if(AnoPTv8ParGetInfo(parid) == 0)
            return;
        const _st_par_info _p = * AnoPTv8ParGetInfo(parid);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_p.min);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_p.min);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE2(_p.min);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE3(_p.min);

        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_p.max);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_p.max);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE2(_p.max);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE3(_p.max);

        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_p.mul);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_p.mul);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE2(_p.mul);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE3(_p.mul);

        for(uint8_t i=0; i<20; i++)
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.name[i];
        uint8_t i=0;
        while(_p.info[i] != '\0')//单引号字符，双引号字符串
        {
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.info[i++];
            if(i > 50)
            {
                break;
            }
        }
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
命令类帧发送
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AnoPTv8SendCmdNum(uint8_t daddr)
{
    uint8_t _cnt = 0;

    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xC1;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = 0;
        uint16_t _tmp = AnoPTv8CmdGetCount();
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(_tmp);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(_tmp);
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}
void AnoPTv8SendCmdInfo(uint8_t daddr, uint16_t cmd)
{
    uint8_t _cnt = 0;

    if(cmd >= AnoPTv8CmdGetCount())
        return;

    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = 0xC2;
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE0(cmd);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = BYTE1(cmd);

        if(AnoPTv8CmdGetInfo(cmd) == 0)
            return;
        const _st_cmd_info _p = * AnoPTv8CmdGetInfo(cmd);
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdid0;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdid1;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdid2;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval0;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval1;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval2;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval3;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval4;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval5;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval6;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.cmd.cmdval7;

        for(uint8_t i=0; i<20; i++)
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.name[i];
        uint8_t i=0;
        while(_p.info[i] != '\0')//单引号字符，双引号字符串
        {
            AnoPTv8TxBuf[bufindex].dataBuf.frame.data[_cnt++] = _p.info[i++];
            if(i > 50)
            {
                break;
            }
        }
        /***********************************************************************/
        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = _cnt;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}

void AnoPTv8SendAnyFrame(uint8_t daddr, uint8_t fid, const uint8_t *dat, const uint8_t len)
{
    int bufindex = AnoPTv8GetFreeTxBufIndex();

    if (bufindex >= 0)
    {
        AnoPTv8TxBuf[bufindex].used = 1;

        AnoPTv8TxBuf[bufindex].dataBuf.frame.head = ANOPTV8_FRAME_HEAD;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.sdevid = ANOPTV8_MYDEVID;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.ddevid = daddr;
        AnoPTv8TxBuf[bufindex].dataBuf.frame.frameid = fid;
        /***********************************************************************/
        memcpy(AnoPTv8TxBuf[bufindex].dataBuf.frame.data, dat, len);

        AnoPTv8TxBuf[bufindex].dataBuf.frame.datalen = len;
        AnoPTv8CalFrameCheck(&AnoPTv8TxBuf[bufindex].dataBuf);
        AnoPTv8TxBuf[bufindex].readyToSend = 1;
    }
}

void AnoPTv8SendIapCmd(uint8_t daddr, const uint8_t *dat, const uint8_t len)
{
    uint8_t _buf[5];

    _buf[0] = 0;
    memcpy(&_buf[1], dat, len);
    AnoPTv8SendAnyFrame(daddr, 0xF0, _buf, len+1);
}


