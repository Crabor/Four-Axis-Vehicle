#include "MPU9150.h"

/**************************************
 * ��������MPU6050_Init
 * ����  ����ʼ��Mpu6050
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *************************************/
int MPU6050_Init(void)
{
    if (I2C_ByteRead(MPU6050_SlaveAddress, MPU6050_WHO_AM_I) != MPU6050_Device_ID) { //���MPU6050�Ƿ�����
        return 0;
    }
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_PWR_MGMT_1, 0x00); //�������״̬,ʹ���ڲ�8MHz����
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_SMPLRT_DIV, 0x00); //������Ƶ (����Ƶ�� = ���������Ƶ�� / (1+DIV)������Ƶ��1000hz��
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_CONFIG, 0x06);
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_INT_PIN_CFG, 0x02); //turn on Bypass Mode
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_USER_CTRL, 0x00); //close Master Mode
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_GYRO_CONFIG, 0x18); //������������+-2000��/�� (��ͷֱ��� = 2^15/2000 = 16.4LSB/��/��
    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_ACCEL_CONFIG, 0x08); //���ٶ�������+-4g   (��ͷֱ��� = 2^15/4g = 8196LSB/g )
    I2C_ByteWrite(AK8975_I2C_ADDR, AK8975_CNTL, 0x00);
    delay_ms(100);
    I2C_ByteWrite(AK8975_I2C_ADDR, AK8975_CNTL, 0x01);

    if (I2C_ByteRead(AK8975_I2C_ADDR, AK8975_WIA) != AK8975_Device_ID) { //���MPU6050�Ƿ�����
        return 0;
    }

    return 1;
}

/*************************************
 * ��������GetData_MPU6050
 * ����  �����16λ����
 * ����  ��REG_Address �Ĵ�����ַ
 * ���  �����ؼĴ�������
 * ����  ���ⲿ����
 ************************************/
uint16_t GetData_MPU6050(uint8_t REG_Address)
{
    uint8_t H, L;
    H = I2C_ByteRead(MPU6050_SlaveAddress, REG_Address);
    L = I2C_ByteRead(MPU6050_SlaveAddress, REG_Address + 1);
    return (H << 8) | L; //�ϳ�����
}

/*************************************
 * ��������GetData_AK8975_MAG
 * ����  �����16λ����
 * ����  ��REG_Address �Ĵ�����ַ
 * ���  �����ؼĴ�������
 * ����  ���ⲿ����
 ************************************/
uint16_t GetData_AK8975(uint8_t REG_Address)
{
    uint8_t H, L, err;

    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_INT_PIN_CFG, 0x02); //turn on Bypass Mode

    //	if(I2C_ByteRead (AK8975_I2C_ADDR,AK8975_ST1)==0x01){//�ж�����״̬
    //		L=I2C_ByteRead (AK8975_I2C_ADDR,REG_Address);
    //		I2C_ByteWrite(AK8975_I2C_ADDR,AK8975_CNTL,0x01);//�˴��ǳ��ؼ���
    //    //��Ϊ�ձ���˾�������ֲ���˵���ڵ��β���ģʽ�£�ÿ��ȡһ�Σ�
    //    //���Զ��ع�power down mode ,����������������Ϊ���β���ģʽ
    //		//https://blog.csdn.net/meker1/article/details/44342161
    //		H=I2C_ByteRead (AK8975_I2C_ADDR,REG_Address+1);
    //		I2C_ByteWrite(AK8975_I2C_ADDR,AK8975_CNTL,0x01);
    //	}
    I2C_ByteWrite(AK8975_I2C_ADDR, AK8975_CNTL, 0x01);
    //	while(I2C_ByteRead (AK8975_I2C_ADDR,AK8975_ST1)==0x00);
    delay_ms(8);
    L = I2C_ByteRead(AK8975_I2C_ADDR, REG_Address);
    err = I2C_ByteRead(AK8975_I2C_ADDR, AK8975_ST2) & 0x04;
    if (err)
        L = I2C_ByteRead(AK8975_I2C_ADDR, REG_Address);
    H = I2C_ByteRead(AK8975_I2C_ADDR, REG_Address + 1);

    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_INT_PIN_CFG, 0x00); //turn off Bypass Mode
    return (H << 8) | L; //�ϳ�
}
