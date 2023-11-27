#include "ir_avoid.h"
#include "stm32f10x.h"


/** *************************
  * @用途  红外避障引脚初始化，
					 在main函数中调用
  * @参数  void
  * @返回值 void
	* @注:红外避障引脚检测见.h文件
				宏定义
 ** *************************/
void IRAVOID_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能gpio时钟，gpio c组的时钟

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

