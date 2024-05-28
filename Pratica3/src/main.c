// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <time.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif
#define LED0 0x1
#define LED1 0x2
#define LED2 0x4
#define LED3 0x8
#define LED4 0x10
#define LED5 0x20
#define LED6 0x40
#define LED7 0x80

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);





void UART_Init(void);
void Transmissao(uint32_t var);
uint8_t Recepcao(uint32_t* var);
void Timer0A_Handler();
uint32_t ms_2_clocks(uint16_t ms);
void init_periodic_timer_0(uint32_t clocks);




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


uint8_t leds = 0;



int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	UART_Init();
	init_periodic_timer_0(sec_2_clocks(0.1));
	uint32_t receb;
	while (1)
	{	
		if (Recepcao(&receb))
			Transmissao(receb);
				

	}
}


