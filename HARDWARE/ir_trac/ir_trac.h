#ifndef __IR_TRAC_H
#define __IR_TRAC_H
#include "sys.h"
#define trac_right1 PCin(14)//车右侧中间
#define trac_right2 PCin(13)//车右侧靠右
#define trac_left1 PCin(15)//车左侧中间
#define trac_left2 PBin(10)//车左侧靠左
void IRTRAC_GPIO_Init(void);//红外寻迹引脚初始化

#endif 

