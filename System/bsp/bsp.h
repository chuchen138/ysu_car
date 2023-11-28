#ifndef __BSP_H
#define __BSP_H

#include "stm32f10x.h"
#include "led.h"
#include "key_beep.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "xunguang.h"
#include "sg_90.h"
#include "sr_04.h"
#include "bluetooth_wifi.h"


void bsp_init(void);
void wait_for_setting(void);
void Other_Mode_Setting(void);
void Other_BLUETOOTH_Ctrl_Mode_Setting(void);
#endif 
