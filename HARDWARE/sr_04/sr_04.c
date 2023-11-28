#include "sr_04.h"
#include "stm32f10x.h"

void SR04_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOA, &GPIO_InitStructure);	

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
	
	//初始化定时器2	 
	TIM_TimeBaseStructure.TIM_Period = 7199; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =0; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
 
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//允许更新中断 ,
	
    TIM_Cmd(TIM2,ENABLE); 	//使能定时器2
}

u16 TIM2_UPDATA = 0;
u32 temp = 0;
float Get_SR04_Distance(void)
{
    float distance;
    SR04_TRIG=1;       //触发信号是高电平脉冲，宽度大于10us
    delay_us(20);
    SR04_TRIG=0;
    while(!SR04_ECHO); //等待高电平
    TIM_SetCounter(TIM2,0);  //重填计数器值
    TIM2_UPDATA = 0;         //溢出次数清零
    while(SR04_ECHO);  //等待低电平
    TIM_Cmd(TIM2,DISABLE);  //暂时关闭定时器，保证数据正确性
    temp = (int)(((double)(TIM_GetCounter(TIM2) + (7200* TIM2_UPDATA)))/72/2);  //得到高电平总时间，单位us（定时器的计数值加上溢出的值才是高电平的时间），除以2是计算单程的时间
        //（7200* TIM2_UPDATA这里为溢出的时间us，可以转换为100* TIM2_UPDATA,因为在公式后面除以了72。可最终理解为TIM2_UPDATA个100us）
    TIM_Cmd(TIM2,ENABLE);

    distance = ((temp * 331) * 1.0 / 1000) / 10.0;
    return distance ;
}

void TIM2_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM2_UPDATA++;//当回响信号很长时，记录溢出次数，每加一次代表加100us
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位
	}
}
