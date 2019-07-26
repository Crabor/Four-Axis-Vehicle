#include "IMU.h"
         
S_FLOAT_XYZ Q_ANGLE;			//��Ԫ��������ĽǶ�
S_FLOAT_XYZ A_ANGLE;
float g_fYawOffSet=0;


float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error


#define Kp 		2.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 		0.01f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 	0.005f                   // half the sample period�������ڵ�һ��

//����������������ٶ�,���ٶȼƺʹ�ǿ��
//����������������������ݵ�ƫ��
//һ��������ڼ�

/**************************************
 * ��������Get_Attitude
 * ����  ���õ���ǰ��̬
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *************************************/
void Get_Attitude(void)
{
	IMUupdate(MPU6050_GYRO_LAST.X*Gyro_Gr,MPU6050_GYRO_LAST.Y*Gyro_Gr,MPU6050_GYRO_LAST.Z*Gyro_Gr,
			ACC_AVG.X,ACC_AVG.Y,ACC_AVG.Z);	//*0.0174ת�ɻ���

}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
  	float norm;
	//  float hx, hy, hz, bx, bz;
  	float vx, vy, vz;// wx, wy, wz;
  	float ex, ey, ez;

  	// �Ȱ���Щ�õõ���ֵ���
  	float q0q0 = q0*q0;
  	float q0q1 = q0*q1;
  	float q0q2 = q0*q2;
	//  float q0q3 = q0*q3;//
  	float q1q1 = q1*q1;
	//  float q1q2 = q1*q2;//
  	float q1q3 = q1*q3;
  	float q2q2 = q2*q2;
  	float q2q3 = q2*q3;
  	float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
		
  	norm = sqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
  	ax = ax /norm;
  	ay = ay / norm;
  	az = az / norm;

  	// estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
  	vx = 2*(q1q3 - q0q2);												//��Ԫ����xyz�ı�ʾ
  	vy = 2*(q0q1 + q2q3);
  	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  	// error is sum of cross product between reference direction of fields and direction measured by sensors
  	ex = (ay*vz - az*vy) ;                           					 //�������������õ���־������
  	ey = (az*vx - ax*vz) ;
  	ez = (ax*vy - ay*vx) ;

  	exInt = exInt + ex * Ki;								  //�������л���
  	eyInt = eyInt + ey * Ki;
  	ezInt = ezInt + ez * Ki;

  	// adjusted gyroscope measurements
  	gx = gx + Kp*ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
  	gy = gy + Kp*ey + eyInt;
  	gz = gz + Kp*ez + ezInt;				   							//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

  	// integrate quaternion rate and normalise						   //��Ԫ�ص�΢�ַ���
  	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  	// normalise quaternion
  	norm = sqrt(q0q0 + q1q1 + q2q2 + q3q3);
  	q0 = q0 / norm;
  	q1 = q1 / norm;
  	q2 = q2 / norm;
  	q3 = q3 / norm;

  	//Q_ANGLE.Z = atan2(2 * q1q2 + 2 * q0q3, -2 * q2q2 - 2 * q3q3 + 1)* 57.3; // yaw
  Q_ANGLE.X = atan2(2*q2q3 + 2*q0q1, -2*q1q1 - 2*q2q2 + 1)*57.3; // roll
	Q_ANGLE.Y = asin(-2*q1q3 + 2*q0q2)* 57.3; // pitch
	Q_ANGLE.Z = (-Read_HMC5883L())-g_fYawOffSet;
	//if(Q_ANGLE.Z<=-180)	Q_ANGLE.Z = Q_ANGLE.Z+360;//yaw

}

void Get_YawOffSet(void)
{
	double temp=0;
	int i=0;
	for(i=0;i<200;i++)
	{
		temp+= (-Read_HMC5883L());
	}
	g_fYawOffSet = (float)(temp/200);
}

