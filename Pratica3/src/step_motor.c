#include "step_motor.h"
#include "tm4c1294ncpdt.h"


#define GPIO_PORT_H = (0x1 << 7)
#define half_step 4096
#define full_step 2048
#define clockwise [1,2,3,4,5,6,7,8]
#define counter [8,7,6,5,4,3,2,1]
// 2048 / 4 = 512

void SysTick_Wait1ms(uint32_t delay);
void PortH_Output(uint32_t valor);


extern uint8_t leds;

void step_motor(void){
	PortH_Output(0);
}

void rotate_step_motor(uint8_t direction, uint8_t speed){
	//Direction: 0 - Clockwise, 1 - Counterclockwise
	//Speed: 0 - Halfstep, 1 - Fullstep
	//Unipolar
	//Quantidade de passos para volta: 2048
	int angle;
	for(int i = 0; i < 512; i++){

		angle = i/64;
		leds = 0;
		if (direction == '0')
			for (int j =0; j<=angle; j++)
				leds = (leds << 1) | 1;
		else
			for (int j =0; j<=angle; j++)
				leds = (leds >> 1) | 0x80;
			
		//Break point: 0-63, 64-127, 128-191, 192-255, 256-319, 320-383, 384-447, 448-511
		if(direction == '0'){
			//Halfstep clockwise
			//piscaLeds(clockwise[angle]);
			
			if(speed == '0'){
				PortH_Output(0x7); // 0111
				SysTick_Wait1ms(10);
				
				PortH_Output(0x3); // 0011
				SysTick_Wait1ms(10);

				PortH_Output(0xB); // 1011
				SysTick_Wait1ms(10);
				
				PortH_Output(0x9); // 1001
				SysTick_Wait1ms(10);

				PortH_Output(0xD); // 1101
				SysTick_Wait1ms(10);
				
				PortH_Output(0xC); // 1100
				SysTick_Wait1ms(10);

				PortH_Output(0xE); // 1110
				SysTick_Wait1ms(10);
				
				PortH_Output(0x6); // 0110
				SysTick_Wait1ms(10);
			}
			//Fullstep clockwise
			else if(speed == '1'){
				PortH_Output(0x7); // 0111
				SysTick_Wait1ms(10);
				PortH_Output(0xB); // 1011
				SysTick_Wait1ms(10);
				PortH_Output(0xD); // 1101
				SysTick_Wait1ms(10);
				PortH_Output(0xE); // 1110
				SysTick_Wait1ms(10);
			}
		}
		else if(direction == '1'){
			//Halfstep counterclockwise
			//piscaLeds(counter[angle]);
			
			if(speed == '0'){
				PortH_Output(0x6); // 0110
				SysTick_Wait1ms(10);
				
				PortH_Output(0xE); // 1110
				SysTick_Wait1ms(10);
				
				PortH_Output(0xC); // 1100
				SysTick_Wait1ms(10);
				
				PortH_Output(0xD); // 1101
				SysTick_Wait1ms(10);
				
				PortH_Output(0x9); // 1001
				SysTick_Wait1ms(10);
				
				PortH_Output(0xB); // 1011
				SysTick_Wait1ms(10);
				
				PortH_Output(0x3); // 0011
				SysTick_Wait1ms(10);
				
				PortH_Output(0x7); // 0111
				SysTick_Wait1ms(10);
			}
			//Fullstep counterclockwise
			else if(speed == '1'){
				PortH_Output(0xE); // 1110
				SysTick_Wait1ms(10);
				PortH_Output(0xD); // 1101
				SysTick_Wait1ms(10);
				PortH_Output(0xB); // 1011
				SysTick_Wait1ms(10);
				PortH_Output(0x7); // 0111
				SysTick_Wait1ms(10);
			}
		}
	}
}