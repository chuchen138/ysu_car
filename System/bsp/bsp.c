#include "bsp.h"
extern u8 car_flag;
extern u32 car_mode;
extern u32 car_speed;
void bsp_init(void){

//	USART1_Config(115200);
    USART2_Config(9600);
    delay_init();
	//LED_Init();//gpio led��ʼ��
    BEEP_Init();
    KEY_Init();
    motor_init();
//  XUNGUANG_Init();
    SG_90_Init();
    SR04_Init();
}


void wait_for_setting(void){
//	int count = 3000/80;
	u32 times = 3000 * 1000 / 80;
    for( ;(times > 0) && (car_flag == FLAG_WAITSETTING) ; times--){
        delay_us(80);
		BEEP = ~BEEP;
    }
	BEEP = 1;
	if(car_flag == FLAG_WAITSETTING){
		car_flag = FLAG_CLEAR;
		car_mode = 0;
	}
}

void Other_Mode_Setting(void){
	if(car_mode & MODE_BLUETOOTH_HELP){
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
		car_mode &= ~(u32)MODE_BLUETOOTH_HELP;
	}else if(car_mode & MODE_ULTRASONIC_DISTANCE){
		
		car_mode &= ~(u32)MODE_ULTRASONIC_DISTANCE;
	}else if(car_mode & MODE_BATTERY_POWER){
		printf("MODE_BATTERY_POWER\r\n");
		car_mode &= ~(u32)MODE_BATTERY_POWER;
	}else if(car_mode & MODE_RED_LIGHT){
		printf("MODE_RED_LIGHT\r\n");
		car_mode &= ~(u32)MODE_RED_LIGHT;
	}else if(car_mode & MODE_GREEN_LIGHT){
		printf("MODE_GREEN_LIGHT\r\n");
		car_mode &= ~(u32)MODE_GREEN_LIGHT;
	}else if(car_mode & MODE_BLUE_LIGHT){
		printf("MODE_BLUE_LIGHT\r\n");
		car_mode &= ~(u32)MODE_BLUE_LIGHT;
	}
}

void Other_BLUETOOTH_Ctrl_Mode_Setting(void){
	if(car_mode & MODE_BLUTOOTH_CTL_FRONT){
		printf("MODE_BLUTOOTH_CTL_FRONT\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_FRONT;
	}else if(car_mode & MODE_BLUTOOTH_CTL_BACK){
		printf("MODE_BLUTOOTH_CTL_BACK\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_BACK;
	}else if(car_mode & MODE_BLUTOOTH_CTL_LEFT){
		printf("MODE_BLUTOOTH_CTL_LEFT\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_LEFT;
	}else if(car_mode & MODE_BLUTOOTH_CTL_RIGHT){
		printf("MODE_BLUTOOTH_CTL_RIGHT\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_RIGHT;
	}

}

void Processing_received_data(u8 Res){
	if(Res >='0' && Res <= '9'){
		if(Res == '0'){
			car_mode = MODE_STOP;
		}else if(Res == '1'){
			car_mode |= MODE_BLUETOOTH_HELP;
		}else if(Res == '2'){
			car_mode = MODE_BLUETOOTH_CTRL;
			car_flag = FLAG_WAITSETTING;
//			printf("car_flag = %d\r\n",car_flag);
		}else if(Res == '3'){
			car_mode = MODE_TRACK;
			car_flag = FLAG_WAITSETTING;
		}else if(Res == '4'){
			car_mode = MODE_OBSTACLE_AVOIDANCE;
			car_flag = FLAG_WAITSETTING;
		}else if(Res == '5'){
			car_mode = MODE_INFRARED_REMOTE_COPNTROL;
			car_flag = FLAG_WAITSETTING;
		}else if(Res == '6'){
			car_mode |= MODE_PURSE_LIGHT;
			car_flag = FLAG_WAITSETTING;
		}else if(Res == '7'){
			car_mode |= MODE_ULTRASONIC_DISTANCE;
		}else if(Res == '8'){
			car_mode |= MODE_BATTERY_POWER;
		}else if(Res == '9'){
			car_mode = MODE_DANCE;
			car_flag = FLAG_WAITSETTING;
		}
	}else{
		if(Res == 'r'){
			car_mode |= MODE_RED_LIGHT;
		}else if(Res == 'g'){
			car_mode |= MODE_GREEN_LIGHT;
		}else if(Res == 'b'){
			car_mode |= MODE_BLUE_LIGHT;
		}else if(Res == 'z'){
			car_flag = FLAG_OK;
		}else if(Res == 'a' && (car_mode & MODE_BLUETOOTH_CTRL)){
			car_mode |= MODE_BLUTOOTH_CTL_LEFT;
		}else if(Res == 's' && (car_mode & MODE_BLUETOOTH_CTRL)){
			car_mode |= MODE_BLUTOOTH_CTL_BACK;
		}else if(Res == 'd' && (car_mode & MODE_BLUETOOTH_CTRL)){
			car_mode |= MODE_BLUTOOTH_CTL_RIGHT;
		}else if(Res == 'w' && (car_mode & MODE_BLUETOOTH_CTRL)){
			car_mode |= MODE_BLUTOOTH_CTL_FRONT;
		}else{
			printf("car_mode = %d, car_flag = %d,speed = %d.\r\n", car_mode,(u32)car_flag,car_speed);
		}
	}
}

void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line8)==SET){   
        delay_ms(1);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0){
            while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0);
//            key_flag = !key_flag;
						car_flag = FLAG_OK;
						printf("car_flag EXTI9_5_IRQHandler\r\n");
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line8);

}


void BSP_Obstacle_Avoidance(u32 speed,float upDist,float minDist){
    float dist,dist_0,dist_180;
    SG_90_SetDegree(90);
	//stop();
    dist = Get_SR04_Distance();
	u2printf("up distance : %0.2f.\r\n", dist);
    if (dist >= upDist){
		forward(speed);
		return;
	}
	else if (dist <= upDist){
        stop();
        SG_90_SetDegree(0);
        delay_ms(800);
        dist_0 = Get_SR04_Distance();

        SG_90_SetDegree(180);
        delay_ms(800);
        dist_180 = Get_SR04_Distance();
        if ( (dist_180 <= minDist )&& (dist_0 <= minDist )){
            backward(speed);
			delay_ms(800);
            return;
        }
        else if (dist_180 < dist_0 ){
            right(speed);
			delay_ms(800);
            return;
        }
        else if(dist_180 >= dist_0){
            left(speed);
			delay_ms(800);
            return;
        }

    }
}
