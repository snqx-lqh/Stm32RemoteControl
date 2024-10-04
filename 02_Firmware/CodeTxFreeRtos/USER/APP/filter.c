#include "filter.h"

void kalman_filter_v1_init(angle_kalman_filter_t *angle_kalman_filter)
{
	angle_kalman_filter->Q_angle=0.001;// ����������Э����
	angle_kalman_filter->Q_gyro=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
	angle_kalman_filter->R_angle=0.5;// ����������Э���� �Ȳ���ƫ��
	angle_kalman_filter->dt=0.005;//
	angle_kalman_filter->C_0 = 1;
	angle_kalman_filter->PP[0][0] = 1;
	angle_kalman_filter->PP[0][1] = 0;
	angle_kalman_filter->PP[1][0] = 0;
	angle_kalman_filter->PP[1][1] = 1;
}

void kalman_filter_v1(angle_kalman_filter_t *angle_kalman_filter,float *Accel,float *Gyro,float *angle)
{
    *angle+=(*Gyro - angle_kalman_filter->Q_bias) * angle_kalman_filter->dt; //�������
    angle_kalman_filter->Pdot[0]=angle_kalman_filter->Q_angle - 
		angle_kalman_filter->PP[0][1] - angle_kalman_filter->PP[1][0]; // Pk-����������Э�����΢��

    angle_kalman_filter->Pdot[1]=-angle_kalman_filter->PP[1][1];
    angle_kalman_filter->Pdot[2]=-angle_kalman_filter->PP[1][1];
    angle_kalman_filter->Pdot[3]= angle_kalman_filter->Q_gyro;
    angle_kalman_filter->PP[0][0] += angle_kalman_filter->Pdot[0] * angle_kalman_filter->dt;   // Pk-����������Э����΢�ֵĻ���
    angle_kalman_filter->PP[0][1] += angle_kalman_filter->Pdot[1] * angle_kalman_filter->dt;   // =����������Э����
    angle_kalman_filter->PP[1][0] += angle_kalman_filter->Pdot[2] * angle_kalman_filter->dt;
    angle_kalman_filter->PP[1][1] += angle_kalman_filter->Pdot[3] * angle_kalman_filter->dt;

    angle_kalman_filter->Angle_err = *Accel - *angle;	//zk-�������

    angle_kalman_filter->PCt_0 = angle_kalman_filter->C_0 * angle_kalman_filter->PP[0][0];
    angle_kalman_filter->PCt_1 = angle_kalman_filter->C_0 * angle_kalman_filter->PP[1][0];

    angle_kalman_filter->E = angle_kalman_filter->R_angle + angle_kalman_filter->C_0 * angle_kalman_filter->PCt_0;

    angle_kalman_filter->K_0 = angle_kalman_filter->PCt_0 / angle_kalman_filter->E;
    angle_kalman_filter->K_1 = angle_kalman_filter->PCt_1 / angle_kalman_filter->E;

    angle_kalman_filter->t_0 = angle_kalman_filter->PCt_0;
    angle_kalman_filter->t_1 = angle_kalman_filter->C_0 * angle_kalman_filter->PP[0][1];

    angle_kalman_filter->PP[0][0] -= angle_kalman_filter->K_0 * angle_kalman_filter->t_0;		 //����������Э����
    angle_kalman_filter->PP[0][1] -= angle_kalman_filter->K_0 * angle_kalman_filter->t_1;
    angle_kalman_filter->PP[1][0] -= angle_kalman_filter->K_1 * angle_kalman_filter->t_0;
    angle_kalman_filter->PP[1][1] -= angle_kalman_filter->K_1 * angle_kalman_filter->t_1;

    *angle	+= angle_kalman_filter->K_0 * angle_kalman_filter->Angle_err;	 //�������
    angle_kalman_filter->Q_bias	+= angle_kalman_filter->K_1 * angle_kalman_filter->Angle_err;	 //�������
    angle_kalman_filter->angle_dot   = *Gyro - angle_kalman_filter->Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
}

/**************************************************************************
�������ܣ�һ�׻����˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void FirstOrderLowPassFilter(float angle_m, float gyro_m,float *angle)
{
	float K1 =0.02;
    *angle = K1 * angle_m+ (1-K1) * (*angle + gyro_m * 0.005);
}
