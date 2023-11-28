#include "bsp.h"
extern u8 key_flag;
extern u8 car_flag;  // 是否确定
//     car_flag
//     car_mode
extern u32 car_mode; // 不同的模式
extern u32 car_speed;
u8 times = 0;
int main(){   
	car_speed = 300;
    bsp_init();
//	while(1){
//			left(500);
//		delay_ms(650);
//		right(500);
//		delay_ms(100);
//		
//		stop();
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//	}
    while(1)
    {   
        if(car_flag == FLAG_WAITSETTING)
        {
                wait_for_setting();
        }
        while(car_mode == MODE_STOP)
        {
            if(times == 15 + 1){
                u2printf("MODE_STOP\r\n");
                times = 0;
            }
            stop();
            Other_Mode_Setting();
            delay_ms(1000);
            times++;
        }
        while(car_flag == FLAG_OK && (car_mode & MODE_BLUETOOTH_CTRL))
        {
            // 蓝牙控制
            //u2printf("MODE_BLUETOOTH_CTRL\r\n");
            set_speed(400,300);
            //
            //
            if((car_mode| MODE_BLUETOOTH_CTRL) != MODE_BLUETOOTH_CTRL)
            {
                Other_Mode_Setting();
                Other_BLUETOOTH_Ctrl_Mode_Setting();
            }
            //delay_ms(1000);
        }
        while(car_flag == FLAG_OK && (car_mode & MODE_TRACK))
        { // 循迹模式
             
            BSP_IR_Trace(350,0);
            if((car_mode| MODE_TRACK) != MODE_TRACK)
            {
                Other_Mode_Setting();
            }
            //delay_ms(1000);
        }
        while(car_flag == FLAG_OK && (car_mode & MODE_OBSTACLE_AVOIDANCE))
        {    // 红外加超声波避障
            //printf("MODE_OBSTACLE_AVOIDANCE\r\n");
			BSP_Obstacle_Avoidance(car_speed,25,10);
			//forward(car_speed);
            if((car_mode| MODE_OBSTACLE_AVOIDANCE) != MODE_OBSTACLE_AVOIDANCE)
            {
                Other_Mode_Setting();
            }
            //delay_ms(1000);
			//delay_ms(1000);
        }
        
        while(car_flag == FLAG_OK && (car_mode & MODE_INFRARED_REMOTE_COPNTROL))
        { // 红外遥控
            //u2printf("MODE_INFRARED_REMOTE_COPNTROL\r\n");
			ir_decode();
            if((car_mode| MODE_INFRARED_REMOTE_COPNTROL) != MODE_INFRARED_REMOTE_COPNTROL)
            {
                Other_Mode_Setting();
            }
            //delay_ms(1000);
        }
        while(car_flag == FLAG_OK && (car_mode & MODE_PURSE_LIGHT))
        { // 追光模式
            //printf("MODE_PURSE_LIGHT\r\n");
            BSP_Purse_Light();
            if((car_mode| MODE_PURSE_LIGHT) != MODE_PURSE_LIGHT)
            {
                Other_Mode_Setting();
            }
            //delay_ms(1000);
        }
        while(car_flag == FLAG_OK && (car_mode & MODE_DANCE))
        {// 舞蹈模式
            //printf("MODE_DANCE\r\n");
			BSP_Dance(300, 0);
            if((car_mode| MODE_DANCE) != MODE_DANCE)
            {
                Other_Mode_Setting();
            }
            //delay_ms(1000);
        }
        if(car_mode!= MODE_STOP)
        {
            Other_Mode_Setting();
        }

    }
}
