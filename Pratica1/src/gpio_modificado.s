; gpio.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 24/08/2020

; -------------------------------------------------------------------------------
        THUMB                        ; Instru��es do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declara��es EQU - Defines
; ========================
; Defini��es de Valores
BIT0	EQU 2_0001
BIT1	EQU 2_0010
; ========================
; Defini��es dos Registradores Gerais
SYSCTL_RCGCGPIO_R	 EQU	0x400FE608
SYSCTL_PRGPIO_R		 EQU    0x400FEA08
; ========================
; Defini��es dos Ports

; Common port offsets
LOCK_OFFSET           EQU 0x520
CR_OFFSET             EQU 0x524
AMSEL_OFFSET          EQU 0x528
PCTL_OFFSET           EQU 0x52C
DIR_OFFSET            EQU 0x400
AFSEL_OFFSET          EQU 0x420
DEN_OFFSET            EQU 0x51C
PUR_OFFSET            EQU 0x510
DATA_OFFSET           EQU 0x3FC
DATA_BITS_OFFSET      EQU 0x000

; Port base addresses, define according to your microcontroller's datasheet
GPIO_PORTA_BASE		  EQU 0x40058000
GPIO_PORTB_BASE       EQU 0x40059000
GPIO_PORTJ_BASE       EQU 0x40060000
GPIO_PORTN_BASE       EQU 0x40064000
GPIO_PORTP_BASE       EQU 0x40065000
GPIO_PORTQ_BASE       EQU 0x40066000

GPIO_PORTA            EQU    2_000000000000001
GPIO_PORTB            EQU    2_000000000000010
GPIO_PORTJ            EQU    2_000000100000000
GPIO_PORTN            EQU    2_001000000000000
GPIO_PORTP            EQU    2_010000000000000
GPIO_PORTQ            EQU    2_100000000000000
	


; -------------------------------------------------------------------------------
; �rea de C�digo - Tudo abaixo da diretiva a seguir ser� armazenado na mem�ria de 
;                  c�digo
        AREA    |.text|, CODE, READONLY, ALIGN=2

		; Se alguma fun��o do arquivo for chamada em outro arquivo	
        EXPORT GPIO_start            ; Permite chamar GPIO_Init de outro arquivo
		EXPORT SetOutputBits
		EXPORT GetOutputBits
		EXPORT SwitchPP
		EXPORT SwitchPB
		EXPORT GetInputJ

;--------------------------------------------------------------------------------
; Fun��o GPIO_start
; Par�metro de entrada: N�o tem
; Par�metro de sa�da: N�o tem
GPIO_start
;=====================

; Ativar o clock para as portas
        LDR     R0, =SYSCTL_RCGCGPIO_R
		
		
		
		; Carrega o endere�o base do port A
		LDR     R1, =GPIO_PORTA

		; Aplica OR com o endere�o base do port B
		LDR     R2, =GPIO_PORTB
		ORR     R1, R1, R2				
		
		; Aplica OR com o endere�o base do port J
		LDR     R2, =GPIO_PORTJ
		ORR     R1, R1, R2

		; Aplica OR com o endere�o base do port N
		LDR     R2, =GPIO_PORTN
		ORR     R1, R1, R2

		; Aplica OR com o endere�o base do port P
		LDR     R2, =GPIO_PORTP
		ORR     R1, R1, R2
		
		; Aplica OR com o endere�o base do port Q
		LDR     R2, =GPIO_PORTQ
		ORR     R1, R1, R2			
		STR     R1, [R0]

        ; Espera at� que todas as portas estejam prontas
        LDR     R0, =SYSCTL_PRGPIO_R
