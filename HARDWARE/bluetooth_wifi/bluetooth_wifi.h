#ifndef __BLUETOOTH_WIFI_H
#define __BLUETOOTH_WIFI_H

#include "usart.h"

#include "stdarg.h"
#include "string.h"

void USART2_Init(u32 bound);
void u2printf(char* fmt, ...);
#endif 

