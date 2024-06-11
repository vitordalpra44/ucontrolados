// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>

#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void GPIO_Init(void);
void GPIO_Init_LEDS(void);



void imprimirString(const char* string);
void limparLCD();
uint8_t Keyboard_Read();
void LCD_Init();

void acenderLeds(uint8_t leds);
int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	GPIO_Init_LEDS();
	LCD_Init();
	
	imprimirString("TESTANDO LCD");
	
	while (1){
			uint8_t key = Keyboard_Read();
			if (key == '1')
				acenderLeds(1);
			else
				acenderLeds(0);
		}
	
	return 0;
}


