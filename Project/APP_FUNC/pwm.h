#ifndef PWM_H
#define PWM_H
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_tim.h"
//#include "gpio.h"
//#include "nvic.h"
#include "includes.h"

//���벶��ʱ����TIM5��Ƶ��Ϊ 1MHz ,�� 1/1M ���������1��
//��һ�������ز����½��ز����ʱ��Ϊ x/1M �룬xΪ���β��������ֵ���
//��ң������������PWMƵ��Ϊ 54.27Hz ����һ��PWM����ʱ��Ϊ 1/54.27 ��
//�����ʱ�����ռ�ձ�ΪD���� D / 54.27 = x / 1M => x = 1000000D / 54.27
//����ΪPWM�����ʱ����TIM3��arrΪ1000��Ϊ�������ռ�ձȺ������һ�����������CCRxֵΪ 1000D
//���� x * PWM_IN_TO_OUT = 1000D => PWM_IN_TO_OUT = 1000D * 54.27 / 1000000D = 0.05427
#define PWM_IN_TO_OUT 0.05427f

void PWM_IN_INIT(void);
void PWM_OUT_INIT(void);
void TIM5_PWM_IN_IRQ(void);

#endif
