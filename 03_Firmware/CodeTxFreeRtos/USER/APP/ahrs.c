#include "ahrs.h"
#include "math.h"

#define sampleFreq	200.f			// sample frequency in Hz
/*采样周期的一半，用于求解四元数微分方程时计算角增量
请确定自己的姿态调用周期: 10ms,即上面的sampleFreq: 100Hz*/
#define halfT 0.005f
 
//这里的Kp,Ki是用于控制加速度计修正陀螺仪积分姿态的速度
#define Kp 8.0f  //2.0f
#define Ki 0.008f  //0.002f
 
 
//初始姿态四元数(地理坐标系)，q(q0,q1,q2,q3)
static float q0 = 1, q1 = 0, q2 = 0, q3 = 0; //最优估计四元数
//定义姿态解算误差的积分.     
//当前加计测得的重力加速度在三轴(x,y,z)上的分量,与当前姿态计算得来的重力在三轴上的分量的误差的积分
static float xErrorInt = 0, yErrorInt = 0, zErrorInt = 0;

/*
 * 6轴陀螺仪姿态融合算法:  Mahony的互补滤波算法 6轴版
 * 单位: m/s^2   rad/s
 * 由于加速度的噪音很大, 此处建议使用滤波后的数据
 * */
void MahonyImuUpdate(float gx, float gy, float gz, float ax, float ay, float az, float* IMU_Angle)//g表陀螺仪，a表加计
{
    float q0temp,q1temp,q2temp,q3temp;//四元数暂存变量，求解"微分方程"时要用
    float norm; //矢量的模或四元数的范数
    float posture_x, posture_y, posture_z;//当前姿态计算得来的重力在三轴上的分量
    //当前加计测得的重力加速度在三轴上的分量,与用当前姿态计算得来的重力在三轴上的分量的误差
    float error_x, error_y, error_z;
 
    // 先把这些用得到的值算好
    float q0q0 = q0*q0, q0q1 = q0*q1, q0q2 = q0*q2, q0q3 = q0*q3, q1q1 = q1*q1, q1q2 = q1*q2;
    float q1q3 = q1*q3, q2q2 = q2*q2, q2q3 = q2*q3, q3q3 = q3*q3;
 
    //加计处于自由落体状态时不进行姿态解算，因为会产生分母无穷大的情况
    if( (ax == 0.0f) && (ay == 0.0f) && (az == 0.0f) )
       return;
 
    //将加速度的原始数据进行归一化,得到单位加速度
    norm = sqrt(ax*ax + ay*ay + az*az);//单位化加速度计，
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
 
    //用当前姿态(参考坐标n系)计算出重力在三个轴上的分量(载体坐标b系)，
    /*把四元数换算成"方向余弦矩阵"中的第三列的三个元素.根据余弦矩阵和欧拉角的定义,地理坐标系的重力向量,转到载体坐标系,正好是这三个元素.*/
    posture_x = 2*(q1q3 - q0q2);
    posture_y = 2*(q0q1 + q2q3);
    posture_z = q0q0 - q1q1 - q2q2 + q3q3;
 
    //计算: 传感器测得的重力与当前姿态计算的重力间的误差，向量外积可以表示这一误差
    // Error is sum of cross product between estimated and measured direction of gravity
    error_x = (ay*posture_z - az*posture_y) ;
    error_y = (az*posture_x - ax*posture_z) ;
    error_z = (ax*posture_y - ay*posture_x) ;
 
    //对两种重力分量的叉积误差进行积分
    xErrorInt = xErrorInt + error_x * Ki;// * (1.0f / sampleFreq);
    yErrorInt = yErrorInt + error_y * Ki;// * (1.0f / sampleFreq);
    zErrorInt = zErrorInt + error_z * Ki;// * (1.0f / sampleFreq);
 
    //对两种重力分量的叉积误差的积分做p,i修正陀螺零偏,通过调节Kp,Ki两个参数,可以控制加速度计修正陀螺仪积分姿态的速度
    gx = gx + Kp*error_x + xErrorInt;  //将误差PI后补偿到陀螺仪，即补偿零点漂移
    gy = gy + Kp*error_y + yErrorInt;
    gz = gz + Kp*error_z + zErrorInt; //这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减
 
    // Integrate rate of change of quaternion
    //gx *= (1.0f / sampleFreq);		// pre-multiply common factors
    //gy *= (1.0f / sampleFreq);
    //gz *= (1.0f / sampleFreq);
 
    //下面进行姿态的更新，也就是四元数微分方程的求解
    q0temp=q0;//暂存当前值用于计算
    q1temp=q1;
    q2temp=q2;
    q3temp=q3;
 
    //四元数微分方程. 采用一阶毕卡解法，融合当前位姿和陀螺仪测量值,并转换到世界参考坐标系N. 
    q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;
    q1 = q1temp + (q0temp*gx + q2temp*gz -q3temp*gy)*halfT;
    q2 = q2temp + (q0temp*gy - q1temp*gz +q3temp*gx)*halfT;
    q3 = q3temp + (q0temp*gz + q1temp*gy -q2temp*gx)*halfT;
 
    //对当前姿态四元数进行单位化,单位化四元数在空间旋转时不会拉伸，仅有旋转角度
    norm = sqrt(q0q0 + q1q1 + q2q2 + q3q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
 
    //再次把这些用得到的值算好
    q0q1 = q0*q1; q0q2 = q0*q2; q0q3 = q0*q3; q1q1 = q1*q1; q1q2 = q1*q2;
    q1q3 = q1*q3; q2q2 = q2*q2; q2q3 = q2*q3; q3q3 = q3*q3;
 
    //四元数到欧拉角的转换,这里输出的是弧度,想要角度值,可以直接乘以57.3,即一弧度对应角度值
    IMU_Angle[0]  = atan2f(2.f * (q0q1 + q2q3),1 - 2.f * ( q1q1 - q2q2) ); // roll: X轴
    IMU_Angle[1]  = asinf(2.f * (q0q2 - q1q3) ); // pitch: Y轴
    //其中YAW航向角由于加速度计对其没有修正作用，因此实际结果会逐渐偏移,想要准确,需要使用磁力计
    IMU_Angle[2]   = atan2f(2.f * (q0q3 + q1q2),1 - 2.f * (q2q2 + q3q3) ); // yaw: Z轴
    
    IMU_Angle[0] = IMU_Angle[0]*57.3f;
    IMU_Angle[1] = IMU_Angle[1]*57.3f;
    IMU_Angle[2] = IMU_Angle[2]*57.3f;

}
