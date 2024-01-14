#ifndef __ANOPTV8RUN_H
#define __ANOPTV8RUN_H
#include "AnoPTv8.h"
//���ջ���ṹ�嶨��
typedef struct
{
    uint8_t recvSta;
    uint16_t recvDataLen;
    _un_frame_v8 dataBuf;
} _recvST;

//���ͻ���ṹ�嶨��
typedef struct
{
    uint8_t used;
    uint8_t readyToSend;
    _un_frame_v8 dataBuf;
} _sendST;

//���ͻ�����
extern _sendST AnoPTv8TxBuf[SENDBUFLEN];
//��ǰ���ڷ���������֡
extern _st_frame_v8 AnoPTv8CurrentAnlFrame;

void AnoPTv8RecvOneByte(uint8_t dat);
int AnoPTv8GetFreeTxBufIndex(void);
void AnoPTv8TxRunThread1ms(void);

#endif
