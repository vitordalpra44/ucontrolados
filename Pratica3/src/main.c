// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);
void UART_Init(void);
void Transmissao(char var);
void Recepcao(char* var, uint8_t* flag_recebeu);
void Timer0A_Handler();
uint32_t ms_2_clocks(uint16_t ms);
void init_periodic_timer_0(uint32_t clocks);

uint32_t ms_2_clocks(uint16_t ms){
    uint32_t clocks_per_sec = 80000000; // 80 MHz, ajustado para a sua configuração de clock
    uint32_t clocks = (clocks_per_sec * ms) / 1000;
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


int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	//UART_Init();
	init_periodic_timer_0(ms_2_clocks(100));
	//PortN_Output(1);
	int i =0;
	while (1)
	{	
//loop de cpu quente
	}
	return 0;
}


