#include "iic.h"

/******************************************************************************
* ��  ����void IIC_Init(void)
* �����ܣ�ģ��IIC���ų�ʼ��
* ��  ������
* ����ֵ����
* ��  ע��PB6->SCL	PB7->SDA
*******************************************************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    // Enable GPIOB clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // GPIO configuration, Open-drain output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
* ��  ����void SDA_OUT(void)
* �����ܣ�����ģ��IIC SDA����Ϊ���
* ��  ������
* ����ֵ����
* ��  ע����
*******************************************************************************/
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
* ��  ����void SDA_IN(void)
* �����ܣ�����ģ��IIC SDA����Ϊ����
* ��  ������
* ����ֵ����
* ��  ע����
*******************************************************************************/
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
* ��  ����void IIC_Start(void)
* �����ܣ�����IIC��ʼ�ź�
* ��  ������
* ����ֵ����
* ��  ע����
*******************************************************************************/
void IIC_Start(void)
{
    SDA_OUT(); //SDA�����
    SDA_H;
    SCL_H;
    delay_us(4);
    SDA_L;
    delay_us(4);
    SCL_L;
}

/******************************************************************************
* ��  ����void IIC_Stop(void)
* �����ܣ�����IICֹͣ�ź�
* ��  ������
* ����ֵ����
* ��  ע����
*******************************************************************************/
void IIC_Stop(void)
{
    SDA_OUT(); //SDA�����
    SCL_L;
    SDA_L;
    delay_us(4);
    SCL_H;
    SDA_H;
    delay_us(4);
}

/******************************************************************************
* ��  ��: uint8_t IIC_WaitAck(void)
* ������: �ȴ�Ӧ���źŵ��� ����ЧӦ�𣺴ӻ���9�� SCL=0 ʱ SDA ���ӻ�����,
*         ���� SCL = 1ʱ SDA��ȻΪ�ͣ�
* ��  ������
* ����ֵ��1������Ӧ��ʧ��	0������Ӧ��ɹ�
* ��  ע���ӻ���������Ӧ��
*******************************************************************************/
uint8_t IIC_WaitAck(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN(); //SDA����Ϊ����  ���ӻ���һ���͵�ƽ��ΪӦ��
    SDA_H;
    delay_us(1);
    SCL_H;
    delay_us(1);
    ;
    while (SDA_read) {
        ucErrTime++;
        if (ucErrTime > 250) {
            IIC_Stop();
            return 1;
        }
    }
    SCL_L; //ʱ�������
    return 0;
}

/******************************************************************************
* ��  ��: void IIC_Ack(void)
* ������: ����ACKӦ�� ������������һ���ֽ����ݺ�����������ACK֪ͨ�ӻ�һ��
*         �ֽ���������ȷ���գ�
* ��  ������
* ����ֵ����
* ��  ע���������ӻ���Ӧ��
*******************************************************************************/
void IIC_Ack(void)
{
    SCL_L;
    SDA_OUT();
    SDA_L;
    delay_us(1);
    SCL_H;
    delay_us(2);
    SCL_L;
}

/******************************************************************************
* ��  ��: void IIC_NAck(void)
* ������: ����NACKӦ�� ���������������һ���ֽ����ݺ�����������NACK֪ͨ�ӻ�
*         ���ͽ������ͷ�SDA,�Ա���������ֹͣ�źţ�
* ��  ������
* ����ֵ����
* ��  ע���������ӻ���Ӧ��
*******************************************************************************/
void IIC_NAck(void)
{
    SCL_L;
    SDA_OUT();
    SDA_H;
    delay_us(1);
    SCL_H;
    delay_us(1);
    SCL_L;
}

/******************************************************************************
* ��  ����void IIC_SendByte(uint8_t data)
* ��  �ܣ�IIC����һ���ֽ�
* ��  ����data Ҫд������
* ����ֵ����
* ��  ע���������ӻ���
*******************************************************************************/
void IIC_SendByte(uint8_t data)
{
    uint8_t t;
    SDA_OUT();
    SCL_L; //����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++) {
        if (data & 0x80)
            SDA_H;
        else
            SDA_L;
        data <<= 1;
        delay_us(1);
        SCL_H;
        delay_us(1);
        SCL_L;
        delay_us(1);
    }
}

/******************************************************************************
* ��  ����uint8_t IIC_ReadByte(uint8_t ack)
* ��  �ܣ�IIC��ȡһ���ֽ�
* ��  ����ack=1 ʱ���������ݻ�û������ ack=0 ʱ����������ȫ���������
* ����ֵ����
* ��  ע���ӻ���������
*******************************************************************************/
uint8_t IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    SDA_IN(); //SDA����Ϊ����ģʽ �ȴ����մӻ���������
    for (i = 0; i < 8; i++) {
        SCL_L;
        delay_us(1);
        SCL_H;
        receive <<= 1;
        if (SDA_read)
            receive++; //�ӻ����͵ĵ�ƽ
        delay_us(1);
    }
    if (ack)
        IIC_Ack(); //����ACK
    else
        IIC_NAck(); //����nACK
    return receive;
}

