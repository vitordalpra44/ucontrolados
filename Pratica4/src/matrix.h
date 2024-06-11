#ifndef _MATRIX
#define _MATRIX
#include <stdint.h>
void SetupMatrix(void);
void ReadMatrix(void);
void ClearBuffer(void);
extern char buffer[20];
extern int buffer_size;
#endif