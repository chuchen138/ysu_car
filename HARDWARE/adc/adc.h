#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#include "delay.h"
void  ADC_init(void);//ADC初始化
float Get_ADC(void);//获取单次adc值
float Get_ADC_Average(u8 times);//多次获取取均值
#endif 
