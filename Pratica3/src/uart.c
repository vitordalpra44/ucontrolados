// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif

#define GPIO_PORTA  (1) //bit 8

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void UART_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCUART_R = 1;
	
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while(SYSCTL_PRUART_R!= 1){};
	UART0_CTL_R = 0;
	UART0_IBRD_R = 43;
	UART0_FBRD_R 	= 25;
	UART0_LCRH_R = 0x78;
	UART0_CC_R = 0;
	UART0_CTL_R = 0x301;
		
}



void Transmissao(char var){
	if ((UART0_FR_R & 0x20) != 0x20){
		UART0_DR_R = var;
	}
}

uint8_t Recepcao(uint32_t *var){
	if ((UART0_FR_R & 0x10) != 0x10){
		*var = UART0_DR_R;
		return 1;
	}
	return 0;
}

