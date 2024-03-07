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
	//put_key(1);
}

void KeyDown()
{
	if(isKeyDown == 0)
		isKeyDown=1;
	LED = !LED;
	//put_key(2);
}

void KeyLeft()
{
	if(isKeyLeft == 0)
		isKeyLeft=1;
	LED = !LED;
	//put_key(3);
}

void KeyRight()
{
	if(isKeyRight == 0)
		isKeyRight=1;
	LED = !LED;
	//put_key(4);
}

//环形缓冲区的按键扫描函数，感觉不太好用，就没用了。

#define KEY_BUFF_LEN 20
int g_keys[KEY_BUFF_LEN] = {0};
int key_r = 0,key_w = 0;

#define NEXT_POS(x) ((x+1) % KEY_BUFF_LEN)

static int is_key_buff_empty(void)
{
	return (key_r == key_w); //一开始key_r和key_w都是0，key_r==key_w表示空
}

static int is_key_buff_full(void)
{
	//下一个写的位置等于key_r，表示存满了
	return (key_r == NEXT_POS(key_w));
}

void put_key(int key)
{
	if(!is_key_buff_full())
	{
		g_keys[key_w] = key;
		key_w  = NEXT_POS(key_w);
	}
}

int get_key()
{
	int key = 0;
	if(!is_key_buff_empty())
	{
		key = g_keys[key_r];
		key_r  = NEXT_POS(key_r);
	}
	return key;
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
