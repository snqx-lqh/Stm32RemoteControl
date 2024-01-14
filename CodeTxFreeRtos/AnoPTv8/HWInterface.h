#ifndef __HARDWAREINTERFACE_H
#define __HARDWAREINTERFACE_H
#include <stdint.h>
#include "main.h"

void AnoPTv8HwSendBytes(uint8_t *buf, uint16_t len);
void AnoPTv8HwRecvByte(uint8_t dat);
void AnoPTv8HwTrigger1ms(void);

void AnoPTv8HwParValRecvCallback(uint16_t parid, int32_t parval);
void AnoPTv8HwParCmdRecvCallback(uint8_t id, uint16_t val);
void AnoPTv8HwParCmdResetParameter(void);
void AnoPTv8HwParCmdResetAccCal(void);
void AnoPTv8HwParCmdResetMagCal(void);
void AnoPTv8HwParCmdFinishAccCal(void);
void AnoPTv8HwParCmdFinishMagCal(void);
#endif
