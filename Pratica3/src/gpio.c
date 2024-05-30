// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTH  (0x1 << 7)// bit 7	
#define GPIO_PORTJ  (0x0100) //bit 8
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTA (0x1)
#define GPIO_PORTQ (0x4000)
#define GPIO_PORTP (1<<13)
#define GPIO_PORTB (1<<1)





extern uint8_t leds;
extern uint8_t pisca;
void SysTick_Wait1ms(uint32_t delay);

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ | GPIO_PORTH | GPIO_PORTB | GPIO_PORTP);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ | GPIO_PORTH | GPIO_PORTB | GPIO_PORTP) ) != (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ | GPIO_PORTH | GPIO_PORTB | GPIO_PORTP));
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTQ_AMSEL_R = 0x00;
	GPIO_PORTH_AHB_AMSEL_R = 0x00;
	GPIO_PORTP_AMSEL_R = 0x00;
	GPIO_PORTB_AHB_AMSEL_R = 0x00;
	
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTA_AHB_PCTL_R = 0x11;
	GPIO_PORTQ_PCTL_R = 0;
	GPIO_PORTH_AHB_PCTL_R = 0x0;
	GPIO_PORTP_PCTL_R = 0;
	GPIO_PORTB_AHB_PCTL_R = 0x0;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1
	GPIO_PORTA_AHB_DIR_R = 0xF0;
	GPIO_PORTQ_DIR_R = 0xF;
	GPIO_PORTH_AHB_DIR_R = 0xF;
	GPIO_PORTP_DIR_R = 0xFF;
	GPIO_PORTB_AHB_DIR_R = 0xFF;
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTA_AHB_AFSEL_R = 0x3;
	GPIO_PORTQ_AFSEL_R = 0x00;
	GPIO_PORTH_AHB_AFSEL_R = 0x0;
	GPIO_PORTP_AFSEL_R = 0x00;
	GPIO_PORTB_AHB_AFSEL_R = 0x0;
	
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;   //Bit0 e bit1
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTA_AHB_DEN_R = 0xFF;
	GPIO_PORTQ_DEN_R = 0xF;
	GPIO_PORTH_AHB_DEN_R = 0xF;
	GPIO_PORTP_DEN_R = 0xFF;
	GPIO_PORTB_AHB_DEN_R = 0xFF;

	GPIO_PORTP_DATA_R = 1 << 5;
	GPIO_PORTB_AHB_DATA_R = 0;
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //Bit0 e bit1	

}	

// -------------------------------------------------------------------------------
// Função PortJ_Input
// Lê os valores de entrada do port J
// Parâmetro de entrada: Não tem
// Parâmetro de saída: o valor da leitura do port
uint32_t PortJ_Input(void)
{
	return GPIO_PORTJ_AHB_DATA_R;
}

void acenderLeds(uint8_t leds){
    uint8_t preserved_bits = GPIO_PORTA_AHB_DATA_R & 0x0F;
    GPIO_PORTA_AHB_DATA_R = preserved_bits | (leds & 0xF0);
    GPIO_PORTQ_DATA_R = leds & 0x0F;
}

// -------------------------------------------------------------------------------
// Função PortN_Output
// Escreve os valores no port N
// Parâmetro de entrada: Valor a ser escrito
// Parâmetro de saída: não tem
void PortN_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
    //agora vamos fazer o OR com o valor recebido na função
    temp = temp | valor;
    GPIO_PORTN_DATA_R = temp; 
}

// -------------------------------------------------------------------------------
// Função PortH_Output
// Escreve os valores no port H
// Parâmetro de entrada: Valor a ser escrito
// Parâmetro de saída: não tem
void PortH_Output(uint32_t valor)
{
    uint32_t temp;
    // vamos zerar somente os bits menos significativos
    // para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTH_AHB_DATA_R & 0x00;
    // agora vamos fazer o OR com o valor recebido na função
    temp = temp | valor;
    GPIO_PORTH_AHB_DATA_R = temp;
}

//Handler do timer acionado anteriormente
void Timer0A_Handler(){
		TIMER0_ICR_R = 0x1; // tem que resetar essa flag
		if (pisca == 0){
			acenderLeds(leds);
			pisca =1;
		}else{
			pisca = 0;
			acenderLeds(0);
		}
}

uint32_t sec_2_clocks(float seg){
    uint32_t clocks_per_sec = 80000000;	// 80 MHz, ajustado para a sua configuração de clock
    uint32_t clocks = clocks_per_sec * seg;
    return clocks;
}

	//inicia um clock periodico no timer0 para com tempo de clocks
void init_periodic_timer_0(uint32_t clocks){
	SYSCTL_RCGCTIMER_R = 1;
	while ((SYSCTL_PRTIMER_R & 0x1) != 1);
	TIMER0_CTL_R = 0;
	TIMER0_CFG_R = 0;
	TIMER0_TAMR_R = 0x2;
	TIMER0_TAILR_R = clocks;
	TIMER0_TAPR_R = 0;
	TIMER0_ICR_R = 1;
	TIMER0_IMR_R = 1;
	NVIC_PRI4_R = (uint32_t) (4 << 29);
	NVIC_EN0_R = 1 << 19;
	TIMER0_CTL_R = 0x1;
}



