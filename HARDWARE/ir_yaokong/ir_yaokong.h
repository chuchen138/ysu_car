#ifndef __IR_YOAKONG_H
#define __IR_YOAKONG_H
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#define IR_IN PAin(1)
void ir_yaokong_init(void);//����ң�س�ʼ�������������ⲿ�ж�PA1����
void ir_decode(void);//�����ֵ������
uint8_t IrDa_Process(void);//��������Э����麯��
uint8_t Get_Pulse_Time(void);//�ߵ�ƽʱ���ȡ����
#endif 
