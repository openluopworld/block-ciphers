/*
 * pride64_128.asm
 *
 *  Created: 2015/9/14 17:10:59
 *   Author: Administrator
 */ 

/*
 * 
 */

.EQU    PTEXT_NUM_BYTE = 8
.EQU	MASTER_KEY_NUM_BYTE = 16
.EQU	KEY0_NUM_BYTE = 8
.EQU	FIXED_KEYS_NUM_BYTE = 4;
.EQU    KEYS_NUM_BYTE = 80; half of rounds key is the same, so it can reduce to 80 bytes.
.EQU	KEY_SCHEDULE_ROUNDS = 21; begin with 1 not 0
.EQU	ENC_DEC_ROUNDS = 19

#define KEYSCHEDULE
#define ENCRYPT
#define DECRYPT

#ifdef KEYSCHEDULE
.def k11 = r4;
.def k13 = r5;
.def k15 = r6;
.def k17 = r7;
; const values, only used in key schedule
.def const193 = r16;
.def const165 = r17;
.def const81  = r18;
.def const197 = r19;
#endif

#ifdef ENCRYPT
.def s0 = r0
.def s1 = r1
.def s2 = r2
.def s3 = r3
.def s4 = r4
.def s5 = r5
.def s6 = r6
.def s7 = r7

.def rk0 = r8
.def rk1 = r9
.def rk2 = r10
.def rk3 = r11
.def rk4 = r12
.def rk5 = r13
.def rk6 = r14
.def rk7 = r15

.def t0 = r16;
.def t1 = r17;
.def t2 = r18;
.def t3 = r19;
#endif

.def currentRound = r24;
.def temp = r25;

	/*
	 * Subroutine: keyschedule
	 * Function:   compute the sub keys.
	 * Register:   [r0-r1] store the result of mul
	 *             [r4-r7] store k11(index 1 byte), k13(index 3 byte), k15(index 5 byte), k17(index 7 byte) of master key k1.
	 *             [r16-r19] const values
	 *             r24 currentRound
	 *             r25 temp
	 *             X
	 *             Y
	 */
#if defined(KEYSCHEDULE)
keyschedule:
	; set the fixed four bytes
	ldi r26, low(SRAM_KEYS);
	ldi r27, high(SRAM_KEYS);
	ldi r28, low(SRAM_KEYS_FIXED_FOUR);
	ldi r29, high(SRAM_KEYS_FIXED_FOUR);
	clr currentRound;
fixedBytes:
	ld temp, x+;
	adiw x, 1;
	st y+, temp;
	inc currentRound;
	cpi currentRound, FIXED_KEYS_NUM_BYTE;
	brne fixedBytes;
	; set the unfixed four bytes
	ldi r26, low(SRAM_KEYS);
	ldi r27, high(SRAM_KEYS);
	ldi r28, low(SRAM_KEYS);
	ldi r29, high(SRAM_KEYS);
	clr currentRound;
unFixedBytes:
	adiw x, 1;
	ld temp, x+;
	st y+, temp;
	inc currentRound;
	cpi currentRound, FIXED_KEYS_NUM_BYTE;
	brne unFixedBytes;
	
	; compute round keys
	ldi const193, 193;
	ldi const165, 165;
	ldi const81, 81;
	ldi const197, 197;
	ldi r26, low(SRAM_KEYS);
	ldi r27, high(SRAM_KEYS);
	ld k11, x+;
	ld k13, x+;
	ld k15, x+;
	ld k17, x+;
	sbiw x, 4;
	ldi currentRound, 1;
keysExtend:
	mul const193, currentRound;
	add r0, k11;
	st x+, r0;
	mul const165, currentRound;
	add r0, k13;
	st x+, r0;
	mul const81, currentRound;
	add r0, k15;
	st x+, r0;
	mul const197, currentRound;
	add r0, k17;
	st x+, r0;
	inc currentRound;
	cpi currentRound, KEY_SCHEDULE_ROUNDS;
	brne keysExtend;
	ret;
#endif

	/*
	 * Subroutine: encrypt
	 * Function:   encyrpt the 128 bytes of data
	 * Register:   [r0-r7] s0-s7, plain text
	 *             [r8-r15], round keys
	 */
#ifdef ENCRYPT
encrypt:
	; start encyrption
	ldi r26, low(SRAM_PTEXT); x stores the current address of data
	ldi r27, high(SRAM_PTEXT);
	ld s0, x+ ;
	ld s1, x+ ;
	ld s2, x+ ;
	ld s3, x+ ;
	ld s4, x+ ;
	ld s5, x+ ;
	ld s6, x+ ;
	ld s7, x+ ;
	; whitening key0
	ldi r28, low(SRAM_KEY0); x stores the current address of data
	ldi r29, high(SRAM_KEY0);
	ld rk0, y+;
	ld rk1, y+;
	ld rk2, y+;
	ld rk3, y+;
	ld rk4, y+;
	ld rk5, y+;
	ld rk6, y+;
	ld rk7, y+;
	; eor k0
	eor s0, rk0;
	eor s1, rk1;
	eor s2, rk2;
	eor s3, rk3;
	eor s4, rk4;
	eor s5, rk5;
	eor s6, rk6;
	eor s7, rk7;
	
	clr currentRound; reset
	ldi r28, low(SRAM_KEYS_FIXED_FOUR); stores the start address of keys
	ldi r29, high(SRAM_KEYS_FIXED_FOUR);
	ld rk0, y+;
	ld rk2, y+;
	ld rk4, y+;
	ld rk6, y+;
	ldi r28, low(SRAM_KEYS); stores the start address of keys
	ldi r29, high(SRAM_KEYS);
