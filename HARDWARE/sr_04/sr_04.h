#ifndef __SR04_H
#define __SR04_H
#include "sys.h"
#include "delay.h"

#define SR04_TRIG PAout(15)// PB12
#define SR04_ECHO PAin(12)// PB13


/** ************************
  * @用途  超声波测距模块初始化
  * @参数  void
  * @返回值 void
* ************************/
void SR04_Init(void);

/** ************************
  * @用途  获取超声波测距模块测得的距离
  * @参数  void
  * @返回值 float 单位为厘米
* ************************/
float Get_SR04_Distance(void);

#endif 

