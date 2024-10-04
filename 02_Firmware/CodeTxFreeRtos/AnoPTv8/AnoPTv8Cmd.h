#ifndef __ANOPTV8CMD_H
#define __ANOPTV8CMD_H
#include "AnoPTv8.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
如何定义命令：
step1：添加引用：#include "AnoPTv8Cmd.h"
step2：定义一个函数，无返回值，函数参数为void，例如：static void AnoPTv8CmdFun_PID_Rest(void)
step3：定义该命令的信息，第一个为命令参数结构体，然后是命令显示名称，然后是命令介绍，最后是函数指针，指向刚才定义的函数AnoPTv8CmdFun_PID_Rest
		const _st_cmd_info _pCmdInfoResetPid = {{0x01,0xA0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"PID_Rest","PIDReset",AnoPTv8CmdFun_PID_Rest};
step4：在程序合适位置，注册该命令（只执行一次），调用函数：AnoPTv8CmdRegister(&_pCmdInfoResetPid);
step5：此时，就可以利用上位机的命令功能触发该功能了
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
命令结构体
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint8_t cmdid0;
    uint8_t cmdid1;
    uint8_t cmdid2;
    uint8_t cmdval0;
    uint8_t cmdval1;
    uint8_t cmdval2;
    uint8_t cmdval3;
    uint8_t cmdval4;
    uint8_t cmdval5;
    uint8_t cmdval6;
    uint8_t cmdval7;
} __attribute__ ((__packed__)) _st_cmd;

typedef struct
{
    const _st_cmd		cmd;
    const char		name[20];
    const char		info[50];
    void 		(*pFun)(void);
} __attribute__ ((__packed__)) _st_cmd_info;

void AnoPTv8CmdFrameAnl(const _un_frame_v8 *p);
int	AnoPTv8CmdGetCount(void);
void AnoPTv8CmdRegister(const _st_cmd_info * _pi);
const _st_cmd_info * AnoPTv8CmdGetInfo(uint16_t cmdid);

#endif
