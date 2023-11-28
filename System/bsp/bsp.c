#include "bsp.h"
extern u8 ysu_flag;
extern u32 ysu_mode;
extern u32 ysu_speed;
void bsp_init(void){

//	    USART1_Config(115200);
    USART2_Config(9600);
    delay_init();
	//LED_Init();//gpio led³õÊ¼»¯
    BEEP_Init();
    KEY_Init();
//    XUNGUANG_Init();
//    SG_90_Init();
//    SR04_Init();
}


void wait_for_setting(void){
//		int count = 3000/80;
		u32 times = 3000 * 1000 / 80;
    for( ;(times > 0) && (ysu_flag == FLAG_WAITSETTING) ; times--){
        delay_us(80);
				BEEP = ~BEEP;
    }
		BEEP = 1;
		if(ysu_flag == FLAG_WAITSETTING){
			ysu_flag = FLAG_CLEAR;
			ysu_mode = 0;
		}
}

void Other_Mode_Setting(void){
	if(ysu_mode & MODE_BLUETOOTH_HELP){
		printf(" \
			MODE_STOP=0,\
			MODE_HELP=1<<0,\
			MODE_BLUETOOTH_HELP=1<<1,\
			MODE_BLUETOOTH_CTRL=1<<2,\
			MODE_TRACK = 1<<3,\
			MODE_OBSTACLE_AVOIDANCE = 1<<4,\
			MODE_INFRARED_REMOTE_COPNTROL=1<<5,\
			MODE_PURSE_LIGHT = 1<<6,\
			MODE_ULTRASONIC_DISTANCE=1<<7,\
			MODE_BATTERY_POWER=1<<8,\
			MODE_DANCE = 1<<9,\
			\
			MODE_RED_LIGHT = 1<<13,\
			MODE_GREEN_LIGHT = 1<<14,\
			MODE_BLUE_LIGHT = 1<<15,\
			\
			MODE_ALL_HIGHEST_SPEED = 1<<20,\
			MODE_MAXIMUM_SPEED = 1<<21,\
			MODE_GENERAL_SPEED = 1<<22,\
			MODE_MINIMUM_SPEED = 1<<23,\
		");
		ysu_mode &= ~(u32)MODE_BLUETOOTH_HELP;
	}else if(ysu_mode & MODE_ULTRASONIC_DISTANCE){
		printf("DISTANCE: %f \r\n",Get_SR04_Distance());
		ysu_mode &= ~(u32)MODE_ULTRASONIC_DISTANCE;
	}else if(ysu_mode & MODE_BATTERY_POWER){
		printf("MODE_BATTERY_POWER\r\n");
		ysu_mode &= ~(u32)MODE_BATTERY_POWER;
	}else if(ysu_mode & MODE_RED_LIGHT){
		printf("MODE_RED_LIGHT\r\n");
		ysu_mode &= ~(u32)MODE_RED_LIGHT;
	}else if(ysu_mode & MODE_GREEN_LIGHT){
		printf("MODE_GREEN_LIGHT\r\n");
		ysu_mode &= ~(u32)MODE_GREEN_LIGHT;
	}else if(ysu_mode & MODE_BLUE_LIGHT){
		printf("MODE_BLUE_LIGHT\r\n");
		ysu_mode &= ~(u32)MODE_BLUE_LIGHT;
	}
}

void Other_BLUETOOTH_Ctrl_Mode_Setting(void){
	if(ysu_mode & MODE_BLUTOOTH_CTL_FRONT){
		printf("MODE_BLUTOOTH_CTL_FRONT\r\n");
		ysu_mode &= ~(u32)MODE_BLUTOOTH_CTL_FRONT;
	}else if(ysu_mode & MODE_BLUTOOTH_CTL_BACK){
		printf("MODE_BLUTOOTH_CTL_BACK\r\n");
		ysu_mode &= ~(u32)MODE_BLUTOOTH_CTL_BACK;
	}else if(ysu_mode & MODE_BLUTOOTH_CTL_LEFT){
		printf("MODE_BLUTOOTH_CTL_LEFT\r\n");
		ysu_mode &= ~(u32)MODE_BLUTOOTH_CTL_LEFT;
	}else if(ysu_mode & MODE_BLUTOOTH_CTL_RIGHT){
		printf("MODE_BLUTOOTH_CTL_RIGHT\r\n");
		ysu_mode &= ~(u32)MODE_BLUTOOTH_CTL_RIGHT;
	}

}

void Processing_received_data(u8 Res){
	if(Res >='0' && Res <= '9'){
		if(Res == '0'){
			ysu_mode = MODE_STOP;
		}else if(Res == '1'){
			ysu_mode |= MODE_BLUETOOTH_HELP;
		}else if(Res == '2'){
			ysu_mode = MODE_BLUETOOTH_CTRL;
			ysu_flag = FLAG_WAITSETTING;
//			printf("ysu_flag = %d\r\n",ysu_flag);
		}else if(Res == '3'){
			ysu_mode = MODE_TRACK;
			ysu_flag = FLAG_WAITSETTING;
		}else if(Res == '4'){
			ysu_mode = MODE_OBSTACLE_AVOIDANCE;
			ysu_flag = FLAG_WAITSETTING;
		}else if(Res == '5'){
			ysu_mode = MODE_INFRARED_REMOTE_COPNTROL;
			ysu_flag = FLAG_WAITSETTING;
		}else if(Res == '6'){
			ysu_mode |= MODE_PURSE_LIGHT;
			ysu_flag = FLAG_WAITSETTING;
		}else if(Res == '7'){
			ysu_mode |= MODE_ULTRASONIC_DISTANCE;
		}else if(Res == '8'){
			ysu_mode |= MODE_BATTERY_POWER;
		}else if(Res == '9'){
			ysu_mode = MODE_DANCE;
			ysu_flag = FLAG_WAITSETTING;
		}
	}else{
		if(Res == 'r'){
			ysu_mode |= MODE_RED_LIGHT;
		}else if(Res == 'g'){
			ysu_mode |= MODE_GREEN_LIGHT;
		}else if(Res == 'b'){
			ysu_mode |= MODE_BLUE_LIGHT;
		}else if(Res == 'z'){
			ysu_flag = FLAG_OK;
		}else if(Res == 'a' && (ysu_mode & MODE_BLUETOOTH_CTRL)){
			ysu_mode |= MODE_BLUTOOTH_CTL_LEFT;
		}else if(Res == 's' && (ysu_mode & MODE_BLUETOOTH_CTRL)){
			ysu_mode |= MODE_BLUTOOTH_CTL_BACK;
		}else if(Res == 'd' && (ysu_mode & MODE_BLUETOOTH_CTRL)){
			ysu_mode |= MODE_BLUTOOTH_CTL_RIGHT;
		}else if(Res == 'w' && (ysu_mode & MODE_BLUETOOTH_CTRL)){
			ysu_mode |= MODE_BLUTOOTH_CTL_FRONT;
		}else{
			printf("ysu_mode = %d, ysu_flag = %d,speed = %d.\r\n", ysu_mode,(u32)ysu_flag,ysu_speed);
		}
	}
}

void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line8)==SET){   
        delay_ms(1);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0){
            while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0);
//            key_flag = !key_flag;
						ysu_flag = FLAG_OK;
						printf("ysu_flag EXTI9_5_IRQHandler\r\n");
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line8);

}
