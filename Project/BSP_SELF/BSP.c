#include "includes.h"

void BSP_Init(void){
    SysTick_Init();
	DELAY_Init(84);
	USART6_Init(84,115200);
	IIC_Init();
	while(MPU6050_Init()!=1);//��MPU6050��ʼ�����ɹ����������������;
//	Calculate_FilteringCoefficient(0.001f,10.f);//����IIR�˲�������
//	PWM_OUT_Init();
//	PWM_IN_Init();
}

void SysTick_Init(void){
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency/OS_TICKS_PER_SEC);
}

