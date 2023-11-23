#ifndef __KEY_BEEP_H
#define __KEY_BEEP_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"


#define BEEP PBout(11)
#define KEY  PAin(8)


extern u8 key_flag;


/** ************************
  * @用途  按键初始化
  * @参数  void
  * @返回值 void
* ************************/
void KEY_Init(void);
/** ************************
  * @用途  获取按键状态
  * @参数  void
  * @返回值 u8 表明按键当前状态，单片机Reset后，
  *           默认值为1，接下来每按一下，值就会
  *           被翻转。例： 初始值返回为1 按一下 返回0 再按一下 返回1
* ************************/
u8 get_key_flag(void);
/** ************************
  * @用途  蜂鸣器初始化
  * @参数  void
  * @返回值 void
* ************************/
void BEEP_Init(void);

/** ************************
  * @用途  启动蜂鸣器
  * @参数  msec 为蜂鸣器响的时间 单位为ms
  * @返回值 void
* ************************/
void BEEP_on(u8 msec);
#endif 

