#include "filter.h"

void kalman_filter_v1_init(angle_kalman_filter_t *angle_kalman_filter)
{
	angle_kalman_filter->Q_angle=0.001;// 过程噪声的协方差
	angle_kalman_filter->Q_gyro=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	angle_kalman_filter->R_angle=0.5;// 测量噪声的协方差 既测量偏差
	angle_kalman_filter->dt=0.005;//
	angle_kalman_filter->C_0 = 1;
	angle_kalman_filter->PP[0][0] = 1;
	angle_kalman_filter->PP[0][1] = 0;
	angle_kalman_filter->PP[1][0] = 0;
	angle_kalman_filter->PP[1][1] = 1;
}

void kalman_filter_v1(angle_kalman_filter_t *angle_kalman_filter,float *Accel,float *Gyro,float *angle)
{
    *angle+=(*Gyro - angle_kalman_filter->Q_bias) * angle_kalman_filter->dt; //先验估计
    angle_kalman_filter->Pdot[0]=angle_kalman_filter->Q_angle - 
		angle_kalman_filter->PP[0][1] - angle_kalman_filter->PP[1][0]; // Pk-先验估计误差协方差的微分

    angle_kalman_filter->Pdot[1]=-angle_kalman_filter->PP[1][1];
    angle_kalman_filter->Pdot[2]=-angle_kalman_filter->PP[1][1];
    angle_kalman_filter->Pdot[3]= angle_kalman_filter->Q_gyro;
    angle_kalman_filter->PP[0][0] += angle_kalman_filter->Pdot[0] * angle_kalman_filter->dt;   // Pk-先验估计误差协方差微分的积分
    angle_kalman_filter->PP[0][1] += angle_kalman_filter->Pdot[1] * angle_kalman_filter->dt;   // =先验估计误差协方差
    angle_kalman_filter->PP[1][0] += angle_kalman_filter->Pdot[2] * angle_kalman_filter->dt;
    angle_kalman_filter->PP[1][1] += angle_kalman_filter->Pdot[3] * angle_kalman_filter->dt;

    angle_kalman_filter->Angle_err = *Accel - *angle;	//zk-先验估计

    angle_kalman_filter->PCt_0 = angle_kalman_filter->C_0 * angle_kalman_filter->PP[0][0];
    angle_kalman_filter->PCt_1 = angle_kalman_filter->C_0 * angle_kalman_filter->PP[1][0];

    angle_kalman_filter->E = angle_kalman_filter->R_angle + angle_kalman_filter->C_0 * angle_kalman_filter->PCt_0;

    angle_kalman_filter->K_0 = angle_kalman_filter->PCt_0 / angle_kalman_filter->E;
    angle_kalman_filter->K_1 = angle_kalman_filter->PCt_1 / angle_kalman_filter->E;

    angle_kalman_filter->t_0 = angle_kalman_filter->PCt_0;
    angle_kalman_filter->t_1 = angle_kalman_filter->C_0 * angle_kalman_filter->PP[0][1];

    angle_kalman_filter->PP[0][0] -= angle_kalman_filter->K_0 * angle_kalman_filter->t_0;		 //后验估计误差协方差
    angle_kalman_filter->PP[0][1] -= angle_kalman_filter->K_0 * angle_kalman_filter->t_1;
    angle_kalman_filter->PP[1][0] -= angle_kalman_filter->K_1 * angle_kalman_filter->t_0;
    angle_kalman_filter->PP[1][1] -= angle_kalman_filter->K_1 * angle_kalman_filter->t_1;

    *angle	+= angle_kalman_filter->K_0 * angle_kalman_filter->Angle_err;	 //后验估计
    angle_kalman_filter->Q_bias	+= angle_kalman_filter->K_1 * angle_kalman_filter->Angle_err;	 //后验估计
    angle_kalman_filter->angle_dot   = *Gyro - angle_kalman_filter->Q_bias;	 //输出值(后验估计)的微分=角速度
}

/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void FirstOrderLowPassFilter(float angle_m, float gyro_m,float *angle)
{
	float K1 =0.02;
    *angle = K1 * angle_m+ (1-K1) * (*angle + gyro_m * 0.005);
}
