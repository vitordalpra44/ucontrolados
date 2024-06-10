// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTL (1<<10)
#define GPIO_PORTM (1<<11)
#define GPIO_PORTK (1<<9)


void SysTick_Wait1ms(uint32_t delay);
void GPIO_Init();

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void GPIO_Init()
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTL | GPIO_PORTM | GPIO_PORTK);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTL | GPIO_PORTM | GPIO_PORTK) ) != (GPIO_PORTL | GPIO_PORTM | GPIO_PORTK));
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTL_AMSEL_R = 0x00;
	GPIO_PORTM_AMSEL_R = 0x00;
	GPIO_PORTK_AMSEL_R = 0x00;

	
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTL_PCTL_R = 0x00;
	GPIO_PORTM_PCTL_R = 0x00;
	GPIO_PORTK_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTL_DIR_R = 0; //BIT0 | BIT1
	GPIO_PORTM_DIR_R = 0x7;
	GPIO_PORTK_DIR_R = 0xFF;

	
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTL_AFSEL_R = 0x00;
	GPIO_PORTM_AFSEL_R = 0x00;
	GPIO_PORTK_AFSEL_R = 0x00;

	
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTL_DEN_R = 0xFF; 		   //Bit0 e bit1
	GPIO_PORTM_DEN_R = 0xFF; 		   //Bit0 e bit1
	GPIO_PORTK_DEN_R = 0xFF; 		   //Bit0 e bit1

	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTL_PUR_R = 0xFF; 


}	





