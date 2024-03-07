#ifndef _KEYSCAN_H
#define _KEYSCAN_H

#include "main.h"

void KeyScan(u8 mode);

extern u8 isKeyUp;
extern u8 isKeyDown;
extern u8 isKeyLeft;
extern u8 isKeyRight;

void put_key(int key);
int  get_key(void);

#endif // !_KEYSCAN_H
 
