; pratica0.s
; Desenvolvido para a placa EK-TM4C1294XL
; VItor Dalprá / Gabriel Willig / Jo�o Mendes
; 24/03/2024
; Feito com base no exemplo.s do professor Guilherme de Santi Peron de Sistemas Microcontrolados da UTFPR.




; -------------------------------------------------------------------------------
        THUMB                        ; Instru��es do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declara��es EQU - Defines
;<NOME>         EQU <VALOR>
DISPLAY_9		EQU 2_01101111	
; -------------------------------------------------------------------------------
; �rea de Dados - Declara��es de vari�veis
		AREA  DATA, ALIGN=2

			

		; Se alguma vari�vel for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a vari�vel <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma vari�vel de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posi��o da RAM		


; -------------------------------------------------------------------------------
; �rea de C�digo - Tudo abaixo da diretiva a seguir ser� armazenado na mem�ria de 
;                  c�digo
        AREA    |.text|, CODE, READONLY, ALIGN=2
		
		; Se alguma fun��o do arquivo for chamada em outro arquivo	
        EXPORT START                ; Permite chamar a fun��o START a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma fun��o externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; fun��o <func>
		IMPORT  GPIO_start
		IMPORT  SetOutputBits
		IMPORT	GetOutputBits
		IMPORT  SwitchPB
		IMPORT 	SwitchPP
		IMPORT  SysTick_Wait1ms
		IMPORT  PLL_Init
		IMPORT  SysTick_Init  
		IMPORT 	GetInputJ

DISPLAY_NUMBERS   DCB   2_00111111, 2_00000110, 2_01011011, 2_01001111, 2_01100110, 2_01101101, 2_01111101, 2_00000111, 2_01111111, 2_01100111
LEDS_SEQUENCE   DCB   2_10000001, 2_01000010, 2_00100100, 2_00011000, 2_00100100, 2_01000010
;0 = 00111111
;1 = 00000110
;2 = 01011011
;3 = 01001111
;4 = 01100110
;5 = 01101101
;6 = 01111101
;7 = 00000111
;8 = 01111111
;9 = 01101111
;
; LEDS
; 10000001
; 01000010
; 00100100
; 00011000
; 00100100
; 01000010
; 10000001
; -------------------------------------------------------------------------------
; Fun��o main()
START 
; Comece o c�digo aqui <======================================================
	
	BL PLL_Init                  ;Chama a subrotina para alterar o clock do microcontrolador para 80MHz	
	BL SysTick_Init
	BL GPIO_start                 ;Chama a subrotina que inicializa os GPIO
	
	
	MOV R4, #2_00000011			;REGISTRADOR 4 QUE GUARDA ESTADO DOS BOTÕES
	MOV R5, #0					;REGISTRADOR 5 QUE GUARDA A FLAG DO CRONOMETRO
	MOV R6, #2_00000001			;REGISTRADOR 6 QUE GUARDA O PASSO
	
	MOV R7, #2_00000000			;REGISTRADOR 7 QUE GUARDA A UNIDADE
	MOV R8, #2_00000000 		;REGISTRADOR 8 QUE GUARDA A DEZENA
	
	MOV R10, #0					;REGISTRADOR 10 QUE ITERA A SEQUÊNCIA DOS LEDS
	MOV R12, #0					;REGISTRADOR 12 QUE É UTILIZADO PARA VERIFICAR SE O PASSO É NEGATIVO

;Clock da placa, quando o R5 chegar em 50 ele altera o valor a ser imprimido nos displays e leds 
Clock
	BL Update_Displays
	BL GetInputJ				;Chama a subrotina que lê o estado das chaves e coloca o resultado em R0
	BIC R3, R4, R0	
	ADD R5, #1
	CMP R5, #50	
	BEQ Process
	MOV R4, R0

