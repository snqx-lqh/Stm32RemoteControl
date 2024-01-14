#include "AnoPTv8Par.h"
const _st_par_info * pParInfoList[ANOPTV8_PAR_MAXCOUNT];
int parCount = 0;

void anoPTv8ParSetVal(uint16_t parid, int32_t val);

void AnoPTv8ParFrameAnl(const _un_frame_v8 *p)
{
    switch (p->frame.frameid)
    {
    case 0xE0:
        //参数命令
        if (p->frame.data[0] == 0)
        {
            //读取设备信息
            AnoPTv8SendDevInfo(p->frame.sdevid);
        }
        else if (p->frame.data[0] == 1)
        {
            //读取参数个数
            AnoPTv8SendParNum(p->frame.sdevid);
        }
        else if (p->frame.data[0] == 2)
        {
            //读取参数值
            AnoPTv8SendParVal(p->frame.sdevid, *(uint16_t *)(p->frame.data+1));
        }
        else if (p->frame.data[0] == 3)
        {
            //读取参数信息
            AnoPTv8SendParInfo(p->frame.sdevid, *(uint16_t *)(p->frame.data+1));
        }
        else if(p->frame.data[0] == 0x10)
        {
            uint16_t cmdval = *(uint16_t *)&p->frame.data[1];
            if(cmdval == 0x00AA)
            {
                //恢复默认参数
                AnoPTv8HwParCmdResetParameter();
            }
			AnoPTv8SendCheck(p->frame.sdevid, p->frame.frameid, p->frame.sumcheck, p->frame.addcheck);
        }
        AnoPTv8HwParCmdRecvCallback(p->frame.data[0], *(uint16_t *)(p->frame.data+1));
        break;
    case 0xE1:
        //参数写入
        anoPTv8ParSetVal(*(uint16_t *)(p->frame.data+0), *(int32_t *)(p->frame.data+2));
        AnoPTv8SendCheck(p->frame.sdevid, p->frame.frameid, p->frame.sumcheck, p->frame.addcheck);
        AnoPTv8HwParValRecvCallback(*(uint16_t *)(p->frame.data+0), *(int32_t *)(p->frame.data+2));
        break;
    }
}

int	AnoPTv8ParGetCount(void)
{
    return parCount;
}

void AnoPTv8ParRegister(const _st_par_info * _pi)
{
    if(parCount <= (ANOPTV8_PAR_MAXCOUNT-1))
    {
        pParInfoList[parCount++] = _pi;
    }
}

const _st_par_info * AnoPTv8ParGetInfo(uint16_t parid)
{
    if(parid > AnoPTv8ParGetCount())
        return 0;
    return pParInfoList[parid];
}

int32_t AnoPTv8ParGetVal(uint16_t parid)
{
    if(parid > AnoPTv8ParGetCount())
        return 0;
    if(pParInfoList[parid]->mul == 0)
        return *(int32_t *)(pParInfoList[parid]->pval);
    else
        return *(float *)(pParInfoList[parid]->pval) * pParInfoList[parid]->mul;
}
void anoPTv8ParSetVal(uint16_t parid, int32_t val)
{
    if(parid > AnoPTv8ParGetCount())
        return;
    if(pParInfoList[parid]->mul == 0)
        *(int32_t *)(pParInfoList[parid]->pval) = val;
    else
        *(float *)(pParInfoList[parid]->pval) = (float)val / pParInfoList[parid]->mul;
}
