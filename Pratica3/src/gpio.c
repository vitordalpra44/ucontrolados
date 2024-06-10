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
extern int interrupcao;
void SysTick_Wait1ms(uint32_t delay);

// -------------------------------------------------------------------------------
// Fun��o GPIO_Init
// Inicializa os ports J e N
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: N�o tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ | GPIO_PORTH | GPIO_PORTB | GPIO_PORTP);
	//1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ | GPIO_PORTH | GPIO_PORTB | GPIO_PORTP) ) != (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ | GPIO_PORTH | GPIO_PORTB | GPIO_PORTP));
	
	// 2. Limpar o AMSEL para desabilitar a anal�gica
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

	// 4. DIR para 0 se for entrada, 1 se for sa�da
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1
	GPIO_PORTA_AHB_DIR_R = 0xF0;
	GPIO_PORTQ_DIR_R = 0xF;
	GPIO_PORTH_AHB_DIR_R = 0xF;
	GPIO_PORTP_DIR_R = 0xFF;
	GPIO_PORTB_AHB_DIR_R = 0xFF;
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun��o alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTA_AHB_AFSEL_R = 0x3;
	GPIO_PORTQ_AFSEL_R = 0x00;
	GPIO_PORTH_AHB_AFSEL_R = 0x0;
	GPIO_PORTP_AFSEL_R = 0x00;
	GPIO_PORTB_AHB_AFSEL_R = 0x0;
	
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x01;   //Bit0
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTA_AHB_DEN_R = 0xFF;
	GPIO_PORTQ_DEN_R = 0xF;
	GPIO_PORTH_AHB_DEN_R = 0xF;
	GPIO_PORTP_DEN_R = 0xFF;
	GPIO_PORTB_AHB_DEN_R = 0xFF;

	GPIO_PORTP_DATA_R = 1 << 5;
	GPIO_PORTB_AHB_DATA_R = 0;
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x01;   //Bit0

	GPIO_PORTJ_AHB_IM_R = 0x0;
	GPIO_PORTJ_AHB_IS_R = 0x0;
	GPIO_PORTJ_AHB_IBE_R = 0x0;
	GPIO_PORTJ_AHB_IEV_R = 0x0;
	GPIO_PORTJ_AHB_ICR_R = 0x1;
	GPIO_PORTJ_AHB_IM_R = 0x1;
	NVIC_EN1_R = 0x1 << 19;
	NVIC_PRI12_R = 5 << 29;

}	

// -------------------------------------------------------------------------------
// Fun��o PortJ_Input
// L� os valores de entrada do port J
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: o valor da leitura do port
//uint32_t PortJ_Input(void)
//{
//	return GPIO_PORTJ_AHB_DATA_R;
//}

void GPIOPortJ_Handler(void){
	interrupcao = 1;
	GPIO_PORTJ_AHB_ICR_R = 0x1;
}

void acenderLeds(uint8_t leds){
    uint8_t preserved_bits = GPIO_PORTA_AHB_DATA_R & 0x0F;
    GPIO_PORTA_AHB_DATA_R = preserved_bits | (leds & 0xF0);
    GPIO_PORTQ_DATA_R = leds & 0x0F;
}

// -------------------------------------------------------------------------------
// Fun��o PortN_Output
// Escreve os valores no port N
// Par�metro de entrada: Valor a ser escrito
// Par�metro de sa�da: n�o tem
void PortN_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amig�vel nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
    //agora vamos fazer o OR com o valor recebido na fun��o
    temp = temp | valor;
    GPIO_PORTN_DATA_R = temp; 
}

// -------------------------------------------------------------------------------
// Fun��o PortH_Output
// Escreve os valores no port H
// Par�metro de entrada: Valor a ser escrito
// Par�metro de sa�da: n�o tem
void PortH_Output(uint32_t valor)
{
    uint32_t temp;
    // vamos zerar somente os bits menos significativos
    // para uma escrita amig�vel nos bits 0 e 1
    temp = GPIO_PORTH_AHB_DATA_R & 0x00;
    // agora vamos fazer o OR com o valor recebido na fun��o
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
    uint32_t clocks_per_sec = 80000000;	// 80 MHz, ajustado para a sua configura��o de clock
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



