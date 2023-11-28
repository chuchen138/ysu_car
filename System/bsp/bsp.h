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
#include "motor.h"
#include "ir_avoid.h"
#include "ir_trac.h"
#include "ir_yaokong.h"
#include "adc.h"

void bsp_init(void);
void wait_for_setting(void);
void Other_Mode_Setting(void);
void Other_BLUETOOTH_Ctrl_Mode_Setting(void);


//void BSP_Obstacle_Avoidance(u32 speed,float upDist,float minDist);
void BSP_Obstacle_Avoidance(u32 speed,float upDist,float minDist);
void BSP_Purse_Light(void);
void BSP_IR_Trace(int speed, int l_r_speed);
void BSP_Dance(void);
#endif 
