#include <stdint.h>
#ifndef TM4
#define TM4
#include "tm4c1294ncpdt.h"
#endif

void SysTick_Wait1ms(uint32_t delay);

// Definições para o estado das linhas e colunas
#define COL_MASK    0x0F
#define ROW1_MASK   0x17
#define ROW2_MASK   0x27
#define ROW3_MASK   0x47
#define ROW4_MASK   0x87

uint8_t mapKey(uint8_t row, uint8_t col);
uint8_t Keyboard_Read();




uint8_t Keyboard_Read() {
	uint8_t col, row = 0, key = 0;
	uint8_t columns[] = {ROW1_MASK, ROW2_MASK, ROW3_MASK, ROW4_MASK};
	for (col = 0; col < 4; col++) {
		// Configura a coluna como saída
		GPIO_PORTM_DIR_R = columns[col];
		
		SysTick_Wait1ms(5);

		// Zera a coluna
		GPIO_PORTM_DATA_R &= 0xF;

		// Lê as linhas
		row = GPIO_PORTL_DATA_R & 0xF;

		// Debounce
		if (row != 0xF) {
			key = mapKey(row, col);
			SysTick_Wait1ms(500);
				if (mapKey((GPIO_PORTL_DATA_R & 0xF), col) == key)
						break;
		}
	}
	// Restaura a configuração original
	GPIO_PORTM_DIR_R = 0xF;
	return key;
}



uint8_t mapKey(uint8_t row, uint8_t col) {
    static const char keyMap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    switch (row) {
        case 0xE: return keyMap[0][col];
        case 0xD: return keyMap[1][col];
        case 0xB: return keyMap[2][col];
        case 0x7: return keyMap[3][col];
        default: return 0;
    }
}