/******************************************************************************
* ��  ����uint8_t I2C_ByteRead(uint8_t I2C_Addr,uint8_t addr)
* �����ܣ���ȡָ���豸 ָ���Ĵ�����һ��ֵ
* ��  ����I2C_Addr  Ŀ���豸��ַ
		  reg	    �Ĵ�����ַ
          *buf      ��ȡ����Ҫ�洢�ĵ�ַ    
* ����ֵ������ 1ʧ�� 0�ɹ�
* ��  ע����
*******************************************************************************/
uint8_t I2C_ByteRead(uint8_t I2C_Addr, uint8_t reg)
{
    uint8_t ret;
    IIC_Start();
    IIC_SendByte(I2C_Addr); //���ʹӻ���ַ
    if (IIC_WaitAck()) //����ӻ�δӦ�������ݷ���ʧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();

    IIC_Start();
    IIC_SendByte(I2C_Addr + 1); //�������ģʽ
    IIC_WaitAck();
    ret = IIC_ReadByte(0);
    IIC_Stop(); //����һ��ֹͣ����
    return ret;
}

/*************************************************************************************
* ��  ����uint8_t I2C_ByteWrite(uint8_t I2C_Addr,uint8_t addr��uint8_t buf))
* �����ܣ�д��ָ���豸 ָ���Ĵ�����һ��ֵ
* ��  ����I2C_Addr  Ŀ���豸��ַ
		  reg	    �Ĵ�����ַ
          buf       Ҫд�������
* ����ֵ��1 ʧ�� 0�ɹ�
* ��  ע����
**************************************************************************************/
uint8_t I2C_ByteWrite(uint8_t I2C_Addr, uint8_t reg, uint8_t data)
{
    IIC_Start();
    IIC_SendByte(I2C_Addr); //���ʹӻ���ַ
    if (IIC_WaitAck()) {
        IIC_Stop();
        return 1; //�ӻ���ַд��ʧ��
    }
    IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();
    IIC_SendByte(data);
    if (IIC_WaitAck()) {
        IIC_Stop();
        return 1; //����д��ʧ��
    }
    IIC_Stop(); //����һ��ֹͣ����
    return 0;
}

/***************************************************************************************
* ��  ����uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
* �����ܣ���ȡָ���豸 ָ���Ĵ����� length��ֵ
* ��  ����dev     Ŀ���豸��ַ
		  reg	  �Ĵ�����ַ
          length  Ҫ�����ֽ���
		  *data   ���������ݽ�Ҫ��ŵ�ָ��
* ����ֵ��1�ɹ� 0ʧ��
* ��  ע����
***************************************************************************************/
uint8_t I2C_MultByteRead(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    uint8_t count = 0;
    uint8_t temp;
    IIC_Start();
    IIC_SendByte(dev); //���ʹӻ���ַ
    if (IIC_WaitAck()) {
        IIC_Stop();
        return 1; //�ӻ���ַд��ʧ��
    }
    IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(dev + 1); //�������ģʽ
    IIC_WaitAck();
    for (count = 0; count < length; count++) {
        if (count != (length - 1))
            temp = IIC_ReadByte(1); //��ACK�Ķ�����
        else
            temp = IIC_ReadByte(0); //���һ���ֽ�NACK

        data[count] = temp;
    }
    IIC_Stop(); //����һ��ֹͣ����
    return 0;
}

/****************************************************************************************
* ��  ����uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
* �����ܣ�������ֽ�д��ָ���豸 ָ���Ĵ���
* ��  ����dev     Ŀ���豸��ַ
		  reg	  �Ĵ�����ַ
		  length  Ҫд���ֽ���
		  *data   Ҫд������ݽ�Ҫ��ŵ�ָ��
* ����ֵ��1�ɹ� 0ʧ��
* ��  ע����
****************************************************************************************/
uint8_t I2C_MultByteWrite(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    uint8_t count = 0;
    IIC_Start();
    IIC_SendByte(dev); //���ʹӻ���ַ
    if (IIC_WaitAck()) {
        IIC_Stop();
        return 1; //�ӻ���ַд��ʧ��
    }
    IIC_SendByte(reg); //���ͼĴ�����ַ
    IIC_WaitAck();
    for (count = 0; count < length; count++) {
        IIC_SendByte(data[count]);
        if (IIC_WaitAck()) //ÿһ���ֽڶ�Ҫ�ȴӻ�Ӧ��
        {
            IIC_Stop();
            return 1; //����д��ʧ��
        }
    }
    IIC_Stop(); //����һ��ֹͣ����
    return 0;
}

