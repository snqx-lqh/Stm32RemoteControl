#include "ahrs.h"
#include "math.h"

#define sampleFreq	200.f			// sample frequency in Hz
/*�������ڵ�һ�룬���������Ԫ��΢�ַ���ʱ���������
��ȷ���Լ�����̬��������: 10ms,�������sampleFreq: 100Hz*/
#define halfT 0.005f
 
//�����Kp,Ki�����ڿ��Ƽ��ٶȼ����������ǻ�����̬���ٶ�
#define Kp 8.0f  //2.0f
#define Ki 0.008f  //0.002f
 
 
//��ʼ��̬��Ԫ��(��������ϵ)��q(q0,q1,q2,q3)
static float q0 = 1, q1 = 0, q2 = 0, q3 = 0; //���Ź�����Ԫ��
//������̬�������Ļ���.     
//��ǰ�ӼƲ�õ��������ٶ�������(x,y,z)�ϵķ���,�뵱ǰ��̬��������������������ϵķ��������Ļ���
static float xErrorInt = 0, yErrorInt = 0, zErrorInt = 0;

/*
 * 6����������̬�ں��㷨:  Mahony�Ļ����˲��㷨 6���
 * ��λ: m/s^2   rad/s
 * ���ڼ��ٶȵ������ܴ�, �˴�����ʹ���˲��������
 * */
void MahonyImuUpdate(float gx, float gy, float gz, float ax, float ay, float az, float* IMU_Angle)//g�������ǣ�a��Ӽ�
{
    float q0temp,q1temp,q2temp,q3temp;//��Ԫ���ݴ���������"΢�ַ���"ʱҪ��
    float norm; //ʸ����ģ����Ԫ���ķ���
    float posture_x, posture_y, posture_z;//��ǰ��̬��������������������ϵķ���
    //��ǰ�ӼƲ�õ��������ٶ��������ϵķ���,���õ�ǰ��̬��������������������ϵķ��������
    float error_x, error_y, error_z;
 
    // �Ȱ���Щ�õõ���ֵ���
    float q0q0 = q0*q0, q0q1 = q0*q1, q0q2 = q0*q2, q0q3 = q0*q3, q1q1 = q1*q1, q1q2 = q1*q2;
    float q1q3 = q1*q3, q2q2 = q2*q2, q2q3 = q2*q3, q3q3 = q3*q3;
 
    //�Ӽƴ�����������״̬ʱ��������̬���㣬��Ϊ�������ĸ���������
    if( (ax == 0.0f) && (ay == 0.0f) && (az == 0.0f) )
       return;
 
    //�����ٶȵ�ԭʼ���ݽ��й�һ��,�õ���λ���ٶ�
    norm = sqrt(ax*ax + ay*ay + az*az);//��λ�����ٶȼƣ�
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
 
    //�õ�ǰ��̬(�ο�����nϵ)������������������ϵķ���(��������bϵ)��
    /*����Ԫ�������"�������Ҿ���"�еĵ����е�����Ԫ��.�������Ҿ����ŷ���ǵĶ���,��������ϵ����������,ת����������ϵ,������������Ԫ��.*/
    posture_x = 2*(q1q3 - q0q2);
    posture_y = 2*(q0q1 + q2q3);
    posture_z = q0q0 - q1q1 - q2q2 + q3q3;
 
    //����: ��������õ������뵱ǰ��̬������������������������Ա�ʾ��һ���
    // Error is sum of cross product between estimated and measured direction of gravity
    error_x = (ay*posture_z - az*posture_y) ;
    error_y = (az*posture_x - ax*posture_z) ;
    error_z = (ax*posture_y - ay*posture_x) ;
 
    //���������������Ĳ�������л���
    xErrorInt = xErrorInt + error_x * Ki;// * (1.0f / sampleFreq);
    yErrorInt = yErrorInt + error_y * Ki;// * (1.0f / sampleFreq);
    zErrorInt = zErrorInt + error_z * Ki;// * (1.0f / sampleFreq);
 
    //���������������Ĳ�����Ļ�����p,i����������ƫ,ͨ������Kp,Ki��������,���Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶ�
    gx = gx + Kp*error_x + xErrorInt;  //�����PI�󲹳��������ǣ����������Ư��
    gy = gy + Kp*error_y + yErrorInt;
    gz = gz + Kp*error_z + zErrorInt; //�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�
 
    // Integrate rate of change of quaternion
    //gx *= (1.0f / sampleFreq);		// pre-multiply common factors
    //gy *= (1.0f / sampleFreq);
    //gz *= (1.0f / sampleFreq);
 
    //���������̬�ĸ��£�Ҳ������Ԫ��΢�ַ��̵����
    q0temp=q0;//�ݴ浱ǰֵ���ڼ���
    q1temp=q1;
    q2temp=q2;
    q3temp=q3;
 
    //��Ԫ��΢�ַ���. ����һ�ױϿ��ⷨ���ںϵ�ǰλ�˺������ǲ���ֵ,��ת��������ο�����ϵN. 
    q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;
    q1 = q1temp + (q0temp*gx + q2temp*gz -q3temp*gy)*halfT;
    q2 = q2temp + (q0temp*gy - q1temp*gz +q3temp*gx)*halfT;
    q3 = q3temp + (q0temp*gz + q1temp*gy -q2temp*gx)*halfT;
 
    //�Ե�ǰ��̬��Ԫ�����е�λ��,��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕ�
    norm = sqrt(q0q0 + q1q1 + q2q2 + q3q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
 
    //�ٴΰ���Щ�õõ���ֵ���
    q0q1 = q0*q1; q0q2 = q0*q2; q0q3 = q0*q3; q1q1 = q1*q1; q1q2 = q1*q2;
    q1q3 = q1*q3; q2q2 = q2*q2; q2q3 = q2*q3; q3q3 = q3*q3;
 
    //��Ԫ����ŷ���ǵ�ת��,����������ǻ���,��Ҫ�Ƕ�ֵ,����ֱ�ӳ���57.3,��һ���ȶ�Ӧ�Ƕ�ֵ
    IMU_Angle[0]  = atan2f(2.f * (q0q1 + q2q3),1 - 2.f * ( q1q1 - q2q2) ); // roll: X��
    IMU_Angle[1]  = asinf(2.f * (q0q2 - q1q3) ); // pitch: Y��
    //����YAW��������ڼ��ٶȼƶ���û���������ã����ʵ�ʽ������ƫ��,��Ҫ׼ȷ,��Ҫʹ�ô�����
    IMU_Angle[2]   = atan2f(2.f * (q0q3 + q1q2),1 - 2.f * (q2q2 + q3q3) ); // yaw: Z��
    
    IMU_Angle[0] = IMU_Angle[0]*57.3f;
    IMU_Angle[1] = IMU_Angle[1]*57.3f;
    IMU_Angle[2] = IMU_Angle[2]*57.3f;

}
