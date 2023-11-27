#include "bsp.h"

extern u8 key_flag;
extern u8 ysu_flag;

int main()
{
	bsp_init();
	
	while(1)
	{   
		if(ysu_flag == FLAG_WAITSETTING){
			
		}
//        BEEP_on(1000);
//        u2printf("DISTANCE: %f \r\n",Get_SR04_Distance());
        printf("DISTANCE: %f \r\n",Get_SR04_Distance());
        //SG_90_SetDegree(0);
        delay_ms(500);
        //SG_90_SetDegree(90);
        delay_ms(500);
        //SG_90_SetDegree(180);
        delay_ms(500); 

	}
}
