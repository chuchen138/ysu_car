#ifndef __XUNGUANG_H
#define __XUNGUANG_H

#include "sys.h"

/** ************************
  * @用途  获取寻光模块状态 0为寻找到光 1为没有
* ************************/
#define XUNGUANG1 PBin(3)
#define XUNGUANG2 PAin(5)

/** ************************
  * @用途  初始化寻光模块
  * @参数  void
  * @返回值 0
* ************************/
void XUNGUANG_Init(void);
#endif 