EsperaGPIO
        LDR     R1, [R0]
        TST     R1, R1
        BEQ     EsperaGPIO

        ; Configurar portas
		
		
		
		;Port A - SAIDA - PA4 - PA7
        MOV     R2, #0x00
        LDR     R0, =GPIO_PORTA_BASE + AMSEL_OFFSET
        STR     R2, [R0]  ; Desabilita fun��o anal�gica (PADR�O)
		
        LDR     R0, =GPIO_PORTA_BASE + PCTL_OFFSET
        STR     R2, [R0]  ; Limpa PCTL para selecionar GPIO (PADR�O)
		
        LDR     R0, =GPIO_PORTA_BASE + DIR_OFFSET
        MOV     R2, #2_11110000 ; Define os pinos A7-A4 como sa�da
        STR     R2, [R0]
		

        LDR     R0, =GPIO_PORTA_BASE + AFSEL_OFFSET
		MOV 	R2, #0x00
        STR     R2, [R0]  ; Desabilita fun��es alternativas
		

        LDR     R0, =GPIO_PORTA_BASE + DEN_OFFSET
		MOV     R2, #2_11110000 ;
        STR     R2, [R0]  ; Habilita fun��o digital
		
		
		;Se o pino for de entrada:
        ;LDR     R0, =GPIO_PORTA_BASE + PUR_OFFSET
        ;MOV     R2, #2_11110000 ; Habilita resistores de pull-up para pinos espec�ficos
        ;STR     R2, [R0]
		
		;Port B - SAIDA - PB4 e PB5
        MOV     R2, #0x00
        LDR     R0, =GPIO_PORTB_BASE + AMSEL_OFFSET
        STR     R2, [R0]  ; Desabilita fun��o anal�gica (PADR�O)
		
        LDR     R0, =GPIO_PORTB_BASE + PCTL_OFFSET
        STR     R2, [R0]  ; Limpa PCTL para selecionar GPIO (PADR�O)
		
        LDR     R0, =GPIO_PORTB_BASE + DIR_OFFSET
        MOV     R2, #2_00110000 ; Define os pinos como saida
        STR     R2, [R0]
		

        LDR     R0, =GPIO_PORTB_BASE + AFSEL_OFFSET
		MOV 	R2, #0x00
        STR     R2, [R0]  ; Desabilita fun��es alternativas
		

        LDR     R0, =GPIO_PORTB_BASE + DEN_OFFSET
		MOV     R2, #2_00110000 ;
        STR     R2, [R0]  ; Habilita fun��o digital
		
		;Port P - SAIDA - PP5
        MOV     R2, #0x00
        LDR     R0, =GPIO_PORTP_BASE + AMSEL_OFFSET
        STR     R2, [R0]  ; Desabilita fun��o anal�gica (PADR�O)
		
        LDR     R0, =GPIO_PORTP_BASE + PCTL_OFFSET
        STR     R2, [R0]  ; Limpa PCTL para selecionar GPIO (PADR�O)
		
        LDR     R0, =GPIO_PORTP_BASE + DIR_OFFSET
        MOV     R2, #2_00100000 ; Define os pinos como saida
        STR     R2, [R0]
		

        LDR     R0, =GPIO_PORTP_BASE + AFSEL_OFFSET
		MOV 	R2, #0x00
        STR     R2, [R0]  ; Desabilita fun��es alternativas
		

        LDR     R0, =GPIO_PORTP_BASE + DEN_OFFSET
		MOV     R2, #2_00100000 ;
        STR     R2, [R0]  ; Habilita fun��o digital

		
		
		;Port Q - SAIDA - PQ3 - PQ0
        MOV     R2, #0x00
        LDR     R0, =GPIO_PORTQ_BASE + AMSEL_OFFSET
        STR     R2, [R0]  ; Desabilita fun��o anal�gica (PADR�O)
		
        LDR     R0, =GPIO_PORTQ_BASE + PCTL_OFFSET
        STR     R2, [R0]  ; Limpa PCTL para selecionar GPIO (PADR�O)
		
        LDR     R0, =GPIO_PORTQ_BASE + DIR_OFFSET
        MOV     R2, #2_00001111 ; Define os pinos A7-A4 como sa�da
        STR     R2, [R0]
		

        LDR     R0, =GPIO_PORTQ_BASE + AFSEL_OFFSET
		MOV 	R2, #0x00
        STR     R2, [R0]  ; Desabilita fun��es alternativas
		

        LDR     R0, =GPIO_PORTQ_BASE + DEN_OFFSET
		MOV     R2, #2_00001111 ;
        STR     R2, [R0]  ; Habilita fun��o digital
		
		;Port J - ENTRADA - J0 - J1
        MOV     R2, #0x00
        LDR     R0, =GPIO_PORTJ_BASE + AMSEL_OFFSET
        STR     R2, [R0]  ; Desabilita fun��o anal�gica (PADR�O)
		
        LDR     R0, =GPIO_PORTJ_BASE + PCTL_OFFSET
        STR     R2, [R0]  ; Limpa PCTL para selecionar GPIO (PADR�O)
		
        LDR     R0, =GPIO_PORTJ_BASE + DIR_OFFSET
        MOV     R2, #2_00000000 ; Define os pinos A7-A4 como sa�da
        STR     R2, [R0]
		

        LDR     R0, =GPIO_PORTJ_BASE + AFSEL_OFFSET
		MOV 	R2, #0x00
        STR     R2, [R0]  ; Desabilita fun��es alternativas
		

        LDR     R0, =GPIO_PORTJ_BASE + DEN_OFFSET
		MOV     R2, #2_00000011 ;
        STR     R2, [R0]  ; Habilita fun��o digital
		
		
		;Se o pino for de entrada:
        LDR     R0, =GPIO_PORTJ_BASE + PUR_OFFSET
        MOV     R2, #2_00000011 ; Habilita resistores de pull-up para pinos espec�ficos
        STR     R2, [R0]

        BX      LR

