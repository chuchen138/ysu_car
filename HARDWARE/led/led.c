#include "led.h"
#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能gpio时钟，gpio c组的时钟
		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}


void RGBLED_Init(void){
  GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//配置GPIO

	GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);


	GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF_PP; //推挽复用输出
	GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=99;       // ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=719;   //PCS 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);


	TIM_OCStructInit(&TIM_OCInitStruct); //初始化
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;  //CCR
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
    TIM_OC4Init(TIM3,&TIM_OCInitStruct);
	TIM_Cmd(TIM3,ENABLE);

    TIM_SetCompare2(TIM3,100);
    TIM_SetCompare3(TIM3,100);
    TIM_SetCompare4(TIM3,100);
}




void set_redLED(u8 Compare){
    if(Compare >= 100)
        Compare = 100;
    TIM_SetCompare3(TIM3,100-Compare);
}

void set_blueLED(u8 Compare){
    if(Compare >= 100)
        Compare = 100;
    TIM_SetCompare4(TIM3,100-Compare);
}

void set_greenLED(u8 Compare){
    if(Compare >= 100)
        Compare = 100;
    TIM_SetCompare2(TIM3,100-Compare);
}