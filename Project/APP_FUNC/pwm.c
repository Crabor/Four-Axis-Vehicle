#include "pwm.h"

void PWM_OUT_INIT(void){
	uint16_t arr = 999;
	uint16_t psc = 1547;//54.27hz �� PWM
//	//uint32_t lowPulse=54;
//	
	RCC->APB1ENR|=1<<1; //TIM3 ʱ��ʹ��
	TIM3->CR1&=~(3<<5);//���ض���ģʽ
	TIM3->CR1&=~(1<<4);//��������������
	TIM3->SMCR &= ~(7<<0);//ʱ����Դ����Ϊ�ڲ�ʱ��             /////////////////////
	TIM3->ARR=arr; //�趨�������Զ���װֵ
	TIM3->PSC=psc; //Ԥ��Ƶ������Ƶ
	
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
	
	//TIM3��ͨ��һ������ʼ��
	RCC->AHB1ENR|=1<<0; //ʹ�� PORTA ʱ��
	GPIO_Set(GPIOA,GPIO_Pin_6,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Fast_Speed,GPIO_PuPd_NOPULL);//���ù���,�������������������
	GPIO_Set(GPIOA,GPIO_Pin_7,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Fast_Speed,GPIO_PuPd_NOPULL);//���ù���,�������������������
	GPIO_AF_Set(GPIOA,6,2); //PA6,AF2
	GPIO_AF_Set(GPIOA,7,2); //PA7,AF2
	TIM3->CCMR1&=~(3<<0);//CH1 ����Ϊ���ģʽ            ////////////////////////
	TIM3->CCMR1|=6<<4; //CH1 PWM1 ģʽ
	TIM3->CCMR1|=1<<3; //CH1 Ԥװ��ʹ��
	TIM3->CCER|=1<<0; //OC1 ���ʹ��
	TIM3->CCER&=~(1<<1); //OC1 �ߵ�ƽ��Ч
	//TIM3->CCER|=1<<1; //OC1 �͵�ƽ��Ч
	TIM3->CCMR1&=~(3<<8);//CH2 ����Ϊ���ģʽ            ////////////////////////
	TIM3->CCMR1|=6<<12; //CH2 PWM1 ģʽ
	TIM3->CCMR1|=1<<11; //CH2 Ԥװ��ʹ��
	TIM3->CCER|=1<<4; //OC2 ���ʹ��
	TIM3->CCER&=~(1<<5); //OC2 �ߵ�ƽ��Ч
	//TIM3->CCER|=1<<5; //OC2 �͵�ƽ��Ч
	
	//TIM3��ͨ�������ĳ�ʼ��
	RCC->AHB1ENR|=1<<1; //ʹ�� PORTB ʱ��
	GPIO_Set(GPIOB,GPIO_Pin_0,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Fast_Speed,GPIO_PuPd_NOPULL);//���ù���,�������������������
	GPIO_Set(GPIOB,GPIO_Pin_1,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Fast_Speed,GPIO_PuPd_NOPULL);//���ù���,�������������������
	GPIO_AF_Set(GPIOB,0,2); //PB0,AF2
	GPIO_AF_Set(GPIOB,1,2); //PB1,AF2
	TIM3->CCMR2&=~(3<<0);//CH3 ����Ϊ���ģʽ            ////////////////////////
	TIM3->CCMR2|=6<<4; //CH3 PWM1 ģʽ
	TIM3->CCMR2|=1<<3; //CH3 Ԥװ��ʹ��
	TIM3->CCER|=1<<8; //OC3 ���ʹ��
	TIM3->CCER&=~(1<<9); //OC3 �ߵ�ƽ��Ч
	//TIM3->CCER|=1<<9; //OC3 �͵�ƽ��Ч
	TIM3->CCMR2&=~(3<<8);//CH2 ����Ϊ���ģʽ            ////////////////////////
	TIM3->CCMR2|=6<<12; //CH2 PWM1 ģʽ
	TIM3->CCMR2|=1<<11; //CH2 Ԥװ��ʹ��
	TIM3->CCER|=1<<12; //OC4 ���ʹ��
	TIM3->CCER&=~(1<<13); //OC4 �ߵ�ƽ��Ч	
	//TIM3->CCER|=1<<13; //OC4 �͵�ƽ��Ч
	
	
	
	TIM3->CR1|=1<<7; //ARPE ʹ��
	TIM3->CR1|=1<<0; //ʹ�ܶ�ʱ�� 3

//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//TIM3 ʱ��ʹ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ�� PORTA ʱ��
//	
//	GPIO_Set(GPIOA,GPIO_Pin_6,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Fast_Speed,GPIO_PuPd_NOPULL);//���ù���,�������������������
//	GPIO_AF_Set(GPIOA,6,2); //PA6,AF2
//	
//	TIM_TimeBaseStructure.TIM_Prescaler=psc; //��ʱ����Ƶ
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseStructure.TIM_Period=arr; //�Զ���װ��ֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ�� 3
//	
//	//��ʼ�� TIM3 Channel1 PWM ģʽ
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM ����ģʽ 1
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ե�
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure); //��ʼ������ TIM3 OC1
//	
//	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
//	
//	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE ʹ��
//	TIM_Cmd(TIM3, ENABLE); //ʹ�� TIM3
}

void TIM5_CH1_Cap_Init(void){
	uint32_t arr = 0xffffffff;
	uint16_t psc = 83;//��1Mhz��Ƶ�ʼ���
	
	RCC->APB1ENR|=1<<3; //TIM5 ʱ��ʹ��
	RCC->AHB1ENR|=1<<0; //ʹ�� PORTA ʱ��
	//GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);//���ù���,����
	GPIO_Set(GPIOA,GPIO_Pin_0,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Fast_Speed,GPIO_PuPd_DOWN);
	GPIO_AF_Set(GPIOA,0,2); //PA0,AF2
	TIM5->ARR=arr; //�趨�������Զ���װֵ
	TIM5->PSC=psc; //Ԥ��Ƶ��
	TIM5->CCMR1|=1<<0; //CC1S=01 ѡ������� IC1 ӳ�䵽 TI1 ��
	TIM5->CCMR1|=0<<4; //IC1F=0000 ���������˲��� ���˲�
	TIM5->CCMR1|=0<<10; //IC1PS=00 ���������Ƶ,����Ƶ
	TIM5->CCER|=0<<1; //CC1P=0 �����ز���
	TIM5->CCER|=1<<0; //CC1E=1 �������������ֵ������Ĵ�����
	TIM5->EGR=1<<0; //������Ʋ��������¼�,ʹд�� PSC ��ֵ������Ч,
	//���򽫻�Ҫ�ȵ���ʱ������Ż���Ч!
	TIM5->DIER|=1<<1; //������ 1 �ж�
	TIM5->DIER|=1<<0; //��������ж�
	TIM5->CR1|=0x01; //ʹ�ܶ�ʱ�� 2
	MY_NVIC_Init(2,0,TIM5_IRQn,2);//��ռ 2�������ȼ� 0���� 2
}

