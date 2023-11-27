#include "ir_yaokong.h"
uint32_t frame_data=0;    /* һ֡���ݻ��� */
uint8_t  frame_cnt=0;
uint8_t  frame_flag=0;    /* һ֡���ݽ�����ɱ�־ */
uint8_t isr_cnt;  /* ���ڼ�����˶��ٴ��ж� */ 
/** *************************
  * @��;  ����ң�����ų�ʼ����
					 ��main�����е���
  * @����  void
  * @����ֵ void
 ** *************************/
void ir_yaokong_init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio���ýṹ��
	EXTI_InitTypeDef EXTI_InitStructure;//�ⲿ�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��gpioʱ�ӣ�gpio A���ʱ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//�������룬����½���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//�ⲿ����
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;//�ⲿ���Ŷ�Ӧ�ж���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;//��Ӧ�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=5;
	NVIC_Init(&NVIC_InitStructure);
}
/** *************************
  * @��;  ��ȡ�ߵ�ƽ��ʱ�� 
  * @����  void
  * @����ֵ uint8_t
	* @ע:��ȡ�����ֵʱ����
 ** *************************/
uint8_t Get_Pulse_Time(void)
{
  uint8_t time = 0;
  while(IR_IN)
  {
    time ++;
    delay_us(20);     // ��ʱ 20us
    if(time == 250)
      return time;   // ��ʱ���   
  }
  return time;
}

/*
 * ֡������4���ֽڣ���һ���ֽ���ң�ص�ID���ڶ����ֽ��ǵ�һ���ֽڵķ���
 * ������������ң�ص������ļ�ֵ�����ĸ��ֽ��ǵ������ֽڵķ���
 */
/** *************************
  * @��;  ����ң��Э�����
					 ������֤���� 
  * @����  void
  * @����ֵ uint8_t
	* @ע:��ȡ�����ֵʱ����
 ** *************************/
uint8_t IrDa_Process(void)
{
  uint8_t first_byte, sec_byte, tir_byte, fou_byte;  //�������� ��ַ�룬��ַ���룬��ֵ�룬��ֵ����
  first_byte = frame_data >> 24;
  sec_byte = (frame_data>>16) & 0xff;
  tir_byte = frame_data >> 8;
  fou_byte = frame_data;
  /* �ǵ����־λ */
  frame_flag = 0;
  if( first_byte==(uint8_t)~sec_byte )
  {
    if( tir_byte == (u8)~fou_byte )
      return tir_byte;
  }
  return 0;   /* ���󷵻� */
}
/** *************************
  * @��;  �ⲿ�����ж� 
  * @����  void
  * @����ֵ void
	* @ע:�����������½��ش���
 ** *************************/
void EXTI1_IRQHandler(void)
{
	uint8_t pulse_time = 0;
  uint8_t leader_code_flag = 0; /* �������־λ�������������ʱ����ʾһ֡���ݿ�ʼ */
  uint8_t irda_data = 0;        /* �����ݴ�λ */ 
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) /* ȷ���Ƿ������EXTI Line�ж� */
	{   
    while(1)
    {
      if(IR_IN== SET)        /* ֻ�����ߵ�ƽ��ʱ�� */
      {       
        pulse_time = Get_Pulse_Time();

        /* >=5ms ���������ź� �����ָ��Ż���������ʱ��Ҳ��break����while(1)ѭ�� */
        if( pulse_time >= 250 )                
        {
          break; /* ����while(1)ѭ�� */
        }

        if(pulse_time>=200 && pulse_time<250)         /* ���ǰ��λ 4ms~4.5ms */
        {
          leader_code_flag = 1;
        }
        else if(pulse_time>=10 && pulse_time<50)      /* 0.56ms: 0.2ms~1ms */
        {
          irda_data = 0;
        }
        else if(pulse_time>=50 && pulse_time<100)     /* 1.68ms��1ms~2ms */
        {
          irda_data =1 ; 
        }        
        else if( pulse_time>=100 && pulse_time<=200 ) /* 2.1ms��2ms~4ms */
        {/* �����룬�ڵڶ����жϳ��� */
          frame_flag = 1;               /* һ֡���ݽ������ */
          frame_cnt++;                  /* ����������1 */
          isr_cnt ++;                   /* ���ж�һ�μ�1 */
          break;                        /* ����while(1)ѭ�� */
        }

        if( leader_code_flag == 1 )
        {/* �ڵ�һ���ж������ */
          frame_data <<= 1;
          frame_data += irda_data;
          frame_cnt = 0;
          isr_cnt = 1;
        }
      }      
    }
		EXTI_ClearITPendingBit(EXTI_Line1);//����жϱ�־λ
	}  
}
/** *************************
  * @��;  ��ֵ������ 
  * @����  void
  * @����ֵ void
	* @ע:��ֵ����������Ҫʱ
				ֱ����������main����
 ** *************************/
void ir_decode()
{	uint8_t key_val;
	if(frame_flag == 1)//�ж�һ֡�����Ƿ����
	{
		key_val = IrDa_Process();//���մ������ң�ؼ�ֵ
		switch( key_val )
    {
        case 0: //���մ���
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n Error \r\n");
        break;   
        case 162://ң�ؼ�ֵPOWER
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n POWER \r\n");
        break;        
        case 226://ң�ؼ�ֵMENU
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n MENU \r\n");
        break;        
        case 34://ң�ؼ�ֵTEST
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n TEST \r\n");
        break;        
        case 2://ң�ؼ�ֵ+
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n + \r\n");
        break;    
        case 194://ң�ؼ�ֵRETURN
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n RETURN \r\n");
        break;  
        case 224://ң�ؼ�ֵ|<<
          //printf("\r\n key_val=%d \r\n",key_val);
         //printf("\r\n |<< \r\n");
        break;        
        case 168://ң�ؼ�ֵ|>
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n > \r\n");
        break;        
        case 144://ң�ؼ�ֵ>>|
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n >>| \r\n");
        break;
        case 104://ң�ؼ�ֵ0
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 0 \r\n");
        break;
        case 152://ң�ؼ�ֵ-
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n - \r\n");
        break;
        case 176://ң�ؼ�ֵC
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n C \r\n");
        break;
        case 48://ң�ؼ�ֵ1
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 1 \r\n");
        break;
        case 24://ң�ؼ�ֵ2
					/*���յ���Ӧ��ֵ���Զ������*/
					//forward(1000);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 2 \r\n");
        break;       
        case 122://ң�ؼ�ֵ3
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 3 \r\n");
        break;        
        case 16://ң�ؼ�ֵ4
					/*���յ���Ӧ��ֵ���Զ������*/
					//left(500);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 4 \r\n");
        break;    
        case 56://ң�ؼ�ֵ5
					/*���յ���Ӧ��ֵ���Զ������*/
					//stop();
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 5 \r\n");
        break;      
        case 90://ң�ؼ�ֵ6
					/*���յ���Ӧ��ֵ���Զ������*/
					//right(500);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 6 \r\n");
        break;        
        case 66://ң�ؼ�ֵ7
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 7 \r\n");
        break;
        case 74://ң�ؼ�ֵ8
					/*���յ���Ӧ��ֵ���Զ������*/
					//backward(1000);
          //printf("\r\n key_val=%d \r\n",key_val);
          //printf("\r\n 8 \r\n");
        break;
        case 82://ң�ؼ�ֵ9
					/*���յ���Ӧ��ֵ���Զ������*/
          //printf("\r\n key_val=%d \r\n",key_val);
         // printf("\r\n 9 \r\n");
        break;
        default:     
        break;
		}
	}
}

