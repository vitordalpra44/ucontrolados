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
		IMPORT SetOutpuyBits
		IMPORT ACTIVATE_PP5_DEACTIVATE_PB4_PB5
		IMPORT SwitchPB4_PB5_PP5
		IMPORT SysTick_Wait1ms
		IMPORT  PLL_Init
		IMPORT  SysTick_Init
; -------------------------------------------------------------------------------
; Fun��o main()
START 
; Comece o c�digo aqui <======================================================
		
	BL GPIO_start                 ;Chama a subrotina que inicializa os GPIO
	BL PLL_Init                  ;Chama a subrotina para alterar o clock do microcontrolador para 80MHz
	BL SysTick_Init
	
	
	;setando os bits de R0 que vão aparecer nos leds/display
	mov R0, #2_10101010
	BL SetOutpuyBits
	
	
SWITCH_RAPIDO_DISPLAYS_E_LEDS
	MOV R0, #4               ;Chamar a rotina para esperar 4 ms
	BL SysTick_Wait1ms
	
	mov R0, #1 ;imprimindo nos LEDS
	bl SwitchPB4_PB5_PP5
	MOV R0, #4                ;Chamar a rotina para esperar 4ms
	BL SysTick_Wait1ms
	mov R0, #2;imprimindo no Display 1
	bl SwitchPB4_PB5_PP5
	MOV R0, #4                ;Chamar a rotina para esperar 4ms
	BL SysTick_Wait1ms
	mov R0, #4 ;imprimindo no Display 2
	bl SwitchPB4_PB5_PP5

	
	b SWITCH_RAPIDO_DISPLAYS_E_LEDS


END_EXEC
	NOP
    ALIGN                           ; garante que o fim da se��o est� alinhada 
    END                             ; fim do arquivo
