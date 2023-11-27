#ifndef __IR_YOAKONG_H
#define __IR_YOAKONG_H
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#define IR_IN PAin(1)
void ir_yaokong_init(void);//红外遥控初始化函数，调用外部中断PA1触发
void ir_decode(void);//红外键值处理函数
uint8_t IrDa_Process(void);//红外数据协议核验函数
uint8_t Get_Pulse_Time(void);//高电平时间获取函数
#endif 
