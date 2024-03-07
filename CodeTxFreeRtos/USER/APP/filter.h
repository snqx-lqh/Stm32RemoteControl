#ifndef _FILTER_H
#define _FILTER_H

#include "main.h"

typedef struct {
	float angle_dot;
	float Q_angle;// ����������Э����
	float Q_gyro;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
	float R_angle;// ����������Э���� �Ȳ���ƫ��
	float dt;
	char  C_0;
	float Q_bias, Angle_err;
	float PCt_0, PCt_1, E;
	float K_0, K_1, t_0, t_1;
	float Pdot[4];
	float PP[2][2];
}angle_kalman_filter_t;

void kalman_filter_v1_init(angle_kalman_filter_t *angle_kalman_filter);
void kalman_filter_v1(angle_kalman_filter_t *angle_kalman_filter,float *Accel,float *Gyro,float *angle);
void FirstOrderLowPassFilter(float angle_m, float gyro_m,float *angle);


#endif

