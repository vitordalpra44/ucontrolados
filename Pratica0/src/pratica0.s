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
NUMBERS_LIST	EQU	0x20000400
FIB_LIST		EQU 0x20000500
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

		
; -------------------------------------------------------------------------------
; Fun��o main()
START 
; Comece o c�digo aqui <======================================================
		
LIST_ELEMENT_STORAGE

	LDR     R0, =NUMBERS_LIST

	MOV     R1, #3
	STRB    R1, [R0], #1

	MOV     R1, #244
	STRB    R1, [R0], #1

	MOV     R1, #14
	STRB    R1, [R0], #1

	MOV     R1, #233
	STRB    R1, [R0], #1

	MOV     R1, #1
	STRB    R1, [R0], #1

	MOV     R1, #6
	STRB    R1, [R0], #1

	MOV     R1, #9
	STRB    R1, [R0], #1

	MOV     R1, #18
	STRB    R1, [R0], #1

	MOV     R1, #13
	STRB    R1, [R0], #1

	MOV     R1, #254
	STRB    R1, [R0], #1

	MOV     R1, #21
	STRB    R1, [R0], #1

	MOV     R1, #34
	STRB    R1, [R0], #1

	MOV     R1, #2
	STRB    R1, [R0], #1

	MOV     R1, #67
	STRB    R1, [R0], #1

	MOV     R1, #135
	STRB    R1, [R0], #1

	MOV     R1, #8
	STRB    R1, [R0], #1

	MOV     R1, #89
	STRB    R1, [R0], #1

	MOV     R1, #43
	STRB    R1, [R0], #1

	MOV     R1, #5
	STRB    R1, [R0], #1

	MOV     R1, #105
	STRB    R1, [R0], #1

	MOV     R1, #144
	STRB    R1, [R0], #1

	MOV     R1, #201
	STRB    R1, [R0], #1

	MOV     R1, #55
	STRB    R1, [R0], #1
	
	
; R0 � o endere�o da lista bruta
; R1 � o endere�o da lista fib
; R2 e R3 e R4 s�o os valores que iteram do fibonacci
; R5 � o tamanho da lista original
; R6 � o valor atual da lista bruta 
; R7 � o ultimo valor fib encontrado na lista bruta

	
	LDR     R0, =NUMBERS_LIST    ; Endere�o base da lista
	LDR 	R1, =FIB_LIST
	MOV     R5, #23         ; Tamanho da lista

LOOP
	LDRB    R6, [R0], #1    ; Carrega o elemento atual para R1 e incrementa R0
	BL	ISFIB
	
	SUBS    R5, R5, #1      ; Decrementa o contador
	BNE     LOOP            ; Se R2 n�o � 0, loop
	B		SORT
	
	
	
ISFIB	
	MOV R2,#0
	MOV R3, #1
LOOPFIB		
	CMP R6, R3
	BEQ FIBFOUND
	
	
	ADD R4, R3, R2
	MOV R2, R3
	MOV R3, R4
		
	CMP R3, R6
	BHI NOTFIB
	
	B LOOPFIB
				
FIBFOUND
	MOV R7,R6
	STRB R7,[R1], #1
	BX LR
NOTFIB
	BX LR
		
SORT
	
;R0 � o endere�o inicio da lista fib FIXO
;R1 � o endere�o do elemento que est� em ordem e est� sendo comparado
;R2 � o endere�o do elemento que quer entrar na ordem e est� sendo comparado
;R3 � o valor do elemento que est� em ordem e est� sendo comparado
;R4 � o valor do elemento que quer entrar na ordem e est� sendo comparado
;R5 � o valor do ultimo endere�o da lista fib FIXO
;R6 � o auxiliar para ir diminuindo os endere�os
;R7 � auxiliar de valor
	LDR		R0, =FIB_LIST     ; Endere�o inicial da lista fibonacci
	sub		R5, R1, #0x1
	ADD 	R1, R0, #1        ; R1 aponta para o segundo elemento da lista

NEXT_ELEMENT
	CMP 	R1, R5            ; Compara se R1 passou do �ltimo elemento
	BHI 	END_EXEC               ; Se sim, terminamos a ordena��o

	LDRB	R4, [R1]          ; Carrega o elemento atual (que quer entrar na ordem) em R4
	MOV     R6, R1            ; R6 � usado para comparar com os elementos anteriores

COMPARE_WITH_PREVIOUS
	SUBS    R6, R6, #1        ; Move R6 para o elemento anterior
	CMP     R6, R0            ; Verifica se R6 est� antes do in�cio da lista
	BLO     SHIFT_DONE        ; Se sim, terminou de deslocar para a direita
	
	;Se n�o:
	LDRB    R3, [R6]          ; Carrega o valor do elemento anterior em R3
	CMP     R4, R3            ; Compara os valores dos elementos
	BCS     SHIFT_DONE        ; Se R4 >= R3, o deslocamento est� conclu�do

	STRB    R3, [R6, #1]      ; Desloca o elemento anterior para a direita
	B       COMPARE_WITH_PREVIOUS

SHIFT_DONE
	STRB    R4, [R6, #1]      ; Insere o elemento na posi��o correta

	ADD     R1, R1, #1        ; Move para o pr�ximo elemento a ser inserido
	B       NEXT_ELEMENT



END_EXEC
	NOP
    ALIGN                           ; garante que o fim da se��o est� alinhada 
    END                             ; fim do arquivo
