#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

int main()
{
    USART1_Config(115200);
    delay_init();
	LED_GPIO_Init();//gpio led初始化
	while(1)
	{
        printf("Hello world!\r\n");//燕过留声
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);//输出低电平
        delay_ms(1000);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);//
        delay_ms(1000);
	}
}
