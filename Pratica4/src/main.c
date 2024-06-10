// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>



void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void GPIO_Init(void);
uint8_t Keyboard_Read(void);
void GPIO_Init_LEDS(void);
void acenderLeds(uint8_t leds);
int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	GPIO_Init_LEDS();
	while (1){
			if (Keyboard_Read() == '1')
				acenderLeds(1);
				//se entrar funcionou

		
	}
	return 0;
}


