#ifndef __ANOPTV8RUN_H
#define __ANOPTV8RUN_H
#include "AnoPTv8.h"
//接收缓冲结构体定义
typedef struct
{
    uint8_t recvSta;
    uint16_t recvDataLen;
    _un_frame_v8 dataBuf;
} _recvST;

//发送缓冲结构体定义
typedef struct
{
    uint8_t used;
    uint8_t readyToSend;
    _un_frame_v8 dataBuf;
} _sendST;

//发送缓冲区
extern _sendST AnoPTv8TxBuf[SENDBUFLEN];
//当前正在分析的数据帧
extern _st_frame_v8 AnoPTv8CurrentAnlFrame;

void AnoPTv8RecvOneByte(uint8_t dat);
int AnoPTv8GetFreeTxBufIndex(void);
void AnoPTv8TxRunThread1ms(void);

#endif
