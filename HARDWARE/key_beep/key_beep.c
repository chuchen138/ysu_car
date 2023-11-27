#include "key_beep.h"
#include "stm32f10x.h"

u8 key_flag = 1;

void BEEP_Init(void){
    GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能gpio时钟，gpio c组的时钟
		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
}

void BEEP_on(u8 msec){
    u32 times = msec * 1000 / 80;
    for(;times > 0;times--){
        delay_us(80);
	    BEEP = ~BEEP;
    }
}

void KEY_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); 
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line8)==SET){   
        delay_ms(1);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0){
            while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0);
            key_flag = !key_flag;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line8);

}
