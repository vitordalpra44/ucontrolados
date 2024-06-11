#include "pwm.h"
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif
#define CLK 80000000
#define GPIO_PORTF (0x20) 
#define GPIO_PORTE (0x10)


uint8_t pwm = 0;
uint8_t on =0;

/* @name setPWM
* @brief dado um valor inteiro entre -100 e 100, seta um pwm para a saída F2, sendo entrada 0 parado e entrada -100 ou 100 velocidade máxima.
* O sinal controla a direção de movimento, sendo negativo, setada as saídas da ponte H para E0 = 1 E1 = 0, e positivo E0 =0 e E1 = 1.
*/
void setPWM(int8_t _pwm)
{
	uint8_t dir=0;
	if(_pwm<0)
	{
		dir = 1;
		_pwm = -_pwm;
	}
	
	if(_pwm > 100)
		_pwm = 100;
	pwm = (uint8_t) _pwm;
	
	if(pwm == 0)
		GPIO_PORTE_AHB_DATA_R = GPIO_PORTE_AHB_DATA_R & ~0x3;
	else if(dir)
		GPIO_PORTE_AHB_DATA_R = GPIO_PORTE_AHB_DATA_R & ~0x3 | 0x1;
	else
		GPIO_PORTE_AHB_DATA_R = GPIO_PORTE_AHB_DATA_R & ~0x3 | 0x2;
}

//faz setup dos pinos E0 e E1, e F2 para o controle de direção do pwm
void setupPWM()
{
	
	SYSCTL_RCGCGPIO_R |= (GPIO_PORTE | GPIO_PORTF);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTE | GPIO_PORTF) ) != (GPIO_PORTE | GPIO_PORTF) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTE_AHB_AMSEL_R &= ~0x3; //apenas os bits 0 e 1
	GPIO_PORTF_AHB_AMSEL_R &= ~0x04; // apenas o bit 2
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTE_AHB_PCTL_R &= ~0xFF;
	GPIO_PORTF_AHB_PCTL_R &= ~0xF00;


	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTE_AHB_DIR_R |= 0x03;
	GPIO_PORTF_AHB_DIR_R |= 0x04;
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTE_AHB_AFSEL_R &= ~0x3;
	GPIO_PORTF_AHB_AFSEL_R &= ~0x4;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTE_AHB_DEN_R |= 0x3;
	GPIO_PORTF_AHB_DEN_R |= 0x4;
	
	//setup do timer para o pwm
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2;
	while((SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R2) != SYSCTL_PRTIMER_R2){};
	TIMER2_CTL_R &= ~TIMER_CTL_TAEN;
	TIMER2_CFG_R = 0;
	TIMER2_TAMR_R = 0x2;
	TIMER2_TAILR_R  = 80000; //8M
	TIMER2_TAPR_R = 0;
	TIMER2_ICR_R |= TIMER_ICR_TATOCINT;
	TIMER2_IMR_R |= TIMER_IMR_TATOIM;
	NVIC_PRI5_R &= ~NVIC_PRI5_INT23_M;
	NVIC_PRI5_R |= 5<<29;
	NVIC_EN0_R = 1 << 23;
	TIMER2_CTL_R |= TIMER_CTL_TAEN;
	
}
void Timer2A_Handler()
{
	TIMER2_ICR_R = 0x01;
	//atualiza periodo
	if(on)
		TIMER2_TAILR_R = 80000*(100-pwm)/100;
	else
		TIMER2_TAILR_R = 80000*pwm/100;
	on = !on;
	//atualiza saída do pwm
	GPIO_PORTF_AHB_DATA_R = GPIO_PORTF_AHB_DATA_R & ~0x4 | (on) ? 0x4 : 0x0;
	
}