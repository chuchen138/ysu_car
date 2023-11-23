#include "motor.h"
#include "stm32f10x.h"
void motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio���ýṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ����Ӧ�ӿڵ�ʱ�ӣ��Լ�RCC_APB2Periph_AFIO
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//����JTAG���ָ�PB4����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void pwm_init()
{	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
//����TIM4��GPIOB��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//��ʼ��GPIO�� PB6 PB7 ���ڲ���PWM�ź�
	
	//�����������ģʽ��Ϊ������TIM4����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO A6 A7 
	//ʹ���ڲ�ʱ��
	TIM_InternalClockConfig(TIM4);
	//����ʱ����Ԫ����
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//��ʱ������Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;//ARR�Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSCԤ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//����ARR����һ���ж� ֹͣ����
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//��ʼ����Ԫ
	//����ȽϽṹ������
	TIM_OCStructInit(&TIM_OCInitStructure);//��ȫ�ṹ����δ���ò���
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����Ƚϼ���ѡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);//��ʼ�� TIM4 OC1
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);//ʹ��CCR1�Զ���װ
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);//��ʼ�� TIM4 OC2	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//ʹ��CCR2�Զ���װ
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//����Ԥװ��
	TIM_Cmd(TIM4, ENABLE);//������ʱ��3
	TIM4->CCR1 = 0;//��������Ƚ�ֵ
	TIM4->CCR2 = 0;
}
void forward(int speed)
{
	TIM_SetCompare1(TIM4, speed);
	TIM_SetCompare2(TIM4, speed);
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;
}
void backward(int speed)
{
	TIM_SetCompare1(TIM4, speed);
	TIM_SetCompare2(TIM4, speed);
	AIN1=0;
	AIN2=1;
	BIN1=0;
	BIN2=1;
}
void right(int speed)
{
	TIM_SetCompare1(TIM4, speed);
	TIM_SetCompare2(TIM4, speed);
	AIN1=0;
	AIN2=1;
	BIN1=1;
	BIN2=0;
}
void left(int speed)
{
	TIM_SetCompare1(TIM4, speed);
	TIM_SetCompare2(TIM4, speed);
	AIN1=1;
	AIN2=0;
	BIN1=0;
	BIN2=1;
}
void stop()
{
	TIM_SetCompare1(TIM4, 0);
	TIM_SetCompare2(TIM4, 0);
	AIN1=0;
	AIN2=0;
	BIN1=0;
	BIN2=0;
}