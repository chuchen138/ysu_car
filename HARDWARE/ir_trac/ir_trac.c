#include "ir_trac.h"
#include "stm32f10x.h"
/** *************************
  * @用途  红外寻迹引脚初始化，
					 在main函数中调用
  * @参数  void
  * @返回值 void
	* @注:红外寻迹引脚检测见.h文
				件宏定义
 ** *************************/
void IRTRAC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB,ENABLE);//使能gpio时钟，gpio c组的时钟
		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
