#include "keyscan.h"

#include "bsp_led.h"
#include "bsp_key.h"

u8 isKeyUp = 0;
u8 isKeyDown = 0;
u8 isKeyLeft = 0;
u8 isKeyRight = 0;

void KeyUp()
{
	if(isKeyUp == 0)
		isKeyUp=1;
	LED=!LED;
}

void KeyDown()
{
	if(isKeyDown == 0)
		isKeyDown=1;
	LED = !LED;
}

void KeyLeft()
{
	if(isKeyLeft == 0)
		isKeyLeft=1;
	LED = !LED;
}

void KeyRight()
{
	if(isKeyRight == 0)
		isKeyRight=1;
	LED = !LED;
}


/**
 * @brief 按键扫描函数
 * 
 * @param mode 模式为1就是连续扫描，为0就是单次
 */
void KeyScan(u8 mode)
{
    static int keyCount = 0;
    static int keyState = 0;
    if(mode == 1) keyState=0;
    if (keyState == 0 && (KEY_UP == 0||KEY_DOWN == 0||KEY_LEFT == 0||KEY_RIGHT == 0))
    {    
        keyCount++;
        if(keyCount>2)
        {
            keyState = 1;
            keyCount=0;
            if(KEY_UP == 0) KeyUp();
            else if(KEY_DOWN == 0) KeyDown();
            else if (KEY_LEFT == 0) KeyLeft();
            else if (KEY_RIGHT == 0) KeyRight();
        }
    }else if (KEY_UP == 1 && KEY_DOWN == 1 && KEY_LEFT == 1 && KEY_RIGHT == 1)
    {
        keyState = 0;
    }
}