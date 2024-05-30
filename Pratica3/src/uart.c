// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#include <string.h>
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

void Transmissao(uint32_t var){
	while ((UART0_FR_R & 0x20) == 0x20);
	UART0_DR_R = var;
}

void Recepcao(uint32_t *var){
	while((UART0_FR_R & 0x10) == 0x10);
	*var = UART0_DR_R;
	Transmissao(*var);
}

void EnviarString(unsigned char* string){
	int i = 0;
	unsigned char* string_pos = string;
	for(unsigned char* c = string_pos; i < (int) strlen((char*) string); ++string_pos, c=string_pos, i++){
		Transmissao(*c);
	}
}

unsigned char* progress_string(int val, int max) {

		unsigned char return_string[100] = {"\r"};
    // Calcula a proporção atual/max
    float proporcao = (float) val / max;
    int num_hashes = (int)(proporcao * 73) +1;

    // Preenche a barra com hashes e traços
    memset(return_string+1, '#', num_hashes);
    memset(return_string + num_hashes + 1, '-', 73 - num_hashes);
		return return_string;
}
