;
; SoftwarePWM+ADC_Assembly.asm
;
; Created: 21.07.2017 13:00:55
; Author : s-del
;

.include "m16def.inc"

.org ADCCaddr rjmp ISR_ADCC              ; ADC Interrupt Vector Address


; Replace with your application code
start:
	LDI r16, LOW(RAMEND)		;	init the stack pointer
	OUT SPL, r16				;
	LDI r16, HIGH(RAMEND)		;
	OUT SPH, r16				;
	rcall init					;

	; r17 holds the duty cycle for the software pwm
	; r16 = 0 temp var to count a cycle for pwm
main:
	//SBI
	//rcall readadc				;	get the new duty cycle
	//LDI r17, 255				;	Override duty cycle
	ldi r16, 0x00				;	0 means led on	
	ldi r18, 0x00				;
	
/*	
lp1:cp r16, r17
	brne PC + 2
	ldi r18, 0xFF				;	r16 == r17
	OUT PORTC, r18				;	r16 == r17 || r16 != r17
	inc r16
	brne lp1
*/
		
/*								; ADC l�uft 100% sicher
	COM r17
	OUT PORTC, r17
	COM r17*/
	
	
	rjmp main					;	loop

init:
	LDI r16, 0xFF				;	PORTC as output
	OUT PORTC, 0x00
	OUT DDRC, r16				;
	//LDI r17, 0xFF				;	duty cycle in r17
	
	LDI r16, 0x67				;	set up adc (64|7|32)(0x67) (32 = 1<<5 =^= ADLAR (left adjust))
	OUT ADMUX, r16				;

	LDI r16, 0x87				;		(128|7)
	OUT ADCSRA, r16				;

	sei
	sbi ADCSRA, ADSC			;	start initial conversion
	ret



ISR_ADCC:
	in r15, SREG
	in r17, ADCH					;	duty cycle in r17 is set to the value in ADCH (because of left adjust) 
	sbi ADCSRA, ADSC				;	start conversion
	out SREG, r15
	reti
















readadc:
	SBI ADCSRA, ADSC				;	start conversion

lp2:SBIC ADCSRA, ADSC				;	check if bit ADSC (conversion running) bit is set
	rjmp lp2						;	wait as long it is set
	
	
	IN r17, ADCH					;	duty cycle in r17 is set to the value in ADCH (because of left adjust) 
	
	LDI r17, 255					; Override Duty Cycle
	ret


; Generated by delay loop calculator
; at http://www.bretmulvey.com/avrdelay.html
;
; Delay 7 372 800 cycles
; 500ms at 14.745600 MHz
; 1 Cycle takes 1T := 1/F_CPU
delay:					; 3*T
    ldi  r18, 38		; Rout	outer register
    ldi  r19, 103		; Rmid	mid register
    ldi  r20, 246		; Rin	inner register
L1: dec  r20			; 1T							; erst 246 mal,		; erst 103 mal,		; erst
    brne L1				; 1T if false, else 2T			; dann je 256 mal	; dann je			; 38
    dec  r19			; 1T												; 256 mal			; mal,
    brne L1				; 1T if false, else 2T								;					; dann
    dec  r18			; 1T																	; 255
    brne L1				; 1T if false, else 2T													; mal
    rjmp PC+1			; 2T

	ret					; 4T