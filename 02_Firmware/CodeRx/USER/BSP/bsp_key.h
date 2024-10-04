#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "main.h"

void bsp_key_init(void);

#define KEY_UP       PAin(15)
#define KEY_DOWN     PBin(3)
#define KEY_LEFT     PBin(4)
#define KEY_RIGHT    PBin(5)

#endif