;Função que controla o Switch 1
SW1	
	CMP R3, #2_00000001			;Verifica se somente o SW1 está pressionado
	BNE SW2             		;Se não estiver pressionada vai pra SW2
	
	MOV R3, R6				
	
	AND R12, R3, #2_11110000 ;Verifica se é negativo
	CMP R12, #2_11110000
	BEQ Decrescent
		
	ADD R3, #1					;Realiza o incremento do passo, caso o próximo passo seja 10 ele reinicia para 1
	CMP R3, #2_00001010
	ITE EQ
		MOVEQ R6, #2_00000001
		MOVNE R6, R3
	
	B Clock
	
;Função que altera o passo caso seja negativo
Decrescent

	SUB R3, #1					;Realiza o incremento do passo, caso o próximo passo seja -10 ele reinicia para -1
	CMP R3, #-10
	ITE EQ
		MOVEQ R6, #-1
		MOVNE R6, R3
		
	B Clock
	
;Função que controla o Switch 2
SW2	
	CMP R3, #2_00000010			 ;Verifica se somente a chave SW2 está pressionada
	BNE Clock
	
	MVN R3, R6					 ;Nega o valor atual do passo, invertendo-o
	ADD R3, R3, #1	
	MOV R6, R3
		
	B Clock
	
;Função de processo, responsável por atualizar o próximo valor dos displays e leds
Process							
	ADD R9, R6
	
	CMP R9, #99  			;Se o próximo valor dos displays for > 99, subtrai 100
	IT GT
		SUBGT R9, #100 ; R9 = 100 - 100 = 0
		
	CMP	R9, #0				;Se o próximo valor dos displays for < 0, soma 100
	IT LT
		ADDLT R9, #100
		
	MOV R3, #10
	UDIV R8, R9, R3			;Altera a dezena, pegando o dígito mais significativo de R9
	MUL R2, R8, R3
	SUB R7, R9, R2			;Altera a unidade, subtraindo de R9 o valor da dezena
	
	CMP R10, #5				;Altera o valor dos leds, a partir da posição da tabela
	ITE LT
		ADDLT R10, #1
		MOVGE R10, #0
	
	MOV R5, #0				;Reinicia flag do cronômetro

Update_Displays
	PUSH {LR}

	MOV R0, R8
	BL Display1				;Imprime Display1

	MOV R0, R7
	BL Display2				;Imprime Display2
	
	MOV R0, R10
	BL Display_Leds			;Imprime Leds
	
	POP {LR}
	
	BX LR

Display_Leds
	PUSH {LR}
	
	LDR  R1, =LEDS_SEQUENCE ;Recebe tabela
	LDRB R2, [R1, R0]		;Pega item da tabela
	
	MOV R0, R2
	BL SetOutputBits		;Seta os bits 
	
	MOV R0, #2_00100000		;Ativa o PP5
	BL SwitchPP				;Habilita porta P
	MOV R0, #1
	BL SysTick_Wait1ms		
	
	MOV R0, #0
	BL SwitchPP				;Desabilita porta P
	MOV R0, #1
	BL SysTick_Wait1ms		
	
	POP {LR}
	BX LR	

Display1
	PUSH {LR}
	
	LDR  R1, =DISPLAY_NUMBERS
	LDRB R2, [R1, R0]
	
	MOV R0, R2
	BL SetOutputBits
	
	mov R0, #2_00010000			;Ativa o PB4
	bl PrintDisplay
	
	POP {LR}
	BX LR

Display2
	PUSH {LR}
	
	LDR  R1, =DISPLAY_NUMBERS
	LDRB R2, [R1, R0]
	
	MOV R0, R2
	BL SetOutputBits

	mov R0, #2_00100000			;Ativa o PB5
	bl PrintDisplay
	
	POP {LR}
	BX LR

PrintDisplay

	PUSH{LR}
	
	BL SwitchPB			;Habilita a porta B
	MOV R0, #1
	BL SysTick_Wait1ms		
	
	MOV R0, #0
	BL SwitchPB			;Desabilita a porta B
	MOV R0, #1
	BL SysTick_Wait1ms		
	
	POP{LR}
	BX LR

END_EXEC
	NOP
    ALIGN                           ; garante que o fim da se��o est� alinhada 
    END                             ; fim do arquivo
