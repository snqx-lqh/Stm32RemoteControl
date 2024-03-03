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

float K1 =0.02;
float angle_dot;
float Q_angle=0.001;// ����������Э����
float Q_gyro=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle=0.5;// ����������Э���� �Ȳ���ƫ��
float dt=0.005;//
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = {0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };
float angle = 0;
/**************************************************************************
�������ܣ����׿������˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void Kalman_Filter(float Accel,float Gyro)
{
    angle+=(Gyro - Q_bias) * dt; //�������
    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

    Pdot[1]=-PP[1][1];
    Pdot[2]=-PP[1][1];
    Pdot[3]=Q_gyro;
    PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
    PP[0][1] += Pdot[1] * dt;   // =����������Э����
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    Angle_err = Accel - angle;	//zk-�������

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;		 //����������Э����
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    angle	+= K_0 * Angle_err;	 //�������
    Q_bias	+= K_1 * Angle_err;	 //�������
    angle_dot   = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
}
/**************************************************************************
�������ܣ�һ�׻����˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void FirstOrderLowPassFilter(float angle_m, float gyro_m)
{
    angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * 0.005);
}
