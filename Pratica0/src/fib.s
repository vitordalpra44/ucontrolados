; Exemplo.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 12/03/2018

; -------------------------------------------------------------------------------
        THUMB                        ; Instruções do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declarações EQU - Defines
;<NOME>         EQU <VALOR>
; -------------------------------------------------------------------------------
; Área de Dados - Declarações de variáveis
		AREA  DATA, ALIGN=2
		; Se alguma variável for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a variável <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma variável de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posição da RAM		

; -------------------------------------------------------------------------------
; Área de Código - Tudo abaixo da diretiva a seguir será armazenado na memória de 
;                  código
        AREA    |.text|, CODE, READONLY, ALIGN=2
		
		; Se alguma função do arquivo for chamada em outro arquivo	
        EXPORT Start                ; Permite chamar a função Start a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma função externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; função <func>

		
; -------------------------------------------------------------------------------
; Função main()
memoRam EQU 0x20000400
memoOrd EQU 0x20000500
Start  
; Comece o código aqui <======================================================

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
    ALIGN                           ; garante que o fim da seção está alinhada 
    END                             ; fim do arquivo