// void IIC_Init(void)
// {
// GPIO_InitTypeDef GPIO_InitStructure;
// I2C_InitTypeDef I2C_InitStructure;

// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOB, ENABLE);

// RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

// GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
// GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIO_Pin_6 | GPIO_Pin_7
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

// GPIO_Init(GPIOB, &GPIO_InitStructure);

// /* I2C ���� */
// I2C_DeInit(I2C1);
// I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
// // I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
// // I2C_InitStructure.I2C_OwnAddress1 = 0xd0;
// I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
// I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
// I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

// /* ʹ�� I2C1 */
// I2C_Cmd(I2C1, ENABLE);

// /* I2C1 ��ʼ�� */
// I2C_Init(I2C1, &I2C_InitStructure);

// /*����1�ֽ�1Ӧ��ģʽ*/
// I2C_AcknowledgeConfig(I2C1, ENABLE);
// }

// /******************************************
//  * ��������I2C_ByteWrite
//  * ����  ��дһ���ֽڵ�I2C�豸�Ĵ�����
//  * ����  ��SlaveAddressд������������ַ,REG_Address �������ݵ�IIC�豸�Ĵ����ĵ�ַ
//  *         REG_data ��д�������
//  * ���  ����
//  * ����  ����
//  * ����  ���ڲ�����
//  ******************************************/
// void I2C_ByteWrite(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
// {

//     I2C_GenerateSTART(I2C1,ENABLE);

//     while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

//     I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

//     while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//     I2C_SendData(I2C1,REG_Address);

//     while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//     I2C_SendData(I2C1,REG_data);

//     while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//     I2C_GenerateSTOP(I2C1,ENABLE);
//     // int dummy;

//     // /* Send STRAT condition */

//     // I2C_GenerateSTART(I2C1, ENABLE);

//     // /* Test on EV5 and clear it */

//     // while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
//     //     ;

//     // /* Send EEPROM address for write */

//     // dummy = I2C1->SR1;

//     // I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);

//     // /* Test on EV6 and clear it */

//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//     //     ;

//     // /* Send the EEPROM's internal address to write to */

//     // I2C_SendData(I2C1, REG_Address);

//     // /* Test on EV8 and clear it */

//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//     //     ;

//     // /* Send the byte to be written */

//     // I2C_SendData(I2C1, REG_data);

//     // /* Test on EV8 and clear it */

//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//     //     ;

//     // /* Send STOP condition */

//     // I2C_GenerateSTOP(I2C1, ENABLE);
// }

// /***********************************************
//  * ��������I2C_ByteRead
//  * ����  ����IIC�豸�Ĵ����ж�ȡһ���ֽ�
//  * ����  ��SlaveAddressд������������ַ,REG_Address ��ȡ���ݵļĴ����ĵ�ַ
//  * ���  ����
//  * ����  ����
//  * ����  ���ڲ�����
// ************************************************/
// uint8_t I2C_ByteRead(uint8_t SlaveAddress, uint8_t REG_Address)
// {
//     uint8_t REG_data;
//     while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//         ;
//     I2C_GenerateSTART(I2C1, ENABLE); //��ʼ�ź�
//     while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//         ;
//     I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter); //�����豸��ַ+д�ź�
//     while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//         ; //
//     I2C_Cmd(I2C1, ENABLE);
//     I2C_SendData(I2C1, REG_Address); //���ʹ洢��Ԫ��ַ����0��ʼ
//     while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//         ;
//     I2C_GenerateSTART(I2C1, ENABLE); //��ʼ�ź�
//     while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//         ;
//     I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Receiver); //�����豸��ַ+���ź�
//     while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//         ;
//     I2C_AcknowledgeConfig(I2C1, DISABLE);
//     I2C_GenerateSTOP(I2C1, ENABLE);
//     while (!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)))
//         ;
//     REG_data = I2C_ReceiveData(I2C1); //�����Ĵ�������

//     return REG_data;
//     // uint8_t data;
//     // I2C_GenerateSTART(I2C1, ENABLE);
//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//     //     ;
//     // I2C_AcknowledgeConfig(I2C1, DISABLE);
//     // I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//     //     ;
//     // I2C_SendData(I2C1, REG_Address);
//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//     //     ;
//     // I2C_GenerateSTART(I2C1, ENABLE);
//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//     //     ;
//     // I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Receiver);
//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//     //     ;
//     // I2C_GenerateSTOP(I2C1, ENABLE);
//     // while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
//     //     ;
//     // data = I2C_ReceiveData(I2C1);

//     // return data;
// }
