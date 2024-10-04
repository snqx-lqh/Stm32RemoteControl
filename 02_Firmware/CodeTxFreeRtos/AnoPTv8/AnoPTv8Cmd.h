#ifndef __ANOPTV8CMD_H
#define __ANOPTV8CMD_H
#include "AnoPTv8.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
��ζ������
step1��������ã�#include "AnoPTv8Cmd.h"
step2������һ���������޷���ֵ����������Ϊvoid�����磺static void AnoPTv8CmdFun_PID_Rest(void)
step3��������������Ϣ����һ��Ϊ��������ṹ�壬Ȼ����������ʾ���ƣ�Ȼ����������ܣ�����Ǻ���ָ�룬ָ��ղŶ���ĺ���AnoPTv8CmdFun_PID_Rest
		const _st_cmd_info _pCmdInfoResetPid = {{0x01,0xA0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"PID_Rest","PIDReset",AnoPTv8CmdFun_PID_Rest};
step4���ڳ������λ�ã�ע������ִֻ��һ�Σ������ú�����AnoPTv8CmdRegister(&_pCmdInfoResetPid);
step5����ʱ���Ϳ���������λ��������ܴ����ù�����
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
����ṹ��
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
