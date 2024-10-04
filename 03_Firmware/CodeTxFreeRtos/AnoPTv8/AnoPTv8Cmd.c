#include "AnoPTv8Cmd.h"
const _st_cmd_info * pCmdInfoList[ANOPTV8_CMD_MAXCOUNT];
int cmdCount = 0;

void AnoPTv8CmdFrameAnl(const _un_frame_v8 *p)
{
    switch (p->frame.frameid)
    {
    case 0xC0:
        //CMD
        for(uint16_t i=0; i<AnoPTv8CmdGetCount(); i++)
        {
            _st_cmd_info _ci = *AnoPTv8CmdGetInfo(i);
            if(p->frame.data[0] == _ci.cmd.cmdid0 && p->frame.data[1] == _ci.cmd.cmdid1 && p->frame.data[2] == _ci.cmd.cmdid2)
            {
                AnoPTv8SendCheck(p->frame.sdevid, p->frame.frameid, p->frame.sumcheck, p->frame.addcheck);
                _ci.pFun();
                return;
            }
        }
        break;
    case 0xC1:
        //CMD读取命令
        if (p->frame.data[0] == 0)
        {
            //读取CMD个数
            AnoPTv8SendCmdNum(p->frame.sdevid);
        }
        else if (p->frame.data[0] == 1)
        {
            //读取CMD信息
            AnoPTv8SendCmdInfo(p->frame.sdevid, *(uint16_t *)(p->frame.data+1));
        }
        break;
    }
}

int	AnoPTv8CmdGetCount(void)
{
    return cmdCount;
}

void AnoPTv8CmdRegister(const _st_cmd_info * _pi)
{
    //先检查是否有重复命令
    if(0)
    {
        for(int i=0; i<cmdCount; i++)
        {
            if(_pi->cmd.cmdid0 == pCmdInfoList[i]->cmd.cmdid0
                    && _pi->cmd.cmdid1 == pCmdInfoList[i]->cmd.cmdid1
                    && _pi->cmd.cmdid2 == pCmdInfoList[i]->cmd.cmdid2)
            {
                return;
            }
        }
    }
    if(cmdCount <= (ANOPTV8_CMD_MAXCOUNT-1))
    {
        pCmdInfoList[cmdCount++] = _pi;
    }
}
const _st_cmd_info * AnoPTv8CmdGetInfo(uint16_t cmdid)
{
    if(cmdid > AnoPTv8CmdGetCount())
        return 0;
    return pCmdInfoList[cmdid];
}

