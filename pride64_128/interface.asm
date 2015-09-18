.ORG 0x0000
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


    .include "./pride64_128.asm"

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
	brbc	1,w_loop	; branch sur loop si Z=0, c¡§¡ès si r16 != 0
	ret					; return from subroutine



; wait2 : r17 * wait (to be set inside) + some instructions
wait2:
	ldi		r17, 0xFF	;
w_loop2:
	rcall	wait
	dec		r17			; r17=r17-1
	brbc	1,w_loop2	; branch sur loop2 si Z=0, c¡§¡ès si r17 != 0
	ret					; return from subroutine
;******************** Q ELEC FUNCTIONS (END) *********************


;******************** MAIN (START) *******************************
main:
	ldi 	XH, high(SRAM_PTEXT)
	ldi 	XL, low(SRAM_PTEXT)
	ldi		r18, PTEXT_NUM_BYTE
	ldi		r19, 0x0
PTEXT_LOOP:
	st x+, r19
	inc r19
	;st X+, r18
	dec r18
	brbc 1, PTEXT_LOOP

	ldi 	XH, high(SRAM_PTEXT)
	ldi 	XL, low(SRAM_PTEXT)
	ldi r18, 0x01;
	st x+, r18;
	ldi r18, 0x23;
	st x+, r18;
	ldi r18, 0x45;
	st x+, r18;
	ldi r18, 0x67;
	st x+, r18;
	ldi r18, 0x89;
	st x+, r18;
	ldi r18, 0xab;
	st x+, r18;
	ldi r18, 0xcd;
	st x+, r18;
	ldi r18, 0xef;
	st x+, r18;
	
	; the whitening key 0 and first round of round keys is initialized
	ldi 	XH, high(SRAM_KEY0)
	ldi 	XL, low(SRAM_KEY0)
	ldi		r18, MASTER_KEY_NUM_BYTE
	clr		r19
KEY_LOOP:
	st x+, r19
	;st X+, r18
	dec r18
	brbc 1, KEY_LOOP

	;fedcba9876543210
	ldi xl, low(SRAM_KEYS)
	ldi xh, high(SRAM_KEYS)
	ldi r18, 0xfe;
	st x+, r18;
	ldi r18, 0xdc;
	st x+, r18;
	ldi r18, 0xba;
	st x+, r18;
	ldi r18, 0x98;
	st x+, r18;
	ldi r18, 0x76;
	st x+, r18;
	ldi r18, 0x54;
	st x+, r18;
	ldi r18, 0x32;
	st x+, r18;
	ldi r18, 0x10;
	st x+, r18;

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
  SRAM_KEY0: .byte KEY0_NUM_BYTE;   the whitening key k0 and whitening key k2(same with k0)
  SRAM_KEYS: .BYTE KEYS_NUM_BYTE;   the 20*8 bytes of round keys
  SRAM_KEYS_FIXED_FOUR: .BYTE FIXED_KEYS_NUM_BYTE;    fixed four bytes of round keys
  SRAM_PTEXT: .BYTE PTEXT_NUM_BYTE; the 16 blocks(each block has 8 bytes) of plaintext. For each block, the byte is from high to low.
;******************** MAIN (END) *********************************