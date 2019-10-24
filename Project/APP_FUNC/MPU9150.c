#include "MPU9150.h"
//��ΪMPU9150����Ĵ�����AK8975�����⣬���Ժ����MPU9150ΪGY86�ˣ�����Ĵ�����HMC5883L���ܽϺá�
//����������л�ͨ��AK8975_EN����������Ϊ�˱����鷳�����ļ�������ͳһ��MPU9150



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
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_PWR_MGMT_1, 0x00); //�������״̬,ʹ���ڲ�8MHz����
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_SMPLRT_DIV, 0x00); //������Ƶ (����Ƶ�� = ���������Ƶ�� / (1+DIV)������Ƶ��1000hz��
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_CONFIG, 0x06);
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_INT_PIN_CFG, 0x02); //turn on Bypass Mode
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_USER_CTRL, 0x00); //close Master Mode
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_GYRO_CONFIG, 0x18); //������������+-2000��/�� (��ͷֱ��� = 2^15/2000 = 16.4LSB/��/��
//     I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_ACCEL_CONFIG, 0x08); //���ٶ�������+-4g   (��ͷֱ��� = 2^15/4g = 8196LSB/g )

// #if AK8975_EN
//     I2C_ByteWrite(AK8975_I2C_ADDR, AK8975_CNTL, 0x00);
//     delay_ms(100);
//     I2C_ByteWrite(AK8975_I2C_ADDR, AK8975_CNTL, 0x01);

//     if (I2C_ByteRead(AK8975_I2C_ADDR, AK8975_WIA) != AK8975_Device_ID) { //���MPU6050�Ƿ�����
//         return 0;
//     }
// #endif

    return 1;
}

// HMC5883L initialization
void HMC5883L_Init(void)
{
    // Set the standard data output rate to 75HZ
    I2C_ByteWrite(HMC5883L_Addr, HMC5883L_ConfigurationRegisterA, 0x18);
    // Set the sampling frequency to +-1.3Ga
    I2C_ByteWrite(HMC5883L_Addr, HMC5883L_ConfigurationRegisterB, 0x20);
    // Turn on continuous measurement mode
    I2C_ByteWrite(HMC5883L_Addr, HMC5883L_ModeRegister, 0x00);
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
 * ��������GetData_HMC5883L
 * ����  �����16λ����
 * ����  ��REG_Address �Ĵ�����ַ
 * ���  �����ؼĴ�������
 * ����  ���ⲿ����
 ************************************/
uint16_t GetData_HMC5883L(uint8_t REG_Address){
    uint8_t H, L;
    H = I2C_ByteRead(HMC5883L_Addr, REG_Address);
    L = I2C_ByteRead(HMC5883L_Addr, REG_Address + 1);
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
    
    I2C_ByteWrite(AK8975_I2C_ADDR, AK8975_CNTL, 0x01);
    //	while(I2C_ByteRead (AK8975_I2C_ADDR,AK8975_ST1)==0x00);
    delay_ms(10);
    L = I2C_ByteRead(AK8975_I2C_ADDR, REG_Address);
    err = I2C_ByteRead(AK8975_I2C_ADDR, AK8975_ST2) & 0x04;
    if (err)
        L = I2C_ByteRead(AK8975_I2C_ADDR, REG_Address);
    H = I2C_ByteRead(AK8975_I2C_ADDR, REG_Address + 1);

    I2C_ByteWrite(MPU6050_SlaveAddress, MPU6050_INT_PIN_CFG, 0x00); //turn off Bypass Mode
    return (H << 8) | L; //�ϳ�
}
