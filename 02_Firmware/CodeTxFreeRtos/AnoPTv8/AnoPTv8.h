#ifndef __ANOPTV8_H__
#define __ANOPTV8_H__

#include "HWInterface.h"
#include "AnoPTv8FrameFactory.h"
#include <string.h>

#define ANOPTV8_MYDEVID		(0x01)
#define ANOPTV8DEVID_SWJ	(0xFE)
#define ANOPTV8DEVID_ALL	(0xFF)
#define ANOPTV8_HWVER		(500)
#define ANOPTV8_SWVER		(100)
#define ANOPTV8_BLVER		(800)
#define ANOPTV8_PTVER		(800)

#define ANOPTV8_FRAME_HEAD			0xAB
#define ANOPTV8_FRAME_HEADLEN 		6
#define ANOPTV8_FRAME_MAXDATALEN	300
#define ANOPTV8_FRAME_MAXFRAMELEN	ANOPTV8_FRAME_MAXDATALEN+ANOPTV8_FRAME_HEADLEN+2

#define ANOPTV8_PAR_MAXCOUNT	100
#define ANOPTV8_CMD_MAXCOUNT	30

#define LOG_COLOR_DEFAULT	0
#define LOG_COLOR_RED	  	1
#define LOG_COLOR_GREEN		2
#define LOG_COLOR_BLUE		3

#define SENDBUFLEN 			5


//获取超过一字节数据的每字节数据，例如int16、int32、float等
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )

//V8协议帧结构定义
typedef struct {
    uint8_t head;
    uint8_t sdevid;
    uint8_t ddevid;
    uint8_t frameid;
    uint16_t datalen;
    uint8_t data[ANOPTV8_FRAME_MAXDATALEN];
    uint8_t sumcheck;
    uint8_t addcheck;
} __attribute__ ((__packed__)) _st_frame_v8;
//联合体，可以字节格式访问V8协议帧结构体
typedef union {
    _st_frame_v8 frame;
    uint8_t rawBytes[sizeof(_st_frame_v8)];
} _un_frame_v8;

#endif
