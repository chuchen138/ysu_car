#include    "led.h"
#include	"stm32f10x.h"
void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能gpio时钟，gpio c组的时钟
		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

