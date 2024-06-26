; pratica0.s
; Desenvolvido para a placa EK-TM4C1294XL
; VItor Dalpr� / Gabriel Willig / Jo?o Mendes
; 24/03/2024
; Feito com base no exemplo.s do professor Guilherme de Santi Peron de Sistemas Microcontrolados da UTFPR.




; -------------------------------------------------------------------------------
        THUMB                        ; Instru??es do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declara??es EQU - Defines
;<NOME>         EQU <VALOR>
DISPLAY_0		EQU 2_00111111
DISPLAY_1		EQU 2_00000110
DISPLAY_2		EQU 2_01011011
DISPLAY_3		EQU 2_01001111
DISPLAY_4		EQU 2_01100110
DISPLAY_5		EQU 2_01101101
DISPLAY_6		EQU 2_01111101
DISPLAY_7		EQU 2_00000111
DISPLAY_8		EQU 2_01111111
DISPLAY_9		EQU 2_01101111	

; -------------------------------------------------------------------------------
; ?rea de Dados - Declara??es de vari?veis
		AREA  DATA, ALIGN=2

			

		; Se alguma vari?vel for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a vari?vel <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma vari?vel de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posi??o da RAM		

; -------------------------------------------------------------------------------
; ?rea de C?digo - Tudo abaixo da diretiva a seguir ser? armazenado na mem?ria de 
;                  c?digo
        AREA    |.text|, CODE, READONLY, ALIGN=2
		
		; Se alguma fun??o do arquivo for chamada em outro arquivo	
        EXPORT Decoder                ; Permite chamar a fun??o START a partir de 
		EXPORT Increment	                        ; outro arquivo. No caso startup.s
		EXPORT Decrement
		

		; Se chamar alguma fun??o externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; fun??o <func>
		IMPORT GetOutputBits
		IMPORT SetOutputBits

; -------------------------------------------------------------------------------
; Fun??o main()
Decoder 
; Comece o c?digo aqui <======================================================

Increment
	push {R0, R2, LR}
	
	BL GetOutputBits
	
	CMP R0, #DISPLAY_0
	BEQ Set_1
	
	CMP R0, #DISPLAY_1
	BEQ Set_2
	
	CMP R0, #DISPLAY_2
	BEQ Set_3
	
	CMP R0, #DISPLAY_3
	BEQ Set_4
	
	CMP R0, #DISPLAY_4
	BEQ Set_5
	
	CMP R0, #DISPLAY_5
	BEQ Set_6
	
	CMP R0, #DISPLAY_6
	BEQ Set_7
	
	CMP R0, #DISPLAY_7
	BEQ Set_8
	
	CMP R0, #DISPLAY_8
	BEQ Set_9
	
	CMP R0, #DISPLAY_9
	BEQ Set_0
	
	
Exit_Increment
	pop {R0, R2, LR}
	BX LR

Decrement
	push {R0, R2, LR}
	
	BL GetOutputBits
	
	CMP R0, #DISPLAY_0
	BEQ Set_9
	
	CMP R0, #DISPLAY_1
	BEQ Set_0
	
	CMP R0, #DISPLAY_2
	BEQ Set_1
	
	CMP R0, #DISPLAY_3
	BEQ Set_2
	
	CMP R0, #DISPLAY_4
	BEQ Set_3
	
	CMP R0, #DISPLAY_5
	BEQ Set_4
	
	CMP R0, #DISPLAY_6
	BEQ Set_5
	
	CMP R0, #DISPLAY_7
	BEQ Set_6
	
	CMP R0, #DISPLAY_8
	BEQ Set_7
	
	CMP R0, #DISPLAY_9
	BEQ Set_8

Set_0
	push {R0, LR}
	ldr R0, =DISPLAY_0
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_1
	push {R0, LR}
	ldr R0, =DISPLAY_1
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment
Set_2
	push {R0, LR}
	ldr R0, =DISPLAY_2
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_3
	push {R0, LR}
	ldr R0, =DISPLAY_3
	
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_4
	push {R0, LR}
	ldr R0, =DISPLAY_4
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_5
	push {R0, LR}
	ldr R0, =DISPLAY_5
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_6
	push {R0, LR}
	ldr R0, =DISPLAY_6
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_7
	push {R0, LR}
	ldr R0, =DISPLAY_7
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

Set_8
	push {R0, LR}
	ldr R0, =DISPLAY_8
	BL SetOutputBits
	pop {R0, LR}	
	B Exit_Increment

Set_9
	push {R0, LR}
	ldr R0, =DISPLAY_9
	BL SetOutputBits
	pop {R0, LR}
	B Exit_Increment

END_EXEC
	NOP
    ALIGN                           ; garante que o fim da se??o est? alinhada 
    END                             ; fim do arquivo
