#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"
#define AIN1 PBout(8)
#define AIN2 PBout(9)
#define BIN1 PBout(5)
#define BIN2 PBout(4)
void motor_init(void);//�������ų�ʼ��
void pwm_init(void);
void forward(int speed);//ǰ�����Ƽ�PWM���� speed400ת1000���
void backward(int speed);//����
void left(int speed);//��ת
void right(int speed);//��ת
void stop(void);//ֹͣ
#endif 

