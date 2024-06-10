#include <stdint.h>
#include "tm4c1294ncpdt.h"


void SysTick_Wait1ms(uint32_t delay);

// Definições para o estado das linhas e colunas
#define COL_MASK    0x07
#define ROW1_MASK   0x10
#define ROW2_MASK   0x20
#define ROW3_MASK   0x40
#define ROW4_MASK   0x80

void setColumnOutput(uint8_t col);
uint8_t readRow(void);
void debounce(uint32_t delay);
uint8_t mapKey(uint8_t row, uint8_t col);
uint8_t Keyboard_Read(void);
uint8_t Keyboard_Read(void) {
    uint8_t col, row = 0, key;
    uint8_t columns[] = {ROW1_MASK, ROW2_MASK, ROW3_MASK, ROW4_MASK};
    


    for (col = 0; col < 4; col++) {
        // Configura a coluna como saída
        setColumnOutput(columns[col]);

        // Aguarda para estabilizar
        SysTick_Wait1ms(5);

        // Zera a coluna
        GPIO_PORTM_DATA_R &= ~COL_MASK;

        // Lê as linhas
        row = readRow();

        // Debounce
        if (row != 0xF) {
					  key = mapKey(row, col);
					  SysTick_Wait1ms(500);
						if (mapKey(readRow(), col) == key)
							break;
						
        }

       SysTick_Wait1ms(500);


    }

    // Restaura a configuração original
    GPIO_PORTM_DIR_R &= ~ROW1_MASK & ~ROW2_MASK & ~ROW3_MASK & ~ROW4_MASK;
		return key;
}

void setColumnOutput(uint8_t col) {
    GPIO_PORTM_DIR_R = (GPIO_PORTM_DIR_R & ~COL_MASK) | col;
}

uint8_t readRow(void) {
    return GPIO_PORTL_DATA_R & 0xF;
}

void debounce(uint32_t delay) {
    SysTick_Wait1ms(delay);
}

uint8_t mapKey(uint8_t row, uint8_t col) {
    static const char keyMap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    switch (row) {
        case 0xE: return keyMap[col][0];
        case 0xD: return keyMap[col][1];
        case 0xB: return keyMap[col][2];
        case 0x7: return keyMap[col][3];
        default: return 0;
    }
}

