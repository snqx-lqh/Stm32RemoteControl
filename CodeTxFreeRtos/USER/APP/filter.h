#ifndef _FILTER_H
#define _FILTER_H

#include "main.h"

typedef struct {
	float angle_dot;
	float Q_angle;// 过程噪声的协方差
	float Q_gyro;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	float R_angle;// 测量噪声的协方差 既测量偏差
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

