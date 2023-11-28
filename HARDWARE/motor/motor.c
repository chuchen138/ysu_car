#include "motor.h"
#include "stm32f10x.h"
/** *************************
  * @用途  电机引脚初始化函数 
  * @参数  void
  * @返回值 void
	* @注:需要时直接在主函数main调用
 ** *************************/
void motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//GPIO配置结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);//使能相应接口的时钟，以及RCC_APB2Periph_AFIO
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁用JTAG，恢复PB4引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_4;//对应引脚
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	pwm_init();//pwm输出初始化
}
/** *************************
  * @用途  电机PWM引脚初始化函数 
  * @参数  void
  * @返回值 void
	* @注:在motor_init()函数中调用
 ** *************************/
void pwm_init()
{	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//开启TIM4和GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//初始化GPIO口 PB6 PB7 用于产生PWM信号
	//复用推挽输出模式因为复用了TIM4外设
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO B6 B7 
	//使用内部时钟
	TIM_InternalClockConfig(TIM4);
	//配置时基单元参数
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//定时器不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;//ARR自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72*5 - 1;//PSC预分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//到达ARR触发一次中断 停止计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//初始化单元
	//输出比较结构体配置
	TIM_OCStructInit(&TIM_OCInitStructure);//补全结构体中未配置参数
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较极性选择
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);//初始化 TIM4 OC1
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);//使能CCR1自动重装
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);//初始化 TIM4 OC2	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//使能CCR2自动重装
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//开启预装载
	TIM_Cmd(TIM4, ENABLE);//开启定时器3
	TIM4->CCR1 = 0;//设置输出比较值
	TIM4->CCR2 = 0;
}



/** *************************
  * @用途  小车前进函数
  * @参数  int 通过调整占空比
					 控制速度，speed范围0-1000
					 占空比speed/1000
  * @返回值 void
	* @注:需要时直接调用
 ** *************************/
void set_speed(int leftspeed,int rightspeed){
    TIM_SetCompare1(TIM4, leftspeed);
	TIM_SetCompare2(TIM4, rightspeed);
}



/** *************************
  * @用途  小车前进函数
  * @参数  
  * @返回值 void
	* @注:需要时直接调用
 ** *************************/
void forward(void)
{
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;
}
/** *************************
  * @用途  小车后退函数
  * @参数  
  * @返回值 void
	* @注:需要时直接调用
 ** *************************/
void backward(void)
{
	AIN1=0;
	AIN2=1;
	BIN1=0;
	BIN2=1;
}
/** *************************
  * @用途  小车右转函数
  * @参数  int 通过调整占空比
					 控制速度，speed范围0-1000
					 占空比speed/1000
  * @返回值 void
	* @注:需要时直接调用
 ** *************************/
void right(int speed)
{
	TIM_SetCompare1(TIM4, speed);
	TIM_SetCompare2(TIM4, speed);
	AIN1=0;
	AIN2=1;
	BIN1=1;
	BIN2=0;
}
/** *************************
  * @用途  小车左转函数
  * @参数  int 通过调整占空比
					 控制速度，speed范围0-1000
					 占空比speed/1000
  * @返回值 void
	* @注:需要时直接调用
 ** *************************/
void left(int speed)
{
	TIM_SetCompare1(TIM4, speed);
	TIM_SetCompare2(TIM4, speed);
	AIN1=1;
	AIN2=0;
	BIN1=0;
	BIN2=1;
}
/** *************************
  * @用途  小车停止函数
  * @参数  void
  * @返回值 void
	* @注:需要时直接调用
 ** *************************/
void stop()
{
	TIM_SetCompare1(TIM4, 0);
	TIM_SetCompare2(TIM4, 0);
	AIN1=0;
	AIN2=0;
	BIN1=0;
	BIN2=0;
}
