#include "stm32f10x.h"
#include "led.h"
#include "key_beep.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "xunguang.h"
#include "sg_90.h"
#include "sr_04.h"
#include "bluetooth_wifi.h"
extern u8 key_flag;

int main()
{
    USART1_Config(115200);
    USART2_Init(115200);
    delay_init();
	//LED_Init();//gpio led初始化
    BEEP_Init();
    KEY_Init();
    XUNGUANG_Init();
    SG_90_Init();
    SR04_Init();
	while(1)
	{   
        u2printf("DISTANCE: %f \r\n",Get_SR04_Distance());
        printf("DISTANCE: %f \r\n",Get_SR04_Distance());
        //SG_90_SetDegree(0);
        delay_ms(500);
        //SG_90_SetDegree(90);
        delay_ms(500);
        //SG_90_SetDegree(180);
        delay_ms(500); 

	}
}
