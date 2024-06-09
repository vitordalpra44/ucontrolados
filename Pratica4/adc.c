#include "adc.h"

#include "tm4c1294ncpdt.h"

#define GPIO_PORTE (0x10)


// -------------------------------------------------------------------------------
// Função setupADC
// Inicializa o port E4 como um conversor analógico digital
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void setupADC(void)
{
	//SETUP DO PINO E4
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (GPIO_PORTE);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTE) ) != (GPIO_PORTE) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTE_AHB_AMSEL_R |= 0x10;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTE_AHB_PCTL_R &= ~0xF0000;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTE_AHB_DIR_R &= ~0x10;

	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTE_AHB_AFSEL_R |= 0x10;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTE_AHB_DEN_R &= ~0x10;
	
		
	//SETUP DO ADC0
	// E4 eh o AIN9
	SYSCTL_RCGCADC_R = SYSCTL_RCGCADC_R0;
	while((SYSCTL_PRADC_R & SYSCTL_RCGCADC_R0) != SYSCTL_RCGCADC_R0 ){};
	ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0; // DESLIGA
	ADC0_PC_R = 0x7; // MAX TAXA DE AMOSTRAGEM
	ADC0_SSMUX0_R = 0x9; // AIN9 (E4)
	ADC0_SSCTL0_R = 0x6; // APENAS UMA LEITURA
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN0; // LIGA
	
	
	

}	

//devolve saída de 12bits do ADC
int readADC()
{
	ADC0_PSSI_R = 0x1; //PEDE SS0 para começar
	while((ADC0_RIS_R & 0x1)!=0x1){};
	int result = ADC0_SSFIFO0_R; //Retira da fila a entrada da SS0
	ADC0_ISC_R = 0x1; //CONFIRMA LEITURA da fila da SS0
	return result;
}

