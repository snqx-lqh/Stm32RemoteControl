#ifndef __ANOPTV8PAR_H
#define __ANOPTV8PAR_H
#include "AnoPTv8.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
��ζ��������
step1��������ã�#include "AnoPTv8Par.h"
step2������float�Ͳ�������������float�ͱ�����float _testpar = 123;
step3������ò�������Ϣ����һ��Ϊָ�������ָ�룬Ȼ����������Сֵ�����ֵ�����ű������������ƣ��������
		const _st_par_info _testParInfo = {&_testpar, 1,2,100,"test", "test2"};
step4���ڳ������λ�ã�ע��ò�����ִֻ��һ�Σ������ú�����AnoPTv8ParRegister(&_testParInfo);
step5����ʱ���Ϳ���������λ���Ըò������ж�д��
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
�����ṹ�嶨�壬������ö��
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    void * 		pval;
    const int32_t	min;
    const int32_t	max;
    const uint32_t	mul;
    const char		name[20];
    const char		info[70];
} __attribute__ ((__packed__)) _st_par_info;

void AnoPTv8ParFrameAnl(const _un_frame_v8 *p);
int	AnoPTv8ParGetCount(void);
void AnoPTv8ParRegister(const _st_par_info * _pi);
const _st_par_info * AnoPTv8ParGetInfo(uint16_t parid);
int32_t AnoPTv8ParGetVal(uint16_t parid);
void anoPTv8ParSetVal(uint16_t parid, int32_t val);

#endif
