; Exemplo.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 12/03/2018

; -------------------------------------------------------------------------------
        THUMB                        ; Instru��es do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declara��es EQU - Defines
;<NOME>         EQU <VALOR>
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
        EXPORT Start                ; Permite chamar a fun��o Start a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma fun��o externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; fun��o <func>

		
; -------------------------------------------------------------------------------
; Fun��o main()
memoRam EQU 0x20000400
memoOrd EQU 0x20000500
Start  
; Comece o c�digo aqui <======================================================

		LDR R12, =0x20000400
		LDR R10, =0x20000400
		LDR R11, =memoOrd
		LDR R7, =memoRam
		
		LDR R6, =8
		STRB R6, [R7], #1
		
		LDR R6, =3
		STRB R6, [R7], #1
		LDR R6, =244
		STRB R6, [R7], #1
		LDR R6, =14
		STRB R6, [R7], #1
	
loop	
		LDRB R0, [R12] , #1 ; ler da lista na memoria
		BL isFib 
		ADD R10,R10, #1
		
		CMP R7,R10   ;VER SE CHEGOU NO FINAL DA LISTA
		BNE loop
		B fim
		
		
isFib
			
		MOV R1,#0
		MOV R2, #1
loopFib		
		CMP R0, R2
		BEQ achouFib
		
		
		
		ADD R3, R2, R1
		MOV R1,R2
		MOV R2,R3
		
		CMP R2,R0
		BHI naoFib
		
		B loopFib
				
achouFib
		MOV R8,R0
		STRB R8,[R11], #1
		BX LR
naoFib
		BX LR
		
		
	
fim	
	
	
	
	NOP
    ALIGN                           ; garante que o fim da se��o est� alinhada 
    END                             ; fim do arquivo
