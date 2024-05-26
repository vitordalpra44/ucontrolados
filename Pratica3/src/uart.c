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
	UART0_IBRD_R = 86;
	UART0_FBRD_R 	= 52;
	UART0_LCRH_R = 0x78;
	UART0_CC_R = 0;
	UART0_CTL_R = 0x301;
		

	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = GPIO_PORTA;
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTA) ) != (GPIO_PORTA) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTA_AHB_PCTL_R = 0x11; // Setando função alternativa UART nos pinos PA1 e PA0
		
	GPIO_PORTA_AHB_AFSEL_R = 3;
	GPIO_PORTA_AHB_DEN_R = 0x3;
		
		
}	


void Transmissao(char var){
	if ((UART0_FR_R & 0x20) != 0x20){
		UART0_DR_R = var;
	}
	
}

void Recepcao(char* var, uint8_t* flag_recebeu){
	if (!(UART0_FR_R << 0xF & 1)){
		*flag_recebeu = 1;
		*var = UART0_DR_R;
	}
}

