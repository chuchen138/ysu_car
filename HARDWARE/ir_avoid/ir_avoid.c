#include "ir_avoid.h"
#include "stm32f10x.h"


/** *************************
  * @��;  ����������ų�ʼ����
					 ��main�����е���
  * @����  void
  * @����ֵ void
	* @ע:����������ż���.h�ļ�
				�궨��
 ** *************************/
void IRAVOID_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;//gpio���ýṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��gpioʱ�ӣ�gpio c���ʱ��

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

