#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"
#define AIN1 PBout(8)
#define AIN2 PBout(9)
#define BIN1 PBout(5)
#define BIN2 PBout(4)
void motor_init(void);//电机控制引脚初始化
void pwm_init(void);//电机pwm配置
void forward(int speed);//前进
void backward(int speed);//后退
void left(int speed);//左转
void right(int speed);//右转
void stop(void);//停止
#endif 

