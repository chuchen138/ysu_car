#include "stm32f10x.h"
#include "led.h"
#include "key_beep.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "xunguang.h"
extern u8 key_flag;

int main()
{
    USART1_Config(115200);
    delay_init();
	//LED_Init();//gpio led初始化
    BEEP_Init();
    KEY_Init();
    XUNGUANG_Init();

	while(1)
	{   
        printf("statusA5: %d \r\n", XUNGUANG2);
        printf("statusB3: %d \r\n", XUNGUANG1);
        delay_ms(500);
	}
}
