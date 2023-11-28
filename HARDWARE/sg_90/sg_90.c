#include "sg_90.h"



void SG_90_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);		

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;
	TIM_TimeBaseInitStruct.TIM_Period =199;
	
	//重复计数器的值
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//配置TIME1通道结构体的参数
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable ;
	TIM_OCInitStruct.TIM_Pulse =0;
	 
	//TIME4通道4初始化
    TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

void SG_90_SetDegree(u8 degree){
    u8 compare;
    compare = 195 - (degree / 45 ) * 5;
    TIM_SetCompare4(TIM1,compare);//0度
}


void SG_90_SetCompare(u8 compare){
    TIM_SetCompare4(TIM1,compare);
}

