#include "IIC_Init.h"
#include "stm32f4xx_i2c.h"

void I2C_Congiguration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
  I2C_InitTypeDef  I2C_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOB,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIO_Pin_6 | GPIO_Pin_7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	  /* I2C ���� */
	I2C_DeInit(I2C1);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
 // I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
 // I2C_InitStructure.I2C_OwnAddress1 = 0xd0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* ʹ�� I2C1 */
  I2C_Cmd(I2C1, ENABLE);

  /* I2C1 ��ʼ�� */
  I2C_Init(I2C1, &I2C_InitStructure);

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C1, ENABLE); 
	
}


/******************************************
 * ��������I2C_ByteWrite
 * ����  ��дһ���ֽڵ�I2C�豸�Ĵ�����
 * ����  ��SlaveAddressд������������ַ,REG_Address �������ݵ�IIC�豸�Ĵ����ĵ�ַ 
 *         REG_data ��д�������
 * ���  ����
 * ����  ����
 * ����  ���ڲ�����
 ******************************************/	
void I2C_ByteWrite(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{

I2C_GenerateSTART(I2C1,ENABLE);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

I2C_SendData(I2C1,REG_Address);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_SendData(I2C1,REG_data);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTOP(I2C1,ENABLE);

}


/******************************************
 * ��������I2C_ByteWrite_Ms5611
 * ����  ��дһ���ֽڵ�I2C�豸�Ĵ�����
 * ����  ��SlaveAddressд������������ַ,COMMANDS �������ݵ�IIC�豸����
 * ���  ����
 * ����  ����
 * ����  ���ڲ�����
 ******************************************/	
void I2C_ByteWrite_Ms5611(uint8_t SlaveAddress,uint8_t Commands)
{

I2C_GenerateSTART(I2C1,ENABLE);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

I2C_SendData(I2C1,Commands);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//I2C_SendData(I2C1,REG_data);

//while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTOP(I2C1,ENABLE);

}


/***********************************************
 * ��������I2C_ByteRead
 * ����  ����IIC�豸�Ĵ����ж�ȡһ���ֽ�
 * ����  ��SlaveAddressд������������ַ,REG_Address ��ȡ���ݵļĴ����ĵ�ַ 
 * ���  ����
 * ����  ����
 * ����  ���ڲ����� 
************************************************/
uint8_t I2C_ByteRead(uint8_t SlaveAddress,uint8_t REG_Address)
{
uint8_t REG_data;

while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));

I2C_GenerateSTART(I2C1,ENABLE);//��ʼ�ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);//�����豸��ַ+д�ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//

I2C_Cmd(I2C1,ENABLE);

I2C_SendData(I2C1,REG_Address);//���ʹ洢��Ԫ��ַ����0��ʼ

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTART(I2C1,ENABLE);//��ʼ�ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Receiver);//�����豸��ַ+���ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

I2C_AcknowledgeConfig(I2C1,DISABLE);

I2C_GenerateSTOP(I2C1,ENABLE);

while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));

REG_data=I2C_ReceiveData(I2C1);//�����Ĵ�������

return REG_data;

}


/***********************************************
 * ��������I2C_ByteRead_Ms5611_16BIT
 * ����  ����IIC�豸�Ĵ����ж�ȡ2���ֽ�
 * ����  ��REG_Address ��ȡ���ݵļĴ����ĵ�ַ 
 * ���  ����
 * ����  ����
 * ����  ���ڲ����� 
************************************************/
uint16_t I2C_ByteRead_Ms5611_16BIT(uint8_t REG_Address)
{
uint16_t REG_data;

while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));

I2C_GenerateSTART(I2C1,ENABLE);//��ʼ�ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,REG_Address,I2C_Direction_Receiver);//�����豸��ַ+���ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

I2C_AcknowledgeConfig(I2C1,ENABLE);
	
while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));

REG_data=I2C_ReceiveData(I2C1);//�����Ĵ�������	
	
I2C_AcknowledgeConfig(I2C1,ENABLE);

while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));	
	
REG_data=(REG_data<<8)|I2C_ReceiveData(I2C1);	//�����Ĵ�������

I2C_AcknowledgeConfig(I2C1,DISABLE);

I2C_GenerateSTOP(I2C1,ENABLE);

return REG_data;

}



/***********************************************
 * ��������I2C_ByteRead_Ms5611_24BIT
 * ����  ����IIC�豸�Ĵ����ж�ȡ3���ֽ�
 * ����  ��REG_Address ��ȡ���ݵļĴ����ĵ�ַ 
 * ���  ����
 * ����  ����
 * ����  ���ڲ����� 
************************************************/
uint32_t I2C_ByteRead_Ms5611_24BIT(uint8_t REG_Address)
{
uint32_t REG_data;

while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));

I2C_GenerateSTART(I2C1,ENABLE);//��ʼ�ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,REG_Address,I2C_Direction_Receiver);//�����豸��ַ+���ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

I2C_AcknowledgeConfig(I2C1,ENABLE);
	
while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));

REG_data=I2C_ReceiveData(I2C1);//�����Ĵ�������	
	
I2C_AcknowledgeConfig(I2C1,ENABLE);

while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));	
	
REG_data=(REG_data<<8)|I2C_ReceiveData(I2C1);	//�����Ĵ�������	

I2C_AcknowledgeConfig(I2C1,ENABLE);

while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));

REG_data=(REG_data<<8)|I2C_ReceiveData(I2C1);	//�����Ĵ�������	

I2C_AcknowledgeConfig(I2C1,DISABLE);

I2C_GenerateSTOP(I2C1,ENABLE);

return REG_data;

}




