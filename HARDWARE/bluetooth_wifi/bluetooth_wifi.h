#ifndef __BLUETOOTH_WIFI_H
#define __BLUETOOTH_WIFI_H

#include "usart.h"

#include "stdarg.h"
#include "string.h"
/** ************************
  * @用途  初始化串口2
  * @参数  bound 波特率 115200
  * @返回值 void
  * ************************/
void USART2_Init(u32 bound);
void u2printf(char* fmt, ...);
#endif 

