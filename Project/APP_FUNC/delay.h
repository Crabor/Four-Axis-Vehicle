#ifndef __DELAY_H_
#define __DELAY_H_

#include "includes.h"
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
