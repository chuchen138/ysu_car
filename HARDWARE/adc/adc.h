#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#include "delay.h"
void  ADC_init(void);//ADC��ʼ��
float Get_ADC(void);//��ȡ����adcֵ
float Get_ADC_Average(u8 times);//��λ�ȡȡ��ֵ
#endif 
