#include "bluetooth_wifi.h"
#include "stm32f10x.h"
/** ************************
  * @用途  初始化串口2
  * @参数  bound 波特率 115200
  * @返回值 void
  * ************************/
void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	// 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// 将USART2 Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  // 将USART2 Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = bound;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART2, &USART_InitStructure);
	// 串口中断优先级配置
	/* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* 配置USART2为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	// 使能串口接收中断		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	// 使能串口
	USART_Cmd(USART2, ENABLE);	    
}
/** ************************
  * @用途  串口2打印函数
  * @参数  用法同printf
  * @返回值 void
  * ************************/
void u2printf(char* fmt, ...){
    int i;
	int len;
    char buffer[100]; //足够容纳才可以，可以搞大点
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    len = strlen(buffer);
    for( i=0; i<len; i++)
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
        USART_SendData(USART2, buffer[i]); 
    }
}
u32 ysu_mode = 0;
u8 ysu_flag = 0;
u32 ysu_speed = 0;
extern void Processing_received_data(u8 Res);
/** ************************
	* @用途  串口2中断服务函数
  * @参数  void
  * @返回值 void
  * ************************/
void USART2_IRQHandler(void)                	//串口2中断服务程序
	{
	u8 Res; 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			Res =USART_ReceiveData(USART2);	//读取接收到的数据
			printf("Res = [%c], hex = [%#x].\r\n",Res,Res);
			Processing_received_data(Res);		 
    } 
} 