encLoop:
	ld rk1, y+;
	ld rk3, y+;
	ld rk5, y+;
	ld rk7, y+;
	; eor round keys
	eor s0, rk0;
	eor s1, rk1;
	eor s2, rk2;
	eor s3, rk3;
	eor s4, rk4;
	eor s5, rk5;
	eor s6, rk6;
	eor s7, rk7;
	; Substitution Layer
	movw t0, s0
	movw t2, s2
	and s0, s2
	eor s0, s4
	and s2, s4
	eor s2, s6
	and s1, s3
	eor s1, s5
	and s3, s5
	eor s3, s7
	movw s4, s0
	movw s6, s2
	and s4, s6
	eor s4, t0
	and s6, s4
	eor s6, t2
	and s5, s7
	eor s5, t1
	and s7, s5
	eor s7, t3
	; State s0, s1, s2, s3, s4, s5, s6, s7
	; Temporary registers t0, t1, t2, t3
	; Linear Layer and Inverse Linear Layer: L0
	movw t0, s0 ; t1:t0 = s1:s0
	swap s0
	swap s1
	eor s0, s1
	eor t0, s0
	mov s1, t0
	eor s0, t1
	; Linear Layer: L1
	swap s3
	movw t0, s2 ; t1:t0 = s3:s2
	movw t2, s2 ; t3:t2 = s3:s2
	lsl t0
	rol t2
	lsr t1
	ror t3
	eor s2, t3
	mov t0, s2
	eor s2, t2
	eor s3, t0
	; Linear Layer: L2
	swap s4
	movw t0, s4 ; t1:t0 = s5:s4
	movw t2, s4 ; t3:t2 = s5:s4
	lsl t0
	rol t2
	lsr t1
	ror t3
	eor s4, t3
	mov t0, s4
	eor s4, t2
	eor s5, t0
	; Linear Layer and Inverse Linear Layer: L3
	movw t0, s6 ; t1:t0 = s7:s6
	swap s6
	swap s7
	eor s6, s7
	eor t1, s6
	mov s7, t1
	eor s6, t0

	inc currentRound;
	cpi currentRound, ENC_DEC_ROUNDS;
	breq lastRound;
	jmp encLoop;
lastRound:
	ld rk1, y+;
	ld rk3, y+;
	ld rk5, y+;
	ld rk7, y+;
	; eor round keys
	eor s0, rk0;
	eor s1, rk1;
	eor s2, rk2;
	eor s3, rk3;
	eor s4, rk4;
	eor s5, rk5;
	eor s6, rk6;
	eor s7, rk7;
	; Substitution Layer
	movw t0, s0
	movw t2, s2
	and s0, s2
	eor s0, s4
	and s2, s4
	eor s2, s6
	and s1, s3
	eor s1, s5
	and s3, s5
	eor s3, s7
	movw s4, s0
	movw s6, s2
	and s4, s6
	eor s4, t0
	and s6, s4
	eor s6, t2
	and s5, s7
	eor s5, t1
	and s7, s5
	eor s7, t3
	; whitening key2
	ldi r28, low(SRAM_KEY0); x stores the current address of data
	ldi r29, high(SRAM_KEY0);
	ld rk0, y+;
	ld rk1, y+;
	ld rk2, y+;
	ld rk3, y+;
	ld rk4, y+;
	ld rk5, y+;
	ld rk6, y+;
	ld rk7, y+;
	; eor k0
	eor s0, rk0;
	eor s1, rk1;
	eor s2, rk2;
	eor s3, rk3;
	eor s4, rk4;
	eor s5, rk5;
	eor s6, rk6;
	eor s7, rk7;
	; move cipher text back to plain text
	st -x, r7;
	st -x, r6;
	st -x, r5;
	st -x, r4;
	st -x, r3;
	st -x, r2;
	st -x, r1;
	st -x, r0;
	ret;
#endif

	/*
	 * Subroutine: decrypt
	 * Function:   decyrpt the 128 bytes of data
	 * Register:   
	 */
#ifdef	DECRYPT
decrypt:
	ldi r26, low(SRAM_PTEXT); x stores the current address of data
	ldi r27, high(SRAM_PTEXT);
	; load the ciphertext from RAM to registers [r7,...,r0], X = [r7, r6, r5, r4], Y = [r3, r2, r1, r0]
	ld r7, x+ ;
	ld r6, x+ ;
	ld r5, x+ ;
	ld r4, x+ ;
	ld r3, x+ ;
	ld r2, x+ ;
	ld r1, x+ ;
	ld r0, x+ ;

	ldi r28, low(SRAM_PTEXT); the address of keys
	ldi r29, high(SRAM_PTEXT);
	;sbiw r28, 1; y is just the start address of keys + 176. So it does not need sub 1.
	clr currentRound; reset
	clr r30; r30 is useless this time
decLoop:
	inc currentRound;
	cpi currentRound, ENC_DEC_ROUNDS;
	brne decLoop;
	; move cipher text back to plain text
	st -x, r0;
	st -x, r1;
	st -x, r2;
	st -x, r3;
	st -x, r4;
	st -x, r5;
	st -x, r6;
	st -x, r7;

	ret; the end point is here or not makes much difference.
#endif