//��ü��ٶȼƽǶ�
void ACC_ANGLE(void)
{
	float x,y,z;
	x=MPU6050_ACC_LAST.X;
	y=MPU6050_ACC_LAST.Y;
	z=MPU6050_ACC_LAST.Z;
	A_ANGLE.X=atan(sqrt((x*x+y*y))/z)*180/3.14;
	A_ANGLE.Y=atan(x/sqrt((y*y+z*z)))*180/3.14;
	A_ANGLE.X=atan(y/sqrt((x*x+z*z)))*180/3.14;		
}


/***************************************************************
//�õ��Ƕ�
//x,y,z:x,y,z������������ٶȷ���(����Ҫ��λ,ֱ����ֵ����)
//dir:Ҫ��õĽǶ�.0,��Z��ĽǶ�;1,��X��ĽǶ�;2,��Y��ĽǶ�.
//����ֵ:�Ƕ�ֵ.��λ0.1��.
float Acc_Get_Angle(float x,float y,float z,u8 dir)
{
	float temp;
 	float res=0;
	switch(dir)
	{
		case 0://����ȻZ��ĽǶ�
 			temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://����ȻX��ĽǶ�
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://����ȻY��ĽǶ�
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return res*180/3.14;
}
*/

 /*
float *KalmanFileter(float angle_m,float gyro_m )
{
	u8 i,j,k;
	//static float T=0.02; 
  static float A[2][2]={ { 1, -0.01}, 
                         { 0,     1} }; 
  static float B[2]={ 0.01, 0 }; 
  static float H[2]={1,0};
  static float Q[2][2]={ {0.005, 0.005 }, 
                         {0.005, 0.005 } }; 
  static float R=0.056; 
  static float X_optimal[2]={ 0, 0} ;
  static float P_optimal[2][2]={ { 0.005, 0.005 }, 
                                 { 0.005, 0.005 } } ;	
  static float temp1[2],temp2[2],temp3[2][2],temp4[2][2],temp5[2],temp6,temp7[2],temp8,temp9[2],temp10[2][2],temp11[2][2];
  static float X_estimate[2],P_estimate[2][2];
  static float Kg[2];
//��һ��������Ԥ�������״̬X_estimate//  
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			{
				temp1[i]+=A[i][j]*X_optimal[j];
			}
	for(i=0;i<2;i++)	temp2[i]=B[i]*gyro_m;
	X_estimate[0]=temp1[0]+temp2[0];
	X_estimate[1]=temp1[1]+temp2[1];
//�ڶ���������X_estimate��Э����//
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			{
				for(k=0;k<2;k++)
				temp3[i][j]+=A[i][k]*P_optimal[k][j];
			}
	
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			{
				for(k=0;k<2;k++)
				temp4[i][j]+=temp3[i][k]*A[j][k];
			}
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			P_estimate[i][j]=temp4[i][j]+Q[i][j];
//������������ó�����������//
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			temp5[i]+=H[j]*P_estimate[i][j];
			temp6=temp5[0]*H[0] +temp5[1]*H[1]+R;
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			temp7[i]+= P_estimate[i][j] *H[j];
			Kg[0]= temp7[0]/temp6;
			Kg[1]= temp7[1]/temp6;
//���Ĳ�����������״ֵ̬X_optimal//

	temp8=angle_m-(H[0]*X_estimate[1]+ H[1]*X_estimate[0]);
	temp9[0]=temp8*Kg[0];
	temp9[1]=temp8*Kg[1];
	X_optimal[0]=X_estimate[0]+temp9[0];
	X_optimal[1]=X_estimate[1]+temp9[1];
//���岽������ֵ��Э����//
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			temp10[i][j]=Kg[i]*H[j];
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			temp11[i][j]=1- temp10[i][j];

	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			{
				for(k=0;k<2;k++)
				P_optimal[i][j]+=temp11[i][k]*P_estimate[k][j];
			}
	return X_optimal;
}
****************************************************************/

