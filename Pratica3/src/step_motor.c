#include <stdint.h>
#include "tm4c1294ncpdt.h"


#define GPIO_PORT_H = (0x1 << 7)
#define half_step 128
#define full_step 64

void SysTick_Wait1ms(uint32_t delay);
void PortH_Output(uint32_t valor);
void rotate_step_motor(uint8_t direction, uint8_t speed);

void rotate_step_motor(uint8_t direction, uint8_t speed){
	//Direction: 0 - Clockwise, 1 - Counterclockwise
	//Speed: 0 - Halfstep, 1 - Fullstep
	//Unipolar
	//Quantidade de passos para volta: 64
	if(direction == '0'){
		//Halfstep clockwise
		if(speed == '0'){
			PortH_Output(0x8); // 1000
      SysTick_Wait1ms(10);
			
      PortH_Output(0xC); // 1100
      SysTick_Wait1ms(10);

			PortH_Output(0x4); // 0100
			SysTick_Wait1ms(10);
			
			PortH_Output(0x6); // 0110
			SysTick_Wait1ms(10);

			PortH_Output(0x2); // 0010
			SysTick_Wait1ms(10);
			
			PortH_Output(0x3); // 0011
			SysTick_Wait1ms(10);

			PortH_Output(0x1); // 0001
			SysTick_Wait1ms(10);
			
			PortH_Output(0x9); // 1001
			SysTick_Wait1ms(10);
		}
		//Fullstep clockwise
		else if(speed == '1'){
			PortH_Output(0x8); // 1000
      SysTick_Wait1ms(10);
      PortH_Output(0x4); // 0100
      SysTick_Wait1ms(10);
      PortH_Output(0x2); // 0010
      SysTick_Wait1ms(10);
      PortH_Output(0x1); // 0001
      SysTick_Wait1ms(10);
		}
	}
	else if(direction == '1'){
		//Halfstep counterclockwise
		if(speed == '0'){
			PortH_Output(0x9); // 1001
			SysTick_Wait1ms(10);
			
			PortH_Output(0x1); // 0001
			SysTick_Wait1ms(10);
			
			PortH_Output(0x3); // 0011
			SysTick_Wait1ms(10);
			
			PortH_Output(0x2); // 0010
			SysTick_Wait1ms(10);
			
			PortH_Output(0x6); // 0110
			SysTick_Wait1ms(10);
			
			PortH_Output(0x4); // 0100
			SysTick_Wait1ms(10);
			
			PortH_Output(0xC); // 1100
      SysTick_Wait1ms(10);
			
			PortH_Output(0x8); // 1000
      SysTick_Wait1ms(10);
		}
		//Fullstep counterclockwise
		else if(speed == '1'){
			PortH_Output(0x1); // 0001
      SysTick_Wait1ms(10);
      PortH_Output(0x2); // 0010
      SysTick_Wait1ms(10);
      PortH_Output(0x4); // 0100
      SysTick_Wait1ms(10);
      PortH_Output(0x8); // 1000
      SysTick_Wait1ms(10);
		}
	}
}
