#include "HWInterface.h"
#include "AnoPTv8Run.h"
#include "AnoPTv8Par.h"
//AptHwSendBytes此函数需要根据用户自己的设备，具体实现，比如使用串口连接上位机，这里就对应该串口的发送函数
//注意：串口驱动务必使用中断+缓冲区或者DMA+缓冲区的方式，阻塞式发送将大大影响系统性能
//注意：串口缓冲区不应过小，推荐256字节或以上
//这里需引用对应的串口h文件
#include "bsp_usart.h"

void AnoPTv8HwSendBytes(uint8_t *buf, uint16_t len)
{
    USART1_Send_Len_Data(buf, len);
}

//AptHwRecvByte此函数已在hardwareInterface.h中声明，用户需要在对应串口的接收事件中调用此函数
//注意：此函数传入的是字节数据，如果接收事件接收到的数据大于1字节，多次调用此函数即可
void AnoPTv8HwRecvByte(uint8_t dat)
{
    AnoPTv8RecvOneByte(dat);
}

//AptHwTrigger1ms此函数已在hardwareInterface.h中声明，用户需要在1ms定时中断或者系统滴答或者自己设计的调度器内
//以1ms的时间间隔调用此函数
void AnoPTv8HwTrigger1ms(void)
{
    AnoPTv8TxRunThread1ms();
}

void AnoPTv8HwParValRecvCallback(uint16_t parid, int32_t parval)
{
    
}

void AnoPTv8HwParCmdRecvCallback(uint8_t id, uint16_t val)
{
    if(id == 0x10)
    {
        if(val == 0x0020)
        {
            //清除加速度校准数据
            
        }
        else if(val == 0x0021)
        {
            //收到加速度校准数据，触发保存，启用参数
            
        }
    }
}

void AnoPTv8HwParCmdResetParameter(void)
{
    
}

