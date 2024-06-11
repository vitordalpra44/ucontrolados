#include <stdint.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif
#include <string.h>
// Protótipos de funções
void SysTick_Wait1us(uint32_t delay);
void LCD_Break_Line_Set();
void LCD_Print_Char(uint8_t character);
void limparLCD();
void imprimirString(const char* string);

volatile uint8_t LCD_CHAR_COUNTER = 0;

#define GPIO_PORTK_DATA_R_MASK 0xFF
#define GPIO_PORTM_DATA_R_MASK 0x07

void LCD_Init() {
    // DATA WRITE
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | 0x38;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x04;
    SysTick_Wait1us(10);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK);
    SysTick_Wait1us(40);

    // DATA WRITE
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | 0x06;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x04;
    SysTick_Wait1us(10);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK);
    SysTick_Wait1us(40);

    // DATA WRITE
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | 0x0E;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x04;
    SysTick_Wait1us(10);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK);
    SysTick_Wait1us(40);

    // DATA WRITE
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | 0x01;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x04;
    SysTick_Wait1us(40);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK);
    SysTick_Wait1us(1640);
}

void LCD_Print_Char(uint8_t character) {
    // Imprime o caractere no LCD
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | character;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x05;
    SysTick_Wait1us(40);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x01;
    SysTick_Wait1us(40);

    // Incrementa o contador de caracteres
    LCD_CHAR_COUNTER++;
    if (LCD_CHAR_COUNTER == 16) {
        LCD_Break_Line_Set();
    }
}

void LCD_Break_Line_Set() {
    // Reseta o contador de caracteres
    LCD_CHAR_COUNTER = 0;

    // Envia comando de quebra de linha (0xC0)
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | 0xC0;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x04;
    SysTick_Wait1us(10);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK);
    SysTick_Wait1us(40);
}

void limparLCD() {
    // Envia comando para limpar o display (0x01)
    GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R & ~GPIO_PORTK_DATA_R_MASK) | 0x01;

    // EN = 1
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK) | 0x04;
    SysTick_Wait1us(40);

    // EN = 0
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R & ~GPIO_PORTM_DATA_R_MASK);
    SysTick_Wait1us(1640);

    // Reseta o contador de caracteres
    LCD_CHAR_COUNTER = 0;
}

void imprimirString(const char* string) {
		if (strlen(string) < 32){
			limparLCD();  // Limpa o LCD antes de imprimir uma nova string
			while (*string) {
					LCD_Print_Char(*string++);
			}
		}
}
