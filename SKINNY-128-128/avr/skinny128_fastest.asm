
/*
 * key schedule is done first
 * and the round constants are eor-ed with the round keys
 */

.def s0 	=	r0
.def s1 	=	r1
.def s2 	=	r2
.def s3 	=	r3
.def s4 	=	r4
.def s5 	=	r5
.def s6 	=	r6
.def s7 	=	r7
.def s8 	=	r8
.def s9 	=	r9
.def s10 	=	r10
.def s11 	=	r11
.def s12	=	r12
.def s13	=	r13
.def s14	=	r14
.def s15	=	r15

.def t0		=	r16
.def count	=	r17

.def k0		=	r18
.def k1		=	r19
.def k2		=	r20
.def k3		=	r21
.def k4		=	r22
.def k5		=	r23
.def k6		=	r24
.def k7		=	r25

.def t1		=	r30


; z stores the sbox
; x stores the round constants
; y stores the round keys

.macro sub_column
mov		r30,	s0
ld		s0,		z
mov		r30,	s1
ld		s1,		z
mov		r30,	s2
ld		s2,		z
mov		r30,	s3
ld		s3,		z
mov		r30,	s4
ld		s4,		z
mov		r30,	s5
ld		s5,		z
mov		r30,	s6
ld		s6,		z
mov		r30,	s7
ld		s7,		z
mov		r30,	s8
ld		s8,		z
mov		r30,	s9
ld		s9,		z
mov		r30,	s10
ld		s10,	z
mov		r30,	s11
ld		s11,	z
mov		r30,	s12
ld		s12,	z
mov		r30,	s13
ld		s13,	z
mov		r30,	s14
ld		s14,	z
mov		r30,	s15
ld		s15,	z
.endmacro

; add round constant
.macro add_const
ld		t0,		x+
mov		t1,		t0
andi	t1,		0x0f
eor 	s0,		t1
andi	t0,		0x30
swap	t0
eor 	s4,		t0
ldi		t0,		0x02
eor 	s8,		t0
.endmacro

; for different rounds, the function should be different
.macro add_key
eor 	s0,		k0
eor 	s1,		k1
eor 	s2,		k2
eor 	s3,		k3
eor 	s4,		k4
eor 	s5,		k5
eor 	s6,		k6
eor 	s7,		k7
ldd		t0,		y+0
eor 	s8,		t0
ldd		t0,		y+1
eor 	s9,		t0
ldd		t0,		y+2
eor 	s10,	t0
ldd		t0,		y+3
eor 	s11,	t0
ldd		t0,		y+4
eor 	s12,	t0
ldd		t0,		y+5
eor 	s13,	t0
ldd		t0,		y+6
eor 	s14,	t0
ldd		t0,		y+7
eor 	s15,	t0
.endmacro


; 0  1  2  3         9  15 8  13
; 4  5  6  7         10 14 12 11
; 8  9  10 11 -----> 0  1  2  3
; 12 13 14 15        4  5  6  7
.macro key_schedule
ldd		t0,		y+0
std		y+0,	s0
ldd		s0,		y+1
std		y+1,	s1
ldd		s1,		y+7
std		y+7,	s7
ldd		s7,		y+3
std		y+3,	s3
ldd		s3,		y+5
std		y+5,	s5
ldd		s5,		s+6
std		y+6,	s6
ldd		s6,		y+4
std		y+4,	s4
ldd		s4,		y+2
std		y+2,	s2
mov		s2,		t0
.endmacro

; 0  1  2  3         0  1  2  3
; 4  5  6  7         7  4  5  6
; 8  9  10 11 -----> 10 11 8  9
; 12 13 14 15        13 14 15 12
; eor 	s4,		s8
; eor 	s8,		s0
; eor 	s12,	s8
; mov	t0,		s12
; mov	s12,	s8
; mov	s8,		s4
; mov	s4,		s0
; mov	s0,		t0
.macro shift_row_mix_column
; first column
eor 	s7,		s10
eor 	s10,	s0
eor 	s13,	s10
mov		t0,		s13
mov		s13,	s10
mov		s10,	s7
mov		s7,		s0
mov		s0,		t0
; second column
eor 	s4,		s11
eor 	s11,	s1
eor 	s14,	s11
mov		t0,		s14
mov		s14,	s11
mov		s11,	s4
mov		s4,		s1
mov		s1,		t0
; third column
eor 	s5,		s8
eor 	s8,		s2
eor 	s15,	s8
mov		t0,		s15
mov		s15,	s8
mov		s8,		s5
mov		s5,		s2
mov		s2,		t0
; fourth column 3 6 9 12
eor 	s6,		s9
eor 	s9,		s3
eor 	s12,	s9
mov		t0,		s12
mov		s12,	s9
mov		s9,		s6
mov		s6,		s3
mov		s3,		t0
.endmacro


encrypt:
	ldi count, 40
enc_round:
	sub_column
	add_const
	add_key
	key_schedule
	shift_row_mix_column
	dec	count
	brne enc_round
	ret









