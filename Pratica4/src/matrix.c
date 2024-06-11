#include "tm4c1294ncpdt.h"
#include "matrix.h"
char buffer[20];
int buffer_size;
uint32_t count[16];

uint16_t last_input;

#define GPIO_PORTL  0x400
#define GPIO_PORTM  0x800
char ConvertToASCII(char in);
void SetupMatrix()
{ 
	  SYSCTL_RCGCGPIO_R |= (GPIO_PORTL| GPIO_PORTM);
	  while((SYSCTL_PRGPIO_R & (GPIO_PORTL |GPIO_PORTM))!=(GPIO_PORTL |GPIO_PORTM)){};
		GPIO_PORTL_AMSEL_R &= ~0x0F;
		GPIO_PORTM_AMSEL_R &= ~0xF0;
	  GPIO_PORTL_PCTL_R = 0;
		GPIO_PORTM_PCTL_R = 0;
		GPIO_PORTL_DIR_R &= ~0x0F;
		GPIO_PORTM_DIR_R |= 0xF;
		GPIO_PORTL_AFSEL_R &= ~0x0F;
		GPIO_PORTM_AFSEL_R &= ~0xF0;
		GPIO_PORTM_DEN_R |= 0xF0;
		GPIO_PORTL_DEN_R |= 0x0F;
			
		last_input = 0;
		buffer_size = 0;
		int i;
		for(i=0;i<20;i++)
		{
			buffer[i]=0;
		}
		for(i=0;i<16;i++)
		{
			count[i]=0;
		}
}

void PortM_Output(uint32_t valor)
{
    GPIO_PORTM_DATA_R = (GPIO_PORTM_DATA_R& 0x0F) | (valor & 0xF0);
}

uint16_t PortL_Input()
{
	return GPIO_PORTL_DATA_R & 0xF;
}

uint16_t UpdateMatrix()
{
	int i;
	uint16_t output = 0;
	for(i=0; i<4;i++)
	{
		PortM_Output(0x10 << i);
		output = (output<<4) | PortL_Input();
	}
	output = ~output;
	uint16_t clicked = 0;
	for(i=0;i<16;i++)
	{
		if((output>> i) & 0x1)
		{
			count[i] += 1;
			if(count[i] > 1)
				clicked |= 1<<i;
		} else
		{
			count[i]=0;
		}
	}
	
	uint16_t aux = last_input;
	last_input = clicked;
	return ~aux & clicked;
}

void ReadMatrix()
{
	uint16_t out = UpdateMatrix();
	int i=0;
	while(buffer_size<20 && i< 16)
	{
		if(out & 1<<i)
		{
			buffer[buffer_size] = ConvertToASCII(i);
			buffer_size+=1;
		}
	}
}

void ClearBuffer()
{
	buffer_size = 0;
}

char ConvertToASCII(char in)
{
	switch(in)
	{
		case 0xC:
			return '1';
		case 0x8:
			return '2';
		case 0x4:
			return '3';
		case 0xD:
			return '4';
		case 0x9:
			return '5';
		case 0x5:
			return '6';
		case 0xE:
			return '7';
		case 0xA:
			return '8';
		case 0x6:
			return '9';
		case 0xF:
			return '*';
		case 0xB:
			return '0';
		case 0x7:
			return '#';
		case 0x0:
			return 'A';
		case 0x1:
			return 'B';
		case 0x2:
			return 'C';
		case 0x3:
			return 'D';
		default:
			return 'U';
	}
}	