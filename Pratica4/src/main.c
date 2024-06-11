// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "pwm.h"
#include <stdlib.h>
#include <string.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void GPIO_Init(void);
void GPIO_Init_LEDS(void);

void imprimirString(const char* string);
void limparLCD();
uint8_t Keyboard_Read();
void LCD_Init();

void int2char(int a, char buffer[4]);
void acenderLeds(uint8_t leds);
int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	LCD_Init();
	setupADC();
	setupPWM();
	setPWM(0);
	uint8_t isPot = 1;
	uint8_t key = 0;
	int32_t adc_atual = 0;
	int32_t adc = 0;
	int8_t cw = 1;
	char print_aux = 0;
	char string[32] = "VALOR: ";
	
	char buffer[5] = {0};
	//int2char(15, buffer);
	//strcat(string, buffer);
	imprimirString("MOTOR PARADO    APERTE '1'");
	//imprimirString((const char*) string);
	while(Keyboard_Read() != '1');
	
	imprimirString("1-POTENCIOMETRO 2-POT + TECLADO");
	
	while(key != '1' && key != '2'){
		key = Keyboard_Read();
	}
	if(key == '1'){
		while(1){
			char string[32] = "VALOR: ";
			adc = readADC();
			adc = (adc*100)/(4096/2) - 100;
			
			if(adc> adc_atual)
				adc_atual++;
			else if(adc<adc_atual)
				adc_atual--;
			
			setPWM(adc_atual);
			int2char(adc_atual, buffer);
			strcat(string, buffer);
			if(print_aux == 5){
			imprimirString((const char*) string);
				print_aux = 0;}
			else
				print_aux++;
			SysTick_Wait1ms(50);
		}
	}
	else if (key == '2'){
		while(1){
			char read = Keyboard_Read();
			if(read == '1')
				cw = 1;
			else if(read == '2')
				cw = -1;
			
			char string[32] = "1-CW 2-CCW      VALOR: ";
			adc = readADC();
			adc = cw*(adc*100)/4096;
			
			
			if(adc> adc_atual)
				adc_atual++;
			else if(adc<adc_atual)
				adc_atual--;
			
			setPWM(adc_atual);
			int2char(adc_atual, buffer);
			strcat(string, buffer);
			if(print_aux == 5){
			imprimirString((const char*) string);
				print_aux = 0;}
			else
				print_aux++;			SysTick_Wait1ms(50);
		}
	}
	return 0;
}

void int2char(int a, char buffer[4]){
	if(a<0){
		a = -a;
		buffer[0] = '-';
	}
	else{
		buffer[0] = '+';
	}
	buffer[1] = '0' + a/100;
	buffer[2] = '0' + (a/10)%10;
	buffer[3] = '0' + a%10;
}

