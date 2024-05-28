// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTJ  (0x0100) //bit 8
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTA (0x1)
#define GPIO_PORTQ (0x4000)


#define LED0 0x1
#define LED1 0x2
#define LED2 0x4
#define LED3 0x8
#define LED4 0x10
#define LED5 0x20
#define LED6 0x40
#define LED7 0x80



extern uint8_t leds;

void SysTick_Wait1ms(uint32_t delay);

// -------------------------------------------------------------------------------
// Fun��o GPIO_Init
// Inicializa os ports J e N
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: N�o tem
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ);
	//1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ) ) != (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTA | GPIO_PORTQ ) );
	
	// 2. Limpar o AMSEL para desabilitar a anal�gica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTQ_AMSEL_R = 0x00;


	
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTA_AHB_PCTL_R = 0x11;
	GPIO_PORTQ_PCTL_R = 0;

	// 4. DIR para 0 se for entrada, 1 se for sa�da
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1
	GPIO_PORTA_AHB_DIR_R = 0xF0;
	GPIO_PORTQ_DIR_R = 0xF;
	
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun��o alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTA_AHB_AFSEL_R = 0x3;
	GPIO_PORTQ_AFSEL_R = 0x00;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;   //Bit0 e bit1
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTA_AHB_DEN_R = 0xFF;
	GPIO_PORTQ_DEN_R = 0xF;

	
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //Bit0 e bit1	

}	

// -------------------------------------------------------------------------------
// Fun��o PortJ_Input
// L� os valores de entrada do port J
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: o valor da leitura do port
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






//Handler do timer acionado anteriormente
void Timer0A_Handler(){
		TIMER0_ICR_R = 0x1; // tem que resetar essa flag
		if(leds == LED0)
			leds = LED1;
		else if (leds == LED1)
			leds = LED2;
		else if (leds == LED2)
			leds = LED3;
		else if (leds == LED3)
			leds = LED4;
		else if (leds == LED4)
			leds = LED5;
		else if (leds == LED5)
			leds = LED6;
		else if (leds == LED6)
			leds = LED7;
		else if (leds == LED7)
			leds = LED0;
		else
			leds = LED0;
		acenderLeds(leds);
}




