// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "step_motor.h"
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif

#define loop_principal "\
\n\r\
-------------------------------------------------------------------------\n\r\
                       BEM VINDO AO GERENCIADOR DE MOTOR                 \n\r\
-------------------------------------------------------------------------\n\r\
1. Ver valores setados (velocidade, direcao e voltas)\n\r\
2. Setar velocidade\n\r\
3. Setar direcao\n\r\
4. Setar voltas\n\r\
5. Acionar o motor\n\r\
\n\r\
Digite: \
"
#define escolha_errada "\n\r\
Opcao inexistente...\
"
#define dividir_secao "\
\n\r\
-------------------------------------------------------------------------"
#define setar_velocidade "\n\rSETANDO VELOCIDADE  (0 - meio passo, 1 - passo completo)\n\rValor: "
#define setar_direcao "\n\rSETANDO DIRECAO  (0 - horario, 1 - anti-horario)\n\rValor: "
#define setar_voltas "\n\rSETANDO VOLTAS (min 1, max 10) \n\rValor: "

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void GPIO_Init(void);
void UART_Init(void);
void Recepcao(uint32_t* var);
uint32_t sec_2_clocks(float sec);
void init_periodic_timer_0(uint32_t clocks);
void EnviarString(unsigned char* string);
void Timer0A_Handler();


uint8_t leds = 0;
uint8_t pisca = 0;
uint32_t velocidade = '1';
uint32_t direcao = '0';
uint32_t voltas = 1;

int8_t verificaDec(uint32_t dec) {return (dec == '0' || dec == '1' || dec == '2' || dec == '3' || dec == '4' || dec == '5' ||dec == '6' || dec == '7' ||dec == '8' || dec == '9');}


int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	UART_Init();
	init_periodic_timer_0(sec_2_clocks(0.1));
	uint32_t retcode = 0, retcode2=0;
	unsigned char buffer[200] = {0};

	while (1)
	{	
		EnviarString(loop_principal);
		Recepcao(&retcode);
		EnviarString(dividir_secao);
		if (retcode == '1'){
			snprintf((char *) buffer, 200, "\n\rVelocidade: %c\n\rVoltas: %d\n\rDirecao: %c", velocidade, voltas, direcao);
			EnviarString(buffer);		
			memset(buffer, 0, 200);
			
		}else if (retcode == '2'){
			EnviarString(setar_velocidade);
			Recepcao(&retcode);
			if (retcode == '0') velocidade = '0';
			else if (retcode == '1') velocidade = '1';
			else{ 
				EnviarString(escolha_errada);
				continue;
			}
			EnviarString("\n\rValor alterado com sucesso");

		}else if (retcode == '3'){
			EnviarString(setar_direcao);
			Recepcao(&retcode);
			if (retcode == '0') direcao = '0';
			else if (retcode == '1') direcao = '1';
			else{ 
				EnviarString(escolha_errada);
				continue;
			}
			EnviarString("\n\rValor alterado com sucesso");
		}else if (retcode == '4'){
			char voltas_char[2] = {0};
				EnviarString(setar_voltas);
				Recepcao(&retcode);
				if ((!verificaDec(retcode))){
						EnviarString(escolha_errada);
						goto final;
				}
				voltas_char[0] = retcode;
				Recepcao(&retcode2);
				if ((!verificaDec(retcode2)) && retcode2 != 13){
						EnviarString(escolha_errada);
						goto final;
				}else if (verificaDec(retcode2)){
						voltas_char[1] = retcode2;
				}
				voltas = atoi(voltas_char);
				if (voltas > 10){
					voltas = 10;
					EnviarString("\n\rSetando voltas para 10");
				}else if (voltas == 0){
					voltas = 1;
					EnviarString("\n\rSetando voltas para 1");
				}
				EnviarString("\n\rValor alterado com sucesso");



		}else if (retcode == '5'){
				rotate_step_motor();
				EnviarString("\n\rFIM");
		}else{
			EnviarString(escolha_errada);

		}
		final:
		EnviarString("\n\r\n\r");
	}
}


