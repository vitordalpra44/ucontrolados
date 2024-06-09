#ifndef _ADC
#define _ADC
#include <stdint.h>
#define ADC_RES 4096
void PortE_Output(uint32_t valor);
void setupADC(void);
int readADC();
#endif