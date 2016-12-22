/*
 * skinny_128_128.asm
 *
 *  Created: 2016/12/22 9:20:25
 *   Author: Administrator
 */ 

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


    .include "./skinny-128-128-reference.asm"

.CSEG

;******************** FUNCTIONS ***********************************
; This function copy bytes from ROM location pointed by ZH:ZL to
; SRAM location pointed by XH:XL
;	Z must contain source address
;	X must contain destination address
;	r18 must contain the number of byte to copy
;	r16 is used for internal computation
copy_word:
	lpm		r16, Z+
	st 		X+,r16
	dec		r18
	brbc	1, copy_word
	ret
;*******************************************************************

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

	;load round constants
	ldi		ZH, high(SBOX<<1)
	ldi		ZL, low(SBOX<<1)
	ldi 	XH, high(SRAM_SBOX)
	ldi 	XL, low(SRAM_SBOX)
	ldi		r18, SBOX_NUM_BYTE
	rcall	copy_word

	;load round constants
	ldi		ZH, high(RC<<1)
	ldi		ZL, low(RC<<1)
	ldi 	XH, high(SRAM_RC)
	ldi 	XL, low(SRAM_RC)
	ldi		r18, ENC_DEC_ROUNDS
	rcall	copy_word

	;load plain text
	ldi		ZH, high(PTEXT<<1)
	ldi		ZL, low(PTEXT<<1)
	ldi 	XH, high(SRAM_PTEXT)
	ldi 	XL, low(SRAM_PTEXT)
	ldi		r18, PTEXT_NUM_BYTE
	rcall	copy_word
	
	;load keys
	ldi		ZH, high(KEY<<1)
	ldi		ZL, low(KEY<<1)
	ldi 	XH, high(SRAM_KEY)
	ldi 	XL, low(SRAM_KEY)
	ldi		r18, KEY_NUM_BYTE
	rcall	copy_word

main_loop:
	sbi		PORTB,1		; portA,0 = high (trigger on port A0)
	nop
	nop
	nop
	nop
 	cbi		PORTB,1		; portA,0 = low

	ldi 	XH, high(SRAM_PTEXT)
	ldi 	XL, low(SRAM_PTEXT)
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
	rcall load_plaintext

	ldi 	YH, high(SRAM_KEY)
	ldi 	YL, low(SRAM_KEY)
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
	rcall load_halfkey

	ldi 	ZH, high(SRAM_SBOX)
	ldi 	ZL, low(SRAM_SBOX)
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
	rcall	encrypt		; encryption routine

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

	;encrypt again, with the cipher as the new plain
	rjmp	main_loop

SBOX:
.DB 0x0c, 0x06, 0x09, 0x00, 0x01, 0x0a, 0x02, 0x0b, 0x03, 0x08, 0x05, 0x0d, 0x04, 0x0e, 0x07, 0x0f
RC:
.DB 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F
.DB 0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B
.DB 0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E
.DB 0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A
.DB 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04, 0x09, 0x13
;5f 9f 3f c4 b9 d8 43 61 ba 11 c1 a4 03 bc c0 e4
PTEXT:
.DB 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
KEY: 
.DB 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f

.DSEG 
.ORG 0x0200
  SRAM_SBOX: .BYTE SBOX_NUM_BYTE
  SRAM_RC: .BYTE ENC_DEC_ROUNDS
  SRAM_PTEXT: .BYTE PTEXT_NUM_BYTE
  SRAM_KEY: .BYTE KEY_NUM_BYTE
;******************** MAIN (END) *********************************