
#ifndef __USART_H
#define __USART_H


#include	"stdio.h"	
#include	"stm32f10x.h"


#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART3_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART4_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART5_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����


#define USART1_REC_LEN  			200  	//�����������ֽ��� 200
#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define USART4_REC_LEN  			200  	//�����������ֽ��� 200
#define USART5_REC_LEN  			200  	//�����������ֽ��� 200


extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�   	
extern u8  USART2_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART2_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART3_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART3_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� tern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART4_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART4_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART5_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART5_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 


extern u16 USART1_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         		//����״̬���	
extern u16 USART3_RX_STA;         		//����״̬���	
extern u16 USART4_RX_STA;         		//����״̬���	
extern u16 USART5_RX_STA;         		//����״̬���	


void USART1_Config(u32 bound);
void USART2_Config(u32 bound);
void USART3_Config(u32 bound);
void USART4_Config(u32 bound);
void USART5_Config(u32 bound);


#endif	/*__USART_H*/




