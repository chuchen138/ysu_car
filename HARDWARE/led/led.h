#ifndef __LED_H
#define __LED_H
#include "stm32f10x_tim.h"

void LED_Init(void);//LED初始化
void set_redLED(u8 Compare);
void set_blueLED(u8 Compare);
void set_greenLED(u8 Compare);
#endif 

