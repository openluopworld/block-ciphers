/*
 *
 * Chinese Academy of Sciences
 * State Key Laboratory of Information Security, 
 * Institute of Information Engineering
 *
 * Copyright (C) 2016 Chinese Academy of Sciences
 *
 * Written in 2016 by Luo Peng <luopeng@iie.ac.cn>
 *
 */
#include <stdint.h>

#include "cipher.h"
#include "constants.h"

#ifdef OPT_FLASH
void Decrypt(uint8_t *block, uint8_t *roundKeys)
{
	uint16_t *block16 = (uint16_t*)block;
	uint16_t *roundKeys16 = (uint16_t*)roundKeys;
	// point to the start address of round 26
	roundKeys16 += 100;

	uint16_t w0 = *block16;     // first line
	uint16_t w1 = *(block16+1); // second line
	uint16_t w2 = *(block16+2); // third line
	uint16_t w3 = *(block16+3); // forth line

	uint16_t sbox0;
	uint8_t i;
	for ( i = 0; i <= NUMBER_OF_ROUNDS; i++ ) {
		/* AddRoundKey */
		w0 ^= READ_ROUND_KEY_WORD(*(roundKeys16));
		w1 ^= READ_ROUND_KEY_WORD(*(roundKeys16+1));
		w2 ^= READ_ROUND_KEY_WORD(*(roundKeys16+2));
		w3 ^= READ_ROUND_KEY_WORD(*(roundKeys16+3));
		if ( NUMBER_OF_ROUNDS == i ) {break;}
		roundKeys16 -= 4;
		/* ShiftRow */
		w1 = (w1>>1  | w1 << 15);
		w2 = (w2>>12 | w2 << 4);
		w3 = (w3>>13 | w3 << 3);
		/* Invert sbox */
		sbox0 =  w0;
		w0    &= w2;
		w0    ^= w3;
		w3    |= sbox0;
		w3    ^= w2;
		w1    ^= w3;
		w2    =  w1;
		w1    ^= sbox0;
		w1    ^= w0;
		w3    =  ~w3;
		sbox0 =  w3;
		w3    |= w1;
		w3    ^= w0;
		w0    &= w1;
		w0    ^= sbox0;
	}
	/* store plain text */
	*block16 = w0;
	*(block16+1) = w1;
	*(block16+2) = w2;
	*(block16+3) = w3;
}

#else
#ifdef OPT_RAM
void Decrypt(uint8_t *block, uint8_t *roundKeys)
{
	uint16_t *block16 = (uint16_t*)block;
	uint16_t *roundKeys16 = (uint16_t*)roundKeys;

	uint16_t w0 = *block16;     // first line
	uint16_t w1 = *(block16+1); // second line
	uint16_t w2 = *(block16+2); // third line
	uint16_t w3 = *(block16+3); // forth line

	roundKeys16 += 78;
	uint16_t k0, k3;
	uint32_t k12;
	k3 = READ_ROUND_KEY_WORD(*roundKeys16);
	
	uint16_t sbox0;
	uint8_t i;
	for ( i = 0; i <= NUMBER_OF_ROUNDS; i++ ) {
		/* AddRoundKey */
		k12 = READ_ROUND_KEY_DOUBLE_WORD(*(uint32_t*)(roundKeys16-2));
		k0 = READ_ROUND_KEY_WORD(*(roundKeys16-3));
		w0 ^= ( (k3&0xff00) | (((uint16_t)k12)&0x00ff) );
		w1 ^= ( (k0<<8) | (((uint16_t)k12)>>8) );
		w2 ^= ( (k3<<8) | (((uint16_t)(k12>>16))&0x00ff) );
		w3 ^= ( (k0&0xff00) | ((uint16_t)(k12>>24)) );
		if ( NUMBER_OF_ROUNDS == i ) {break;}
		roundKeys16 -= 3;
		k3 = k0;
		/* ShiftRow */
		w1 = (w1>>1  | w1 << 15);
		w2 = (w2>>12 | w2 << 4);
		w3 = (w3>>13 | w3 << 3);
		/* Invert sbox */
		sbox0 =  w0;
		w0    &= w2;
		w0    ^= w3;
		w3    |= sbox0;
		w3    ^= w2;
		w1    ^= w3;
		w2    =  w1;
		w1    ^= sbox0;
		w1    ^= w0;
		w3    =  ~w3;
		sbox0 =  w3;
		w3    |= w1;
		w3    ^= w0;
		w0    &= w1;
		w0    ^= sbox0;
	}
	/* store cipher text */
	*block16 = w0;
	*(block16+1) = w1;
	*(block16+2) = w2;
	*(block16+3) = w3;
}
#endif
#endif
