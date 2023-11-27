
#ifndef __USART_H
#define __USART_H


#include	"stdio.h"	
#include	"stm32f10x.h"


#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART3_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART4_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART5_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����


#define USART1_REC_LEN  			200  	//�����������ֽ��� 200
#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define USART4_REC_LEN  			200  	//�����������ֽ��� 200
#define USART5_REC_LEN  			200  	//�����������ֽ��� 200

typedef enum{FLAG_CLEAR,FLAG_WAITSETTING,FLAG_OK}YSU_FLAG_STATUS;
typedef enum{
	MODE_STOP=0,
	MODE_HELP=1<<0,
	MODE_BLUETOOTH_HELP=1<<1,
	MODE_BLUETOOTH_CTRL=1<<2,
	MODE_TRACK = 1<<3,
	MODE_OBSTACLE_AVOIDANCE = 1<<4,
	MODE_INFRARED_REMOTE_COPNTROL=1<<5,
	MODE_PURSE_LIGHT = 1<<6,
	MODE_ULTRASONIC_DISTANCE=1<<7,
	MODE_BATTERY_POWER=1<<8,
	MODE_DANCE = 1<<9,
	
	
	
	MODE_RED_LIGHT = 1<<13,
	MODE_GREEN_LIGHT = 1<<14,
	MODE_BLUE_LIGHT = 1<<15,
	
	MODE_ALL_HIGHEST_SPEED = 1<<20,
	MODE_MAXIMUM_SPEED = 1<<21,
	MODE_GENERAL_SPEED = 1<<22,
	MODE_MINIMUM_SPEED = 1<<23,
}YSU_MODE_STATUS;

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




