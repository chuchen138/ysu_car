#include "bsp.h"
extern u8 car_flag;
extern u32 car_mode;
extern u32 car_speed;
void bsp_init(void){

//	USART1_Config(115200);
    USART2_Config(9600);
    delay_init();
	RGBLED_Init();
    BEEP_Init();
    KEY_Init();
    motor_init();
    
    XUNGUANG_Init();
    SG_90_Init();
    SR04_Init();
    ADC_init();
	
	IRTRAC_GPIO_Init();
	IRAVOID_GPIO_Init();
    
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
		u2printf(" \
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

		u2printf("MODE_ULTRASONIC_DISTANCE\r\n Distance: [%0.2f] cm\r\n",Get_SR04_Distance());

		car_mode &= ~(u32)MODE_ULTRASONIC_DISTANCE; 
	}else if(car_mode & MODE_BATTERY_POWER){

		u2printf("MODE_BATTERY_POWER \r\n\
        Battery Voltage: [%0.2f] V \r\n\
        Battery Voltage(corrected): [%0.2f] V \r\n",Get_ADC(),Get_ADC_Average(5));

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
        forward();
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_FRONT;
	}else if(car_mode & MODE_BLUTOOTH_CTL_BACK){
        backward();
		printf("MODE_BLUTOOTH_CTL_BACK\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_BACK;
	}else if(car_mode & MODE_BLUTOOTH_CTL_LEFT){
        left(500);
		printf("MODE_BLUTOOTH_CTL_LEFT\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_LEFT;
	}else if(car_mode & MODE_BLUTOOTH_CTL_RIGHT){
        right(500);
		printf("MODE_BLUTOOTH_CTL_RIGHT\r\n");
		car_mode &= ~(u32)MODE_BLUTOOTH_CTL_RIGHT;
	}else if(car_mode & MODE_BLUTOOTH_CTL_PAUSE){
        stop();
        u2printf("stop!\r\n");
        car_mode &= ~(u32)MODE_BLUTOOTH_CTL_PAUSE;
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
		}else if(Res == 'p' && (car_mode & MODE_BLUETOOTH_CTRL)){
			car_mode |= MODE_BLUTOOTH_CTL_PAUSE;
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

/*
void BSP_Obstacle_Avoidance(u32 speed,float upDist,float minDist){
    float dist,dist_0,dist_180;
    SG_90_SetDegree(90);
	//stop();
    dist = Get_SR04_Distance();
	u2printf("up distance : %0.2f.\r\n", dist);
    if (dist >= upDist){
        set_speed(car_speed,car_speed);
		forward();
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
            set_speed(car_speed,car_speed);
            backward();
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
}*/



/** *************************
  * @用途  红外寻迹子程序
  * @参数  int speed，int l_r_speed
                     前进后退速度，左右转速度
  * @返回值 void
 ** *************************/
void BSP_Purse_Light(void){
    int leftValue,rightValue;
    leftValue = xunguang_left;
    rightValue = xunguang_right;
    if ( leftValue == 1 && rightValue == 1 ){
        stop();
    }
    else if (leftValue == 1 && rightValue == 0){
        set_speed(500,300);
        forward();
    }
    else if (leftValue == 0 && rightValue == 1){
        set_speed(300,500);
        forward();
    }
    else {
        set_speed(500,500);
        forward();
    }
}





/** *************************
  * @用途  红外寻迹子程序
  * @参数  int speed，int l_r_speed
                     前进后退速度，左右转速度
  * @返回值 void
 ** *************************/
 void BSP_IR_Trace(int speed, int l_r_speed){
    //左右侧中间都检测到，直行
	 //u2printf("MODE_TRACK\r\n");
    if(trac_right1==1&&trac_left1==1)
    {
        set_speed(speed,speed);
        forward();
    }
    //右侧中间检测到，右转
    else if(trac_right1==1&&trac_left1==0)
    {	set_speed( speed,l_r_speed);
		forward();
        //set_speed( l_r_speed,speed);
        //right(l_r_speed);
    }
    //左侧中间检测到，左转
    else if(trac_right1==0&&trac_left1==1)
    {	set_speed( l_r_speed,speed);
		forward();
        //set_speed(speed,l_r_speed);
    }
    //左侧中间和最左侧同时检测到，即左转直角弯，一直左转直到最左侧检测不到
    else if(trac_left2==1&&trac_left1==1&&trac_right2==0)
    {	stop();
		//set_speed( l_r_speed,speed);
		left(300);
    }
    //右侧中间和最右侧同时检测到，即右转直角弯，一直右转直到最右侧检测不到
    else if(trac_right2==1&&trac_right1==1&&trac_left2==0)
    {	stop();
		right(300);
    }
    //如果同时检测不到则后退
    else if(trac_right2==1&&trac_right1==1&&trac_left1==1&&trac_left2==1)
    {
        stop();
		//backward();
    }
}
 



/*
#### 避障逻辑
设置一个范围，[upDist,minDist]
两个红外线避障模块 LeftSensorValue，RightSensorValue
1. 如果距离大于 upDist 并且两个红外值都为1，那么小车前进
2. 如果距离在范围中间，那么，左值为0则右转，右值为0则左转
3. 如果距离在范围中间，左右值都为0，那么舵机转动，
   > 舵机左边距离大左转，右边距离大右转
   >
   > 如果舵机左右距离都小于距离范围，后退
4. 如果距离在范围中间，左右都为1，继续前进
5. 如果距离小于范围，后退。再舵机控制
*/
void BSP_Obstacle_Avoidance(u32 speed,float upDist,float minDist){
    float dist,dist_0,dist_180;
	int LeftSensorValue, RightSensorValue = 0;
    SG_90_SetDegree(90);
    //stop();
    dist = Get_SR04_Distance();
    u2printf("up distance : %0.2f.\r\n", dist);
    LeftSensorValue  = PAin(4);
    RightSensorValue = PAin(6);
    if (dist >= upDist && LeftSensorValue == 1 && RightSensorValue == 1){
        set_speed(speed,speed);
		forward();
        //delay_ms(200);
    }else if(dist >= upDist){
        set_speed(speed,speed);
		forward();
        //delay_ms(80);
    }
//    else if ( dist >= minDist && dist <= upDist && RightSensorValue == 1 && LeftSensorValue == 1) {
//        forward(speed); 
//        delay_ms(400);
//    }
    else if ( dist >= minDist && dist <= upDist && LeftSensorValue == 1 && RightSensorValue == 0) {
        left(speed); 
u2printf("right one\r\n");
        delay_ms(400);
    }else if ( dist >= minDist && dist <= upDist && RightSensorValue == 1 && LeftSensorValue == 0) {
        right(speed); 
u2printf("left one\r\n");
        delay_ms(400);
    }else if( dist >= minDist && dist <= upDist ){
        stop();
u2printf("two\r\n");
        SG_90_SetDegree(0);
        delay_ms(800);
        dist_0 = Get_SR04_Distance();

        SG_90_SetDegree(180);
        delay_ms(800);
        dist_180 = Get_SR04_Distance();
        if ( (dist_180 <= minDist )&& (dist_0 <= minDist )){
			set_speed(speed,speed);
            backward();
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

    }    else if(dist < minDist || (RightSensorValue == 0 && LeftSensorValue == 0)){
		set_speed(speed,speed);
        backward();
        delay_ms(200);
        stop();
        SG_90_SetDegree(0);
        delay_ms(800);
        dist_0 = Get_SR04_Distance();

        SG_90_SetDegree(180);
        delay_ms(800);
        dist_180 = Get_SR04_Distance();
        if ( (dist_180 <= minDist )&& (dist_0 <= minDist )){
			set_speed(speed,speed);
            backward();
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

