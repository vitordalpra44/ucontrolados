#ifndef _ADC
#define _ADC
#include <stdint.h>
#define ADC_RES 4096
void setupADC(void);
int readADC();
#endif
