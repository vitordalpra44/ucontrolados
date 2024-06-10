// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include "tm4c1294ncpdt.h"


// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  

#define GPIO_PORTQ (0x4000)
#define GPIO_PORTP (1<<13)
#define GPIO_PORTB (1<<1)

void SysTick_Wait1ms(uint32_t delay);

// -------------------------------------------------------------------------------
// Fun??o GPIO_Init
// Inicializa os ports J e N
// Par?metro de entrada: N?o tem
// Par?metro de sa?da: N?o tem
void GPIO_Init_LEDS(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTQ |  GPIO_PORTB | GPIO_PORTP);
	//1b.   ap?s isso verificar no PRGPIO se a porta est? pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTQ |  GPIO_PORTB | GPIO_PORTP) ) != (GPIO_PORTQ |  GPIO_PORTB | GPIO_PORTP));
	
	// 2. Limpar o AMSEL para desabilitar a anal?gica
	GPIO_PORTQ_AMSEL_R = 0x00;
	GPIO_PORTP_AMSEL_R = 0x00;
	GPIO_PORTB_AHB_AMSEL_R = 0x00;
	
		
	// 3. Limpar PCTL para selecionar o GPIO
	
	GPIO_PORTQ_PCTL_R = 0;
	GPIO_PORTP_PCTL_R = 0;
	GPIO_PORTB_AHB_PCTL_R = 0x0;

	// 4. DIR para 0 se for entrada, 1 se for sa?da
	
	GPIO_PORTQ_DIR_R = 0xF;
	GPIO_PORTP_DIR_R = 0xFF;
	GPIO_PORTB_AHB_DIR_R = 0xFF;
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun??o alternativa	
	
	GPIO_PORTQ_AFSEL_R = 0x00;
	GPIO_PORTP_AFSEL_R = 0x00;
	GPIO_PORTB_AHB_AFSEL_R = 0x0;
	
	// 6. Setar os bits de DEN para habilitar I/O digital	
	
	GPIO_PORTQ_DEN_R = 0xF;
	GPIO_PORTP_DEN_R = 0xFF;
	GPIO_PORTB_AHB_DEN_R = 0xFF;

	GPIO_PORTP_DATA_R = 1 << 5;
	GPIO_PORTB_AHB_DATA_R = 0;

}	


void acenderLeds(uint8_t leds){
    GPIO_PORTQ_DATA_R = leds & 0x0F;
}

