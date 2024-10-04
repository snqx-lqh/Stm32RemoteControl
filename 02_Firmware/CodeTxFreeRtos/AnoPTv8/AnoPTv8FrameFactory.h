#ifndef __ANOPTV8FRAMEFACTORY_H
#define __ANOPTV8FRAMEFACTORY_H
#include "HWInterface.h"

void AnoPTv8SendBuf(uint8_t daddr, uint8_t fid, uint8_t *buf, uint16_t len);

void AnoPTv8SendCheck(uint8_t daddr, uint8_t id, uint8_t sc, uint8_t ac);
void AnoPTv8SendDevInfo(uint8_t daddr);
void AnoPTv8SendStr(uint8_t daddr, uint8_t string_color, char *str);
void AnoPTv8SendValStr(uint8_t daddr, float val, char *str);

void AnoPTv8SendParNum(uint8_t daddr);
void AnoPTv8SendParVal(uint8_t daddr, uint16_t parid);
void AnoPTv8SendParInfo(uint8_t daddr, uint16_t parid);

void AnoPTv8SendCmdNum(uint8_t daddr);
void AnoPTv8SendCmdInfo(uint8_t daddr, uint16_t cmd);

void AnoPTv8SendAnyFrame(uint8_t daddr, uint8_t fid, const uint8_t *dat, const uint8_t len);

void AnoPTv8SendIapCmd(uint8_t daddr, const uint8_t *dat, const uint8_t len);


#endif