; -------------------------------------------------------------------------------
SetOutputBits
    PUSH    {R1, R2, R3}                ; Salva os registradores R1, R2 e R3

    ; Isola os 8 bits menos significativos de R0
    MOV     R3, R0
    AND     R3, #0xFF      

    ; Zerar todos os bits de A e Q antes de setar os novos valores
    LDR     R1, =GPIO_PORTA_BASE + DATA_OFFSET
    MOV     R2, #0x00
    STR     R2, [R1]             ; Zera A7-A4
    
    LDR     R1, =GPIO_PORTQ_BASE + DATA_OFFSET
    STR     R2, [R1]             ; Zera Q3-Q0

    ; Trata os bits altos (A7-A4) de R3 para o port A
    LDR     R1, =GPIO_PORTA_BASE + DATA_OFFSET; Acessa o registrador correspondente a A4
    STR     R3, [R1]                     ; Escreve os bits em A7-A4

    ; Trata os bits baixos (Q3-Q0) de R3 para o port Q
    LDR     R1, =GPIO_PORTQ_BASE + DATA_OFFSET  ; Acessa o registrador de dados de Q
    STR     R3, [R1]                     ; Escreve os bits em Q3-Q0

    POP     {R1, R2, R3}                 ; Restaura os registradores R1, R2 e R3
    BX      LR                           ; Retorna da fun��o

GetOutputBits
	PUSH 	{R1, R2, LR, R3, R4}
	LDR     R1, =GPIO_PORTA_BASE + DATA_OFFSET
	LDR     R2, =GPIO_PORTQ_BASE + DATA_OFFSET
	LDR 	R3, [R1]
	and		r3, #0xF0
	ldr		R4, [R2]
	and		r4, #0xF
	
	orr		R3, R4
	mov 	R0, R3
	
	POP 	{R1, R2, LR, R3, R4}
	BX		LR

GetInputJ
	PUSH {R1}
	LDR	R1, =GPIO_PORTJ_BASE + DATA_OFFSET 
	LDR R0, [R1]                            
	POP {R1}
	BX LR									

SwitchPB
    PUSH    {R1, R2, LR}            

	MOV     R2, #0
	
    LDR     R1, =GPIO_PORTB_BASE + DATA_OFFSET
    STR     R2, [R1]                                
	
    BIC R2, #2_00110000                    
	ORR R0, R0, R2                         
	STR R0, [R1]                            
	
	POP    {R1, R2, LR}
	
	BX LR								

SwitchPP
	PUSH    {R1, R2, LR}        
    ; Zerar PP5
	MOV     R2, #0
    LDR     R1, =GPIO_PORTP_BASE + DATA_OFFSET  
    STR     R2, [R1]                             

	BIC R2, #2_00100000                     
	ORR R0, R0, R2                        
	STR R0, [R1]                       
	
	POP    {R1, R2, LR}
	
	BX LR	

Exit
    POP     {R1, R2, LR}                               ; Restaura os registradores
    BX      LR                                     ; Retorna da fun��o
	
	
    ALIGN                           ; garante que o fim da se��o est� alinhada 
    END                             ; fim do arquivo