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

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	 
	while (1){
			if (Keyboard_Read() == '1')
				//se entrar funcionou
				continue;
		
	}
	return 0;
}


