#ifndef USART_H
#define USART_H
#include "includes.h"
//������λ��-https://blog.csdn.net/wangjt1988
//����ԭ�ӡ�����STM32F4����ָ��-�Ĵ����汾_V1.1��

//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp) (*((char*)(&dwTemp)))
#define BYTE1(dwTemp) (*((char*)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char*)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char*)(&dwTemp) + 3))

//Variable declaration
extern u8 sendBuf[50]; //�������ݻ���

//��������
// void USART2_Init(u32 pclk2, u32 bound);
void USART6_Init(u32 pclk2, u32 bound);
void USART6_IRQ(void);
void USART6_ItSend(unsigned char* DataToSend, u8 data_num);
void USART6_NItSend(unsigned char* DataToSend, u8 data_num);
void SendSenser(int16_t ACCEL_X, int16_t ACCEL_Y, int16_t ACCEL_Z, int16_t GYRO_X, int16_t GYRO_Y, int16_t GYRO_Z, int16_t MAG_X, int16_t MAG_Y, int16_t MAG_Z);
void SendAttitude(float roll, float pitch, float yaw);
void SendStr(const char* str);
void SendByte(u8 frame, u8* p);
void SendHalfWord(u8 frame, u16* p);
void SendWord(u8 frame, u32* p);
void SendPWMIN(u8 frame, u8* STA, u16* OVF, u16* VAL_UP, u16* VAL_DOWN, u16* PW);
#endif
