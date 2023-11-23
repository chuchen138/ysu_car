#ifndef __LED_H
#define __LED_H
#include "stm32f10x_tim.h"

/** ************************
  * @用途  初始化LED相关配置
  * @参数  void
  * @返回值 void
* ************************/
void LED_Init(void);

/** ************************
  * @用途  设置RGB灯的颜色
  * @参数  Compare：灯的亮度，取值为0-100，0为不亮，100最亮
  * @返回值 void
  * ************************/
void set_redLED(u8 Compare);
void set_blueLED(u8 Compare);
void set_greenLED(u8 Compare);
#endif 

