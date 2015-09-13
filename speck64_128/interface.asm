
; global interrupt disable
	cli
; initialize stack
	ldi		r31,HIGH(RAMEND)
	out		SPH,r31
	ldi		r31,LOW(RAMEND)
	out		SPL,r31

; initialize trigger B1
  	ldi		r16, 0b11	; portB,1 = output (triggers)
  	out		DDRB, r16

	rjmp	main


    .include "./speck64_128.asm"

.CSEG
;******************** Q ELEC FUNCTIONS (START) *********************
; wait : ret + 0xFF * (5*nop + 1*dec + 1*brbc)
wait:
	ldi		r16, 0xFF	;r16=FF
w_loop:
	nop
	nop
	nop
	nop
	nop
	dec		r16			; r16=r16-1
	brbc	1,w_loop	; branch sur loop si Z=0, c¨¤s si r16 != 0
	ret					; return from subroutine

; wait2 : r17 * wait (to be set inside) + some instructions
wait2:
	ldi		r17, 0xFF	;
w_loop2:
	rcall	wait
	dec		r17			; r17=r17-1
	brbc	1,w_loop2	; branch sur loop2 si Z=0, c¨¤s si r17 != 0
	ret					; return from subroutine
;******************** Q ELEC FUNCTIONS (END) *********************

;******************** MAIN (START) *******************************
main:
	ldi 	XH, high(SRAM_PTEXT)
	ldi 	XL, low(SRAM_PTEXT)
	ldi		r18, PTEXT_NUM_BYTE
PTEXT_LOOP:
	st X+, r18
	dec r18
	brbc 1, PTEXT_LOOP; branch if the bit in SREG is cleared. The 1 bit of SREG is Z, so it mean "branch if the result is not 0"

	; k0 = [16, 15, 14, 13]
	ldi 	XH, high(SRAM_KEYS)
	ldi 	XL, low(SRAM_KEYS)
	ldi		r18, MASTER_KEY_NUM_BYTE
	st X+, r18
	dec r18
	st X+, r18
	dec r18
	st X+, r18
	dec r18
	st X+, r18
	dec r18
	; l0 = [12, 11, 10, 9]; l1 = [8, 7, 6, 5]; l2 = [4, 3, 2, 1]
	ldi 	XH, high(SRAM_L)
	ldi 	XL, low(SRAM_L)
	ldi		r18, 12
KEY_LOOP:
	st X+, r18
	dec r18
	brbc 1, KEY_LOOP

	sbi		PORTB,1		; portA,0 = high (trigger on port A0)
	nop
	nop
	nop
	nop
 	cbi		PORTB,1		; portA,0 = low

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

#ifdef KEYSCHEDULE
	rcall keyschedule 
#endif
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
#ifdef ENCRYPT	
	rcall	encrypt		; encryption routine
#endif
	
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

#ifdef DECRYPT
	rcall   decrypt      ; encryption routine
#endif

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	sbi		PORTB,0		; portA,0 = high (trigger on port A0)
	nop
	nop
	nop
	nop
 	cbi		PORTB,0		; portA,0 = low


	;make a pause
	rcall	wait2
	rcall	wait2
	rcall	wait2
	rcall	wait2
	rcall	wait2


.DSEG
  SRAM_KEYS: .BYTE KEYS_NUM_BYTE;        the 44*4 bytes of round keys
  SRAM_PTEXT: .BYTE PTEXT_NUM_BYTE;      the 16 blocks(each block has 8 bytes) of plaintext. For each block, the byte is from high to low.
  SRAM_L: .byte 12
;******************** MAIN (END) *********************************