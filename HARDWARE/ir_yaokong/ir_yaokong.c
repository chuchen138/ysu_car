#include "ir_yaokong.h"
uint32_t frame_data=0;    /* 一帧数据缓存 */
uint8_t  frame_cnt=0;
uint8_t  frame_flag=0;    /* 一帧数据接收完成标志 */
uint8_t isr_cnt;  /* 用于计算进了多少次中断 */ 
/** *************************
  * @用途  红外遥控引脚初始化，
					 在main函数中调用
  * @参数  void
  * @返回值 void
 ** *************************/
void ir_yaokong_init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio配置结构体
	EXTI_InitTypeDef EXTI_InitStructure;//外部中断配置
	NVIC_InitTypeDef NVIC_InitStructure;//中断结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能gpio时钟，gpio A组的时钟
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入，检测下降沿
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//外部引脚
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;//外部引脚对应中断线
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;//对应中断
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=5;
	NVIC_Init(&NVIC_InitStructure);
}
/** *************************
  * @用途  获取高电平的时间 
  * @参数  void
  * @返回值 uint8_t
	* @注:获取红外键值时调用
 ** *************************/
uint8_t Get_Pulse_Time(void)
{
  uint8_t time = 0;
  while(IR_IN)
  {
    time ++;
    delay_us(20);     // 延时 20us
    if(time == 250)
      return time;   // 超时溢出   
  }
  return time;
}

/*
 * 帧数据有4个字节，第一个字节是遥控的ID，第二个字节是第一个字节的反码
 * 第三个数据是遥控的真正的键值，第四个字节是第三个字节的反码
 */
/** *************************
  * @用途  红外遥控协议解析
					 数据验证函数 
  * @参数  void
  * @返回值 uint8_t
	* @注:获取红外键值时调用
 ** *************************/
uint8_t IrDa_Process(void)
{
  uint8_t first_byte, sec_byte, tir_byte, fou_byte;  //四组数据 地址码，地址反码，键值码，键值反码
  first_byte = frame_data >> 24;
  sec_byte = (frame_data>>16) & 0xff;
  tir_byte = frame_data >> 8;
  fou_byte = frame_data;
  /* 记得清标志位 */
  frame_flag = 0;
  if( first_byte==(uint8_t)~sec_byte )
  {
    if( tir_byte == (u8)~fou_byte )
      return tir_byte;
  }
  return 0;   /* 错误返回 */
}
/** *************************
  * @用途  外部触发中断 
  * @参数  void
  * @返回值 void
	* @注:检测红外数据下降沿触发
 ** *************************/
void EXTI1_IRQHandler(void)
{
	uint8_t pulse_time = 0;
  uint8_t leader_code_flag = 0; /* 引导码标志位，当引导码出现时，表示一帧数据开始 */
  uint8_t irda_data = 0;        /* 数据暂存位 */ 
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) /* 确保是否产生了EXTI Line中断 */
	{   
    while(1)
    {
      if(IR_IN== SET)        /* 只测量高电平的时间 */
      {       
        pulse_time = Get_Pulse_Time();
        
        /* >=5ms 不是有用信号 当出现干扰或者连发码时，也会break跳出while(1)循环 */
        if( pulse_time >= 250 )                
        {
          break; /* 跳出while(1)循环 */
        }
        
        if(pulse_time>=200 && pulse_time<250)         /* 获得前导位 4ms~4.5ms */
        {
          leader_code_flag = 1;
        }
        else if(pulse_time>=10 && pulse_time<50)      /* 0.56ms: 0.2ms~1ms */
        {
          irda_data = 0;
        }
        else if(pulse_time>=50 && pulse_time<100)     /* 1.68ms：1ms~2ms */
        {
          irda_data =1 ; 
        }        
        else if( pulse_time>=100 && pulse_time<=200 ) /* 2.1ms：2ms~4ms */
        {/* 连发码，在第二次中断出现 */
          frame_flag = 1;               /* 一帧数据接收完成 */
          frame_cnt++;                  /* 按键次数加1 */
          isr_cnt ++;                   /* 进中断一次加1 */
          break;                        /* 跳出while(1)循环 */
        }
        
        if( leader_code_flag == 1 )
        {/* 在第一次中断中完成 */
          frame_data <<= 1;
          frame_data += irda_data;
          frame_cnt = 0;
          isr_cnt = 1;
        }
      }      
    }
		EXTI_ClearITPendingBit(EXTI_Line1);//清除中断标志位
	}  
}
/** *************************
  * @用途  键值处理函数 
  * @参数  void
  * @返回值 void
	* @注:键值处理函数，需要时
				直接在主函数main调用
 ** *************************/
void ir_decode()
{	uint8_t key_val;
	if(frame_flag == 1)//判断一帧接收是否完成
	{
		key_val = IrDa_Process();//接收处理红外遥控键值
		switch( key_val )
    {
        case 0: //接收错误
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n Error \r\n");
        break;   
        case 162://遥控键值POWER
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n POWER \r\n");
        break;        
        case 226://遥控键值MENU
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n MENU \r\n");
        break;        
        case 34://遥控键值TEST
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n TEST \r\n");
        break;        
        case 2://遥控键值+
					/*接收到对应键值后自定义程序*/
			set_redLED(0);
			set_greenLED(100);
			set_blueLED(0);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n + \r\n");
        break;    
        case 194://遥控键值RETURN
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n RETURN \r\n");
        break;  
        case 224://遥控键值|<<
			set_redLED(0);
			set_greenLED(0);
			set_blueLED(100);
          //printf("\r\n key_val=%d \r\n",key_val);
         //printf("\r\n |<< \r\n");
        break;        
        case 168://遥控键值|>
					/*接收到对应键值后自定义程序*/
			set_redLED(100);
			set_greenLED(0);
			set_blueLED(0);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n > \r\n");
        break;        
        case 144://遥控键值>>|
					/*接收到对应键值后自定义程序*/
			set_redLED(100);
			set_greenLED(100);
			set_blueLED(0);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n >>| \r\n");
        break;
        case 104://遥控键值0
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 0 \r\n");
        break;
        case 152://遥控键值-
			set_redLED(100);
			set_greenLED(0);
			set_blueLED(100);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n - \r\n");
        break;
        case 176://遥控键值C
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n C \r\n");
        break;
        case 48://遥控键值1
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 1 \r\n");
        break;
        case 24://遥控键值2
					/*接收到对应键值后自定义程序*/
					//forward(1000);
			set_speed(500,500);
			forward();
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 2 \r\n");
        break;       
        case 122://遥控键值3
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 3 \r\n");
        break;        
        case 16://遥控键值4
					/*接收到对应键值后自定义程序*/
					left(300);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 4 \r\n");
        break;    
        case 56://遥控键值5
					/*接收到对应键值后自定义程序*/
					stop();
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 5 \r\n");
        break;      
        case 90://遥控键值6
					/*接收到对应键值后自定义程序*/
				right(300);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 6 \r\n");
        break;        
        case 66://遥控键值7
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 7 \r\n");
        break;
        case 74://遥控键值8
					/*接收到对应键值后自定义程序*/
					//backward(1000);
			set_speed(500,500);
			backward();
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 8 \r\n");
        break;
        case 82://遥控键值9
					/*接收到对应键值后自定义程序*/
          //printf("\r\n key_val=%d \r\n",key_val);
         // printf("\r\n 9 \r\n");
        break;
        default:     
        break;
		}
	}
}